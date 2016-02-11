//
//  ofxGenericValueStore.cpp
//  ofxGeneric
//
//  Handles saving and loading cache data. Cache is stored in the documents directory
//  and exists until it is manually purged. A purge would usually happen when the cache
//  values become invalid due to more up to date information coming from the server.
//
//  Created by Eli Delventhal on 4/10/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericValueStore.h"
#include "ofxGenericUtility.h"
#include "ofxGenericDate.h"
#include "ofxGenericHMAC.h"

#include "ofxJSONElement.h"
#include <tinyxml.h>

#include "ofUtils.h"

#define HmacStoreKey "hash"


#define LogValueStoreTypeError(type) \
    { \
        ofxGLogFatalError(" ofxGenericValueStore error [" + string(__PRETTY_FUNCTION__) + " Line: " + ofxGToString(__LINE__) + "] unsupported value type: " + ofxGToString( type ) ); \
    }

#define LogValueStoreValueError(type, value) \
    { \
        ofxGLogFatalError(" ofxGenericValueStore error [" + string(__PRETTY_FUNCTION__) + " Line: " + ofxGToString(__LINE__) + "] unsupported value " + ofxGToString( value ) + "for type: " + ofxGToString( type ) ); \
    }

ofPtr< ofxGenericValueStore > ofxGenericValueStore::create( bool asArray )
{
    ofxGenericValueStore::Type type;
    if ( asArray )
    {
        type = ofxGenericValueStoreTypeArray;
    } else 
    {
        type = ofxGenericValueStoreTypeObject;
    }
    return create( type );
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::create( ofxGenericValueStore::Type type )
{
    if ( type != ofxGenericValueStoreTypeUninitialized )
    {
        ofPtr< ofxGenericValueStore > create( new ofxGenericValueStore() );
        create->init( create, type );
        return create;   
    }
    return ofPtr< ofxGenericValueStore >();
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::createWithValue( float value )
{
    ofPtr< ofxGenericValueStore > create( new ofxGenericValueStore() );
    create->init( create, ofxGenericValueStoreTypeFloat );
    create->write( value );
    return create;
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::createWithValue( int value )
{
    ofPtr< ofxGenericValueStore > create( new ofxGenericValueStore() );
    create->init( create, ofxGenericValueStoreTypeInt );
    create->write( value );
    return create;
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::createWithValue( unsigned int value )
{
    ofPtr< ofxGenericValueStore > create( new ofxGenericValueStore() );
    create->init( create, ofxGenericValueStoreTypeUInt );
    create->write( value );
    return create;
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::createWithValue( bool value )
{
    ofPtr< ofxGenericValueStore > create( new ofxGenericValueStore() );
    create->init( create, ofxGenericValueStoreTypeBool );
    create->write( value );
    return create;
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::createWithValue( string value )
{
    ofPtr< ofxGenericValueStore > create( new ofxGenericValueStore() );
    create->init( create, ofxGenericValueStoreTypeString );
    create->write( value );
    return create;
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::createWithValue( const char* value )
{
    string useValue;
    if ( value != NULL )
    {
        useValue = value;
    }
    return createWithValue( useValue );
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::createWithValue( ofPoint value )
{
    ofPtr< ofxGenericValueStore > create( new ofxGenericValueStore() );
    create->init( create, ofxGenericValueStoreTypeString );
    create->write( value );
    return create;
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::createWithValue( ofPtr< ofxGenericValueStore > value )
{
    if ( value )
    {
        ofPtr< ofxGenericValueStore > create( new ofxGenericValueStore() );
        create->init( create, value->getType() );
        create->write( value );
        return create;
    }
    return ofPtr< ofxGenericValueStore >();
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::createFrom( ofxGenericMIMEType mimeType, string content )
{
    ofPtr< ofxGenericValueStore > result;
    switch( mimeType )
    {
        case ofxGenericMIMETypeJSON:
            result = createFromJSON( content );
            break;
        case ofxGenericMIMETypeXML:
            result = createFromXML( content );
            break;
        case ofxGenericMIMETypeUnknown:
            break;
    }
    return result;
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::createFromJSON( string JSON )
{
    ofPtr< ofxGenericValueStore > create( new ofxGenericValueStore() );
    create->init( create, ofxGenericValueStoreTypeObject );
    ofxJSONElement parse( JSON );
    return createFrom( parse );
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::createFromXML( string xml )
{
    //TIXML_ENCODING_UTF8
    // tODO:
    TiXmlDocument doc;
    doc.Parse( xml.c_str() );
    return ofxGenericValueStore::createFromXML( doc );
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::createFromXML( TiXmlDocument& xml )
{
    return ofxGenericValueStore::createFrom( &xml );
}

ofxGenericValueStore::ofxGenericValueStore()
: _type( ofxGenericValueStoreTypeUninitialized ), _fileInDocuments( false )
{
}

void ofxGenericValueStore::init( ofPtrWeak< ofxGenericValueStore > setThis, Type type )
{
    _this = setThis;
    _type = type;
    if ( _type == ofxGenericValueStoreTypeString )
    {
        _values._stringValue = new string();
    } else if ( _type == ofxGenericValueStoreTypeArray )
    {
        _values._arrayValue = new ofxGenericValueStoreArray();
    } else if ( _type == ofxGenericValueStoreTypeObject )
    {
        _values._objectValue = new ofxGenericValueStoreObject();
    }
    
    _verify = false;
}

ofxGenericValueStore::~ofxGenericValueStore()
{
    if ( _type == ofxGenericValueStoreTypeString )
    {
        delete _values._stringValue;
    } else if ( _type == ofxGenericValueStoreTypeArray )
    {
        delete _values._arrayValue;
    } else if ( _type == ofxGenericValueStoreTypeObject )
    {
        delete _values._objectValue;
    }
}

ofxGenericValueStore::Type ofxGenericValueStore::getType() const
{
    return _type;
}

void ofxGenericValueStore::write( float value )
{
    if ( isFloat() )
    {
        _values._floatValue = value;
    } else if ( isInt() )
    {
        write( ( int )value );
    } else if ( isUInt() )
    {
        write( ( unsigned int )value );
    
    } else if ( isBool() )
    {
        write( ( bool )value );
    } else if ( isString() )
    {
        write( ofxGToString( value ) );
    }
    else
    {
        LogValueStoreTypeError( getType() );
    }
}

void ofxGenericValueStore::write( int value )
{
    if ( isFloat() )
    {
        write( ( float )value );
    } else if ( isInt() )
    {
        _values._intValue = value;
    } else if ( isUInt() )
    {
        // [JON 9/29/14] Json::Reader::decodeNumber automatically determines the integer type
        // based on the integer value read. Numbers < Json::Value::maxInt are converted to
        // signed integers.
        // 
        // We need to support this conversion from Int to UInt for server result ids less
        // than Json::Value::maxInt, which Json::Reader puts in an Int. We need to support
        // unsigned integer values and GameResultModel expects a UInt.
        if ( value >= 0 )
        {
            _values._uintValue = (unsigned int)value;
        } else
        {
            _values._uintValue = 0;
            LogValueStoreValueError( getType(), value );
        }
    } else if ( isBool() )
    {
        write( ( bool )value );
    } else if ( isString() )
    {
        write( ofxGToString( value ) );
    }
    else
    {
        LogValueStoreTypeError( getType() );
    }
}

void ofxGenericValueStore::write( unsigned int value )
{
    if ( isFloat() )
    {
        write( ( float )value );
    } else if ( isUInt() )
    {
        _values._uintValue = value;
    } else if ( isBool() )
    {
        write( ( bool )value );
    } else if ( isString() )
    {
        write( ofxGToString( value ) );
    }
    else
    {
        LogValueStoreTypeError( getType() );
    }
}

void ofxGenericValueStore::write( bool value )
{
    if ( isFloat() )
    {
        write( ( float )value );
    } else if ( isInt() )
    {
        write( ( int )value );
    } else if ( isUInt() )
    {
        write( ( unsigned int )value );
    } else if ( isBool() )
    {
        _values._boolValue = value;
    } else if ( isString() )
    {
        write( ofxGToString( value ) );
    }
    else
    {
        LogValueStoreTypeError( getType() );
    }
}

void ofxGenericValueStore::write( string value )
{
    if ( isFloat() )
    {
        write( ( float )atof( value.c_str() ) );
    } else if ( isInt() )
    {
        write( ( int )atoi( value.c_str() ) );
    } else if ( isUInt() )
    {
        write( ( unsigned int )strtoul( value.c_str(), NULL, 0) );
    } else if ( isBool() )
    {
        write( ofxGToBool( value ) );
    } else if ( isString() )
    {
        ( *_values._stringValue ) = value;
    }
    else
    {
        LogValueStoreTypeError( getType() );
    }
}

void ofxGenericValueStore::write( const char* value )
{
    write( string( value ) );
}

void ofxGenericValueStore::write( ofPoint value )
{
    write( ofxGToString( value ) );
}

void ofxGenericValueStore::write( ofPtr< ofxGenericValueStore > value )
{
    if ( value )
    {
        if ( value->isFloat() )
        {
            write( value->asFloat() );
        } else if ( value->isInt() )
        {
            write( value->asInt() );
        } else if ( value->isUInt() )
        {
            write( value->asUInt() );
        } else if ( value->isBool() )
        {
            write( value->asBool() );
        } else if ( value->isString() )
        {
            write( value->asString() );
        } else if ( value->isObject() )
        {
            for( ofxGenericValueStoreObjectIterator trav = value->objectBegin(); trav != value->objectEnd(); trav ++ )
            {
                string key = ( *trav ).first;
                ofPtr< ofxGenericValueStore > keyValue = ( *trav ).second;
                if ( isString() )
                {
                    string currentValue = asString();
                    if ( !currentValue.empty() )
                    {
                        currentValue += ",\n";
                    }
                    currentValue += "\"" + key + "\" : \"";
                    if ( keyValue )
                    {
                        keyValue->asString();
                    }
                    currentValue += "\"";
                    write( currentValue );
                } else if ( isObject() )
                {
                    write( key, createWithValue( keyValue ) );
                } else if ( isArray() )
                {
                    // TODO: save key somehow
                    write( length(), createWithValue( keyValue ) );
                }
            }
        } else if ( value->isArray() )
        {
            for( unsigned int travIndex = 0; travIndex < value->length(); travIndex++ )
            {
                ofPtr< ofxGenericValueStore > keyValue = value->read( travIndex );
                if ( isString() )
                {
                    string currentValue = asString();
                    if ( !currentValue.empty() )
                    {
                        currentValue += ",\n";
                    }
                    currentValue += ofxGToString( travIndex ) + " : \"";
                    if ( keyValue )
                    {
                        keyValue->asString();
                    }
                    currentValue += "\"";
                    write( currentValue );
                } else if ( isObject() )
                {
                    write( ofxGToString( travIndex ), createWithValue( keyValue ) );
                } else if ( isArray() )
                {
                    write( travIndex, createWithValue( keyValue ) );
                }
            }
        }
        else
        {
            LogValueStoreTypeError( getType() );
        }
    }
}

void ofxGenericValueStore::operator=( float value )
{
    write( value );
}

void ofxGenericValueStore::operator=( int value )
{
    write( value );
}

void ofxGenericValueStore::operator=( bool value )
{    
    write( value );
}

void ofxGenericValueStore::operator=( string value )
{    
    write( value );
}

void ofxGenericValueStore::operator=( const char* value )
{
    write( value );
}

void ofxGenericValueStore::operator++()
{
    if ( isFloat() )
    {
        write( asFloat() + 1 );
    } else if ( isInt() )
    {
        write( asInt() + 1 );
    } else if ( isUInt() )
    {
        write( asUInt() + 1 );
    } else if ( isBool() )
    {
        write( !asBool() );
    }
    else
    {
        LogValueStoreTypeError( getType() );
    }
}

void ofxGenericValueStore::operator--()
{
    if ( isFloat() )
    {
        write( asFloat() - 1 );
    } else if ( isInt() )
    {
        write( asInt() - 1 );
    } else if ( isUInt() )
    {
        unsigned int value = asUInt();
        if ( value > 0 )
        {
            write( value - 1 );
        }
        else
        {
            LogValueStoreValueError( getType(), value );
            write( 0 );
        }
    } else if ( isBool() )
    {
        write( !asBool() );
    }
    else
    {
        LogValueStoreTypeError( getType() );
    }
}

void ofxGenericValueStore::operator++( int )
{
    ++( *this );
}

void ofxGenericValueStore::operator--( int )
{
    --( *this );
}


bool ofxGenericValueStore::isFloat() const
{
    return getType() == ofxGenericValueStoreTypeFloat;
}

bool ofxGenericValueStore::isInt() const
{
    return getType() == ofxGenericValueStoreTypeInt;
}

bool ofxGenericValueStore::isUInt() const
{
    return getType() == ofxGenericValueStoreTypeUInt;
}


bool ofxGenericValueStore::isBool() const
{
    return getType() == ofxGenericValueStoreTypeBool;
}

bool ofxGenericValueStore::isString() const
{
    return getType() == ofxGenericValueStoreTypeString;
}

bool ofxGenericValueStore::isObject() const
{
    return getType() == ofxGenericValueStoreTypeObject;
}

bool ofxGenericValueStore::isArray() const
{
    return getType() == ofxGenericValueStoreTypeArray;
}

float ofxGenericValueStore::asFloat( float defaultValue ) const
{
    if ( isFloat() )
    {
        return _values._floatValue;
    } else if ( isInt() )
    {
        return ( float )asInt();
    } else if ( isUInt() )
    {
        return ( float )asUInt();
    } else if ( isString() )
    {
        return ( float )atof( asString().c_str() );
    }
    else
    {
        LogValueStoreTypeError( getType() );
    }
    return defaultValue;
}

int ofxGenericValueStore::asInt( int defaultValue ) const
{
    if ( isInt() )
    {
        return _values._intValue;
    } else if ( isFloat() )
    {
        return ( int )asFloat();
    } else if ( isString() )
    {
        return atoi( asString().c_str() );
    }
    else
    {
        LogValueStoreTypeError( getType() );
    }
    
    return defaultValue;
}

unsigned int ofxGenericValueStore::asUInt( unsigned int defaultValue ) const
{
    if ( isUInt() )
    {
        return _values._uintValue;
    } else if ( isInt() )
    {
        // [JON 9/29/14] Json::Reader::decodeNumber automatically determines the integer type
        // based on the integer value read. Numbers < Json::Value::maxInt are converted to
        // signed integers.
        //
        // We need to support this conversion from Int to UInt for server result ids less
        // than Json::Value::maxInt, which Json::Reader puts in an Int. We need to support
        // unsigned integer values and GameResultModel expects a UInt.
        int value = asInt();
        if ( value >= 0 )
        {
            return (unsigned int) value;
        } else
        {
            LogValueStoreValueError( getType(), value );
            return 0;
        }
        
        return ( unsigned int )asInt();
    } else if ( isFloat() )
    {
        return ( int )asFloat();
    } else if ( isString() )
    {
        return atoi( asString().c_str() );
    }
    else
    {
        LogValueStoreTypeError( getType() );
    }
    return defaultValue;
}

bool ofxGenericValueStore::asBool( bool defaultValue ) const
{
    if ( isBool() )
    {
        return _values._boolValue;
    } else if ( isInt() )
    {
        return ( bool )asInt();
    } else if ( isUInt() )
    {
        return ( bool )asUInt();
    } else if ( isFloat() )
    {
        return ( bool )asFloat();
    } else if ( isString() )
    {
        return ofxGToBool( asString() );
    }
    else
    {
        LogValueStoreTypeError( getType() );
    }
    
    return defaultValue;
}

string ofxGenericValueStore::asString( string defaultValue ) const
{
    if ( isFloat() )
    {
        return ofxGToString( asFloat() );
    } else if ( isInt() )
    {
        return ofxGToString( asInt() );
    } else if ( isUInt() )
    {
        return ofxGToString( asUInt() );
    } else if ( isBool() )
    {
        return ofxGToString( asBool() );
    } else if ( isString() )
    {
        return *_values._stringValue;
    } else if ( isObject() )
    {
        // TODO: parse?
        return "object";
    } else if ( isArray() )
    {
        // TODO: parse?
        return "array";
    }
    else
    {
        LogValueStoreTypeError( getType() );
    }
    
    return defaultValue;
}

string ofxGenericValueStore::asString( ofxGenericMIMEType mimeType ) const
{
    string result;
    switch( mimeType )
    {
        case ofxGenericMIMETypeJSON:
            result = toJSONString();
            break;
            //        case ofxGenericMIMETypeXML:
            //            result = toXMLString();
            //            break;
        default:
            result = "";
    }
    return result;
}

std::map<string, string> ofxGenericValueStore::asStringToStringMap() const
{
    std::map<string, string> asMap;
    vector<string> keys = getObjectKeys();
    for (vector<string>::iterator i = keys.begin(); i != keys.end(); ++i)
    {
        asMap[*i] = read(*i, string(""));
    }
    
    return asMap;
}


ofxGenericValueStoreObject* ofxGenericValueStore::asObject() const
{
    if ( isObject() )
    {
        return _values._objectValue;
    }
    return NULL;
}

ofxGenericValueStoreArray* ofxGenericValueStore::asArray() const
{
    if ( isArray() )
    {
        return _values._arrayValue;
    }
    return NULL;
}


bool ofxGenericValueStore::operator==( ofPtr< ofxGenericValueStore > compare ) const
{
    // TODO: real object and array comparision?
    if ( !isObject() && !isArray() && compare ) // same type?
    {
        return asString() == compare->asString();
    }
    return false;
}

bool ofxGenericValueStore::exists( string key ) const
{
    if ( objectKeyExists( key ) )
    {
#if DEBUG
        ofxGenericValueStoreObjectIterator find = asObject()->find( key );
        if ( find != asObject()->end() )
        {
#endif
            return true;
#if DEBUG
        }
        ofxGLogFatalError( "ofxGenericValueStore of object type has found key " + key + " in key list but not in object map" );
#endif
    }
    
#if DEBUG
    ofxGenericValueStoreObjectIterator find = asObject()->find( key );
    if ( find != asObject()->end() )
    {
        ofxGLogFatalError( "ofxGenericValueStore of object type has found key " + key + " in object map but not in key list" );
        return true;
    }
#endif
    return false;
}

const std::vector< string >& ofxGenericValueStore::getObjectKeys() const
{
    return _objectKeys;
}

void ofxGenericValueStore::addObjectKey( string key )
{
    if ( isObject() )
    {
        if ( !objectKeyExists( key ) )
        {
            _objectKeys.push_back( key );
        }
    }
}

bool ofxGenericValueStore::objectKeyExists( string key ) const
{
    if ( isObject() )
    {
        std::vector< string >::const_iterator findKey = find( _objectKeys.begin(), _objectKeys.end(), key );
        return findKey != _objectKeys.end();
    }
    return false;
}

void ofxGenericValueStore::dropObjectKey( string key )
{
    if ( isObject() )
    {
        std::vector< string >::iterator eraseKey = find( _objectKeys.begin(), _objectKeys.end(), key );
        if ( eraseKey != _objectKeys.end() )
        {
            _objectKeys.erase( eraseKey );
        }
    }
}

// TODO: only create new if incorrect type?
void ofxGenericValueStore::write(string key, float value )
{
    if ( asObject() )
    {
        ( *asObject() )[ key ] = createWithValue( value );
        addObjectKey( key );
    }
}

void ofxGenericValueStore::write( string key, int value )
{
    if ( asObject() )
    {
        ( *asObject() )[ key ] = createWithValue( value );
        addObjectKey( key );
    }
}

void ofxGenericValueStore::write( string key, unsigned int value )
{
    if ( asObject() )
    {
        ( *asObject() )[ key ] = createWithValue( value );
        addObjectKey( key );
    }
}

void ofxGenericValueStore::write( string key, bool value )
{
    if ( asObject() )
    {
        ( *asObject() )[ key ] = createWithValue( value );
        addObjectKey( key );
    }
}

void ofxGenericValueStore::write( string key, string value, bool onlyIfFilled )
{
    if ( asObject() )
    {
        if ( !onlyIfFilled || value.size() > 0 )
        {
            ( *asObject() )[ key ] = createWithValue( value );
            addObjectKey( key );
        }
    }
}

void ofxGenericValueStore::write( string key, const char* value, bool onlyIfFilled )
{
    write( key, string( value ), onlyIfFilled );
}

void ofxGenericValueStore::write( string key, ofPoint value )
{
    if ( asObject() )
    {
        ( *asObject() )[ key ] = createWithValue( value );
        addObjectKey( key );
    }
}

void ofxGenericValueStore::write(string key, ofPtr< ofxGenericValueStore > value )
{
    if ( asObject() )
    {
        ( *asObject() )[ key ] = value;
        addObjectKey( key );
    }
}

float ofxGenericValueStore::read( string key, float defaultValue ) const
{
    ofPtr< ofxGenericValueStore > value = read( key );
    if ( value )
    {
        return value->asFloat();
    }
    return defaultValue;
}

int ofxGenericValueStore::read( string key, int defaultValue ) const
{
    ofPtr< ofxGenericValueStore > value = read( key );
    if ( value )
    {
        return value->asInt();
    }
    return defaultValue;
}

unsigned int ofxGenericValueStore::read( string key, unsigned int defaultValue ) const
{
    ofPtr< ofxGenericValueStore > value = read( key );
    if ( value )
    {
        return value->asUInt();
    }
    return defaultValue;
}

bool ofxGenericValueStore::read( string key, bool defaultValue ) const
{
    ofPtr< ofxGenericValueStore > value = read( key );
    if ( value )
    {
        return value->asBool();
    }
    return defaultValue;
}

string ofxGenericValueStore::read( string key, string defaultValue ) const
{
    ofPtr< ofxGenericValueStore > value = read( key );
    if ( value )
    {
        return value->asString();
    }
    return defaultValue;
}

string ofxGenericValueStore::read( string key, const char* defaultValue ) const
{
    ofPtr< ofxGenericValueStore > value = read( key );
    if ( value )
    {
        return value->asString();
    }
    return string( defaultValue );
}

ofPoint ofxGenericValueStore::read( string key, ofPoint defaultValue ) const
{
    ofPtr< ofxGenericValueStore > value = read( key );
    if ( value )
    {
        return ofxGToPoint( value->asString() );
    }
    
    return defaultValue;
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::read( string key ) const
{
    return read( key, ofPtr< ofxGenericValueStore >() );
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::read( string key, ofPtr< ofxGenericValueStore > defaultValue ) const
{
    if ( asObject() && exists( key ) )
    {
        return ( *asObject() )[ key ];
    }
    return defaultValue;
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::operator[]( string key ) const
{
    return read( key );
}

void ofxGenericValueStore::drop( string key )
{
    if ( isObject() )
    {
        ( *_values._objectValue ).erase( key );
        dropObjectKey( key );
    }
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::readOrCreate( string key, Type type )
{
    bool didCreate;
    return readOrCreate( key, type, didCreate );
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::readOrCreate( string key, Type type, bool& didCreate )
{
    didCreate = false;
    if ( isObject() )
    {
        ofPtr< ofxGenericValueStore > value = read( key );
        if ( !value )
        {
            value = ofxGenericValueStore::create( type );
            write( key, value );
            didCreate = true;
        }
        return value;
    }
    return ofPtr< ofxGenericValueStore >();
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::readAndCreateIfNot( string key, ofxGenericValueStoreCreateIfNot createIfNotFunction )
{
    ofPtr< ofxGenericValueStore > result = read( key );
    ofPtr< ofxGenericValueStore > createIfNot = createIfNotFunction( result );
    if( createIfNot != result )
    {
        write( key, createIfNot );
        result = createIfNot;
    }
    return result;
}

#pragma Array

void ofxGenericValueStore::ensureIndexAvailable( unsigned int index )
{
    if ( asArray() )
    {
        if ( index >= length() )
        {
            ( *asArray() ).resize( index + 1 );
        }
    }
}

void ofxGenericValueStore::write( unsigned int index, float value )
{
    if ( asArray() )
    {
        ensureIndexAvailable( index );
        ( *asArray() )[ index ] = createWithValue( value );
    }
}

void ofxGenericValueStore::write( unsigned int index, int value )
{
    if ( asArray() )
    {
        ensureIndexAvailable( index );
        ( *asArray() )[ index ] = createWithValue( value );
    }
}

void ofxGenericValueStore::write( unsigned int index, bool value )
{
    if ( asArray() )
    {
        ensureIndexAvailable( index );
        ( *asArray() )[ index ] = createWithValue( value );
    }
}

void ofxGenericValueStore::write( unsigned int index, string value, bool onlyIfFilled  )
{
    if ( asArray() )
    {
        if ( !onlyIfFilled || value.size() )
        {
            ensureIndexAvailable( index );
            ( *asArray() )[ index ] = createWithValue( value );
        }
    }
}

void ofxGenericValueStore::write( unsigned int index, const char* value, bool onlyIfFilled  )
{
    write( index, string( value ), onlyIfFilled );
}

void ofxGenericValueStore::write( unsigned int index, ofPoint value )
{
    if ( asArray() )
    {
        ensureIndexAvailable( index );
        ( *asArray() )[ index ] = createWithValue( value );
    }
}

void ofxGenericValueStore::write( unsigned int index, ofPtr< ofxGenericValueStore > value )
{
    if ( asArray() )
    {
        ensureIndexAvailable( index );
        ( *asArray() )[ index ] = value;
    }
}

void ofxGenericValueStore::writeToFront(ofPtr<ofxGenericValueStore> value)
{
    if (asArray()) {
        insert(arrayBegin(), value);
    }
}

float ofxGenericValueStore::read( unsigned int index, float defaultValue ) const
{
    ofPtr< ofxGenericValueStore > value = read( index );
    if ( value )
    {
        return value->asFloat();
    }
    return defaultValue;
}

int ofxGenericValueStore::read( unsigned int index, int defaultValue ) const
{
    ofPtr< ofxGenericValueStore > value = read( index );
    if ( value )
    {
        return value->asInt();
    }
    return defaultValue;
}

bool ofxGenericValueStore::read( unsigned int index, bool defaultValue ) const
{
    ofPtr< ofxGenericValueStore > value = read( index );
    if ( value )
    {
        return value->asBool();
    }
    return defaultValue;
}

string ofxGenericValueStore::read( unsigned int index, string defaultValue ) const
{
    ofPtr< ofxGenericValueStore > value = read( index );
    if ( value )
    {
        return value->asString();
    }
    return defaultValue;
}

string ofxGenericValueStore::read( unsigned int index, const char* defaultValue ) const
{
    ofPtr< ofxGenericValueStore > value = read( index );
    if ( value )
    {
        return value->asString();
    }
    return string( defaultValue ); 
}

ofPoint ofxGenericValueStore::read( unsigned int index, ofPoint defaultValue ) const
{
    ofPtr< ofxGenericValueStore > value = read( index );
    if ( value )
    {
        return ofxGToPoint( value->asString() );
    }
    return defaultValue;
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::read( unsigned int index ) const
{
    return read( index, ofPtr< ofxGenericValueStore >() );
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::read( unsigned int index, ofPtr< ofxGenericValueStore > defaultValue ) const
{
    if ( asArray() && length() > index )
    {
        return ( *asArray() )[ index ];
    }
    return defaultValue;
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::operator[]( unsigned int index ) const
{
    return read( index );
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::readOrCreate( unsigned int index, Type type )
{
    bool didCreate;
    return readOrCreate( index, type, didCreate );
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::readOrCreate( unsigned int index, Type type, bool& didCreate )
{
    didCreate = false;
    if ( isArray() )
    {
        ofPtr< ofxGenericValueStore > value = read( index );
        if ( !value )
        {
            value = ofxGenericValueStore::create( type );
            write( index, value );
            didCreate = true;
        }
        return value;
    }
    return ofPtr< ofxGenericValueStore >();
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::readAndCreateIfNot( unsigned int index, ofxGenericValueStoreCreateIfNot createIfNotFunction )
{
    ofPtr< ofxGenericValueStore > result = read( index );
    ofPtr< ofxGenericValueStore > createIfNot = createIfNotFunction( result );
    if( createIfNot != result )
    {
        write( index, createIfNot );
        result = createIfNot;
    }
    return result;
}

unsigned int ofxGenericValueStore::length() const
{
    if ( asArray() )
    {
        return ( *asArray() ).size();
    } else if ( isObject() )
    {
        return ( *asObject() ).size();
    }
    return 0;
}

ofxGenericValueStoreObjectIterator ofxGenericValueStore::objectBegin()
{
    if ( asObject() )
    {
        return asObject()->begin();
    }
    return ofxGenericValueStoreObjectIterator();
}

ofxGenericValueStoreObjectIterator ofxGenericValueStore::objectEnd()
{
    if ( asObject() )
    {
        return asObject()->end();
    }
    return ofxGenericValueStoreObjectIterator();
}

ofxGenericValueStoreArrayIterator ofxGenericValueStore::arrayBegin()
{
    if ( asArray() )
    {
        return asArray()->begin();
    }
    return ofxGenericValueStoreArrayIterator();
}

ofxGenericValueStoreArrayIterator ofxGenericValueStore::arrayEnd()
{
    if ( asArray() )
    {
        return asArray()->end();
    }
    return ofxGenericValueStoreArrayIterator();
}

ofxGenericValueStoreObjectConstIterator ofxGenericValueStore::objectBegin() const
{
    if ( asObject() )
    {
        return asObject()->begin();
    }
    return ofxGenericValueStoreObjectConstIterator();
}

ofxGenericValueStoreObjectConstIterator ofxGenericValueStore::objectEnd() const
{
    if ( asObject() )
    {
        return asObject()->end();
    }
    return ofxGenericValueStoreObjectConstIterator();
}

ofxGenericValueStoreArrayConstIterator ofxGenericValueStore::arrayBegin() const
{
    if ( asArray() )
    {
        return asArray()->begin();
    }
    return ofxGenericValueStoreArrayConstIterator();
}

ofxGenericValueStoreArrayConstIterator ofxGenericValueStore::arrayEnd() const
{
    if ( asArray() )
    {
        return asArray()->end();
    }
    return ofxGenericValueStoreArrayConstIterator();
}

ofxGenericValueStoreArrayIterator ofxGenericValueStore::arrayFind( ofPtr< ofxGenericValueStore > value )
{
    if ( asArray() )
    {
        for ( ofxGenericValueStoreArrayIterator find = asArray()->begin(); find != asArray()->end(); find ++ )
        {
            if ( value == *find )
            {
                return find;
            }
        }
    }
    return arrayEnd();
}

void ofxGenericValueStore::remove( ofxGenericValueStoreObjectIterator location )
{
    if ( asObject() )
    {
        asObject()->erase( location );
    }
}

void ofxGenericValueStore::remove( ofxGenericValueStoreArrayIterator location )
{
    if ( asArray() )
    {
        asArray()->erase( location );
    }
}


void ofxGenericValueStore::insert(ofxGenericValueStoreArrayIterator location, ofPtr<ofxGenericValueStore> value)
{
    if (asArray()) {
        asArray()->insert(arrayBegin(), 1, value);
    }
}


void ofxGenericValueStore::drop( int index )
{
    if ( asArray() )
    {
        ( *asArray() )[ index ] = ofPtr< ofxGenericValueStore >();
    }
}

void ofxGenericValueStore::setFileName( string fileName, bool fileInDocuments )
{
    _fileName = fileName;
    _fileInDocuments = fileInDocuments;
}

string ofxGenericValueStore::getFileName()
{
    return _fileName;
}

bool ofxGenericValueStore::readFromDisk()
{
    if ( !_fileName.empty() )
    {
        ofxJSONElement read;
        if ( read.openLocal( _fileName, _fileInDocuments ) )
        {
            purge();
            convertFrom( read );
            
            if( _verify && !verifyContentsFromDisk() )
            {
                // TODO: this filename can contain the users ID, we should log only the actual filename.
                ofxGLogError("ofxGenericValueStore::readFromDisk file " + _fileName + " failed on verification!");
                purge();
                return false;
            }
            
            return true;
        }
    }
    return false;
}

void ofxGenericValueStore::convertFrom( Json::Value& convert )
{
    if ( convert.type() == Json::realValue && isFloat() )
    {
        write( ( float )convert.asDouble() );
    } else if ( convert.type() == Json::intValue && isInt() )
    {
        write( convert.asInt() );
    } else if ( convert.type() == Json::uintValue && isUInt() )
    {
        write( convert.asUInt() );
    } else if ( convert.type() == Json::booleanValue && isBool() )
    {
        write( convert.asBool() );
    } else if ( convert.type() == Json::stringValue && isString() )
    {
        write( convert.asString() );
    } else if ( convert.type() == Json::objectValue && asObject() )
    {
        asObject()->clear();
        for( Json::Value::iterator travMembers = convert.begin(); travMembers != convert.end(); travMembers ++ )
        {
            string key = travMembers.key().asString();
            write( key, createFrom( *travMembers ) );
        }    
    } else if ( convert.type() == Json::arrayValue && asArray() )
    {
        asArray()->resize( convert.size() );
        for( unsigned int index = 0; index < convert.size(); index ++ )
        {
            write( index, ofxGenericValueStore::createFrom( convert[ index ] ) ); 
        }
    }
    else
    {
        LogValueStoreTypeError( getType() );
    }
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::createFrom( Json::Value& convert )
{
    switch( convert.type() )
    {
        case Json::nullValue:
            return ofPtr< ofxGenericValueStore >();
        case Json::intValue:
            return createWithValue( convert.asInt() );
        case Json::uintValue:
            return createWithValue( convert.asUInt() );
        case Json::realValue:
            return createWithValue( ( float )convert.asDouble() );
        case Json::stringValue:
            return createWithValue( convert.asString() );
        case Json::booleanValue:
            return createWithValue( convert.asBool() );
        case Json::objectValue:
        {
            ofPtr< ofxGenericValueStore > object = ofxGenericValueStore::create( false );
            for( Json::Value::iterator travMembers = convert.begin(); travMembers != convert.end(); travMembers ++ )
            {
                string key = travMembers.key().asString();
                object->write( key, ofxGenericValueStore::createFrom( *travMembers ) );            
            }
            return object;
        }
            break;
        case Json::arrayValue:
        {
            ofPtr< ofxGenericValueStore > array = ofxGenericValueStore::create( true );
            for( unsigned int index = 0; index < convert.size(); index ++ )
            {
                array->write( index, ofxGenericValueStore::createFrom( convert[ index ] ) );
            }    
            return array;
        }
        default:
            LogValueStoreTypeError( convert.type() );
            break;
    }
    return ofPtr< ofxGenericValueStore >();
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::createFrom( TiXmlNode* xml )
{
    if ( xml )
    {
        switch( xml->Type() )
        {
            case TiXmlNode::DOCUMENT:
            case TiXmlNode::ELEMENT:
            {
                unsigned int children = 0;
                
                TiXmlElement* element = xml->ToElement();
                if ( element && element->FirstAttribute() != NULL )
                {
                    children += 2;
                }
                if ( xml->FirstChild() != NULL )
                {
                    if ( xml->FirstChild() == xml->LastChild() )
                    {
                        children += 1;
                    } else
                    {
                        children += 2;
                    }
                }
                if ( children == 0 )
                {
                    return ofPtr< ofxGenericValueStore >();
                } else if ( children == 1 )
                {
                    return createFrom( xml->FirstChild() );
                } else
                {
                    ofPtr< ofxGenericValueStore > node = ofxGenericValueStore::create( false );

                    if ( element )
                    {
                        for( TiXmlAttribute* travAttributes = element->FirstAttribute(); travAttributes != NULL; travAttributes = travAttributes->Next() )
                        {
                            node->write( travAttributes->Name(), travAttributes->Value() );
                        }
                    }
                    
                    for( TiXmlNode* travNodes = xml->FirstChild(); travNodes != NULL; travNodes = travNodes->NextSibling() )
                    {
                        string value = travNodes->Value();
                        ofPtr< ofxGenericValueStore > originally = node->read( value );
                        if ( originally )
                        {
                            if ( !originally->isArray() )
                            {
                                ofPtr< ofxGenericValueStore > array = ofxGenericValueStore::create( true );
                                array->write( array->length(), originally );
                                node->write( value, array );
                                originally = array;
                            }
                            originally->write( originally->length(), createFrom( travNodes ) );
                        } else
                        {
                            node->write( value, createFrom( travNodes ) );
                        }
                    }
                    
                    return node;
                }
            }
                break;
            case TiXmlNode::COMMENT:
                break;
            case TiXmlNode::UNKNOWN:
                break;
            case TiXmlNode::TEXT:
            {
                TiXmlText* text = xml->ToText();
                if ( text )
                {
                    return createWithValue( text->Value() );
                }
            }
                break;
            case TiXmlNode::DECLARATION:
                break;
            case TiXmlNode::TYPECOUNT:
                break;
        }
    }
    return ofPtr< ofxGenericValueStore >();
}


//writes the cache to disk
bool ofxGenericValueStore::writeToDisk()
{
    if ( _fileName.length() > 0 )
    {
        if ( _verify )
        {
            signContentsForSavingToDisk();
        }
        
        ofxGmkdir( ofxGGetPathFromFileName( _fileName ), _fileInDocuments );
        Json::Value* root = convertToJSON();
        ofxJSONElement write( *root );
        delete root;
        return write.save( _fileName, true, _fileInDocuments );
    }
    return false;
}

string ofxGenericValueStore::toJSONString() const
{
    Json::Value* asJSON = convertToJSON();
    if ( asJSON )
    {
        const string& result = ofxJSONElement( *asJSON ).getRawString();
        delete asJSON;
        return result;
    }
    return string();
}

Json::Value* ofxGenericValueStore::convertToJSON() const
{
    Json::Value* node = NULL;
    if ( isFloat() )
    {
        node = new Json::Value( ( double )asFloat() );
    }
    else if ( isInt() )
    {
        node = new Json::Value( asInt() );
    }
    else if ( isUInt() )
    {
        node = new Json::Value( asUInt() );
    }
    else if ( isBool() )
    {
        node = new Json::Value( asBool() );
    }
    else if ( isString() )
    {
        node = new Json::Value( asString() );
    }
    else if ( isObject() )
    {
        node = new Json::Value( Json::objectValue );
        for( ofxGenericValueStoreObjectConstIterator travMembers = objectBegin(); travMembers !=objectEnd(); travMembers ++ )
        {
            string key = ( *travMembers ).first;
            if ( ( *travMembers ).second )
            {
                Json::Value* member = ( *travMembers ).second->convertToJSON();
                if ( member )
                {
                    ( *node )[ key ] = *member;
                    delete member;
                }
            } else 
            {
                ( *node )[ key ] = Json::nullValue;
            }
        }
    } else if ( isArray() )
    {
        node = new Json::Value( Json::arrayValue );
        int indexCount = 0;
        for( ofxGenericValueStoreArrayConstIterator travIndices = arrayBegin(); travIndices !=arrayEnd(); travIndices ++ )
        {
            if ( *travIndices )
            {
                Json::Value* index = ( *travIndices )->convertToJSON();
                if ( index )
                {
                    ( *node )[ indexCount ] = *index;
                    delete index;
                    indexCount ++;
                } else 
                {
                    // TODO: proper verbose report
                }
            } else 
            {
                ofxGLogError( "In writing to JSON a node at index " + ofxGToString( indexCount ) + " is empty" );
            }
        }
    }
    else
    {
        LogValueStoreTypeError( getType() );
    }
    return node;
}

string ofxGenericValueStore::createCSVHeaderString( std::vector< string > keys, string separater )
{
    string result;
    
    for( std::vector< string >::const_iterator key = keys.begin(); key != keys.end(); key++ )
    {
        if ( !result.empty() )
        {
            result += separater;
        }
        result += *key;
    }
    
    return result;
}

string ofxGenericValueStore::toCSVRowString( std::vector< string > keys, string separater ) const
{
    string result;

    if ( isObject() )
    {
        for( std::vector< string >::const_iterator key = keys.begin(); key != keys.end(); key++ )
        {
            if ( !result.empty() )
            {
                result += separater;
            }
            result += read( *key, "" );
        }
    }

    return result;
}

void ofxGenericValueStore::purge()
{
    if ( asObject() )
    {
        ( *asObject() ).clear();
    } else if ( asArray() )
    {
        ( *asArray() ).clear();
    }
}

#if TARGET_OS_IPHONE
NSNumber* ofxGenericValueStore::toNSNumber() const
{
    NSNumber* result = nil;

    switch ( getType() )
    {
        case ofxGenericValueStoreTypeArray:
            break;
        case ofxGenericValueStoreTypeBool:
            result = [ NSNumber numberWithBool:asBool() ];
            break;
        case ofxGenericValueStoreTypeFloat:
            result = [ NSNumber numberWithFloat:asFloat() ];
            break;
        case ofxGenericValueStoreTypeInt:
            result = [ NSNumber numberWithInt:asInt() ];
            break;
        case ofxGenericValueStoreTypeUInt:
            result = [ NSNumber numberWithUnsignedInt:asUInt() ];
            break;
        case ofxGenericValueStoreTypeObject:
            break;
        case ofxGenericValueStoreTypeString:
            result = [ NSNumber numberWithFloat: ofToFloat( asString() ) ];
            break;
        case ofxGenericValueStoreTypeUninitialized:
            break;
        default:
            LogValueStoreTypeError( getType() );
    }
    
    return result;
}
#endif

void ofxGenericValueStore::setVerify( bool verify, string securityKey )
{
    _verify = verify;
    _securityKey = securityKey;
}

bool ofxGenericValueStore::getVerify()
{
    return _verify;
}


string ofxGenericValueStore::generateHash()
{
    // TODO: Enable this part to add time stamps to the hash
    /*
    ofPtr< ofxGenericDate > date = ofxGenericDate::create();
    double dateStamp = (double) date->getDate();
    dateStamp = round( dateStamp / ( 60 * 1000 ) );
    
    string body = toJSONString() + ofxGToString( dateStamp );
    */
    string body = toJSONString();
        
    return ofxGenericHMAC::createHMAC( _securityKey, body );
}

bool ofxGenericValueStore::verifyContentsFromDisk()
{
    if( _verify )
    {
        if( exists( HmacStoreKey ) )
        {
            string savedHash = read( HmacStoreKey, "" );
            
            drop( HmacStoreKey );
            
            string expectedHash = generateHash();
            
            if( savedHash.compare( expectedHash ) == 0 )
            {
                return true;
            }
            else
            {
                ofxGLogWarning("ofxGenericValueStore::verifyContentsFromDisk file " + _fileName + " have invalid hash!");
                return false;
            }
        }
        else
        {
            ofxGLogWarning("ofxGenericValueStore::verifyContentsFromDisk file " + _fileName + " HmacStoreKey missing!");
            return false;
        }
    }
    
    return true;
}

void ofxGenericValueStore::signContentsForSavingToDisk()
{
    if( _verify )
    {        
        // Remove previous hash
        if( exists( HmacStoreKey ) )
        {
            drop( HmacStoreKey );
        }
        
        // Generate new hash and add it to the final json
        string newHash = generateHash();
        write( HmacStoreKey, createWithValue( newHash ) );
    }
}

ofPtr< ofxGenericValueStore > operator << ( ofPtr< ofxGenericValueStore > store, const char* key )
{
    ofPtr< ofxGenericValueStore > childStore;
    if ( store )
    {
        childStore = store->read( key );
    }
    return childStore;
}

ofPtr< ofxGenericValueStore > operator << ( ofPtr< ofxGenericValueStore > store, const std::string& key )
{
    return store << key.c_str();
}

ofPtr< ofxGenericValueStore > operator << ( ofPtr< ofxGenericValueStore > store, unsigned int index )
{
    ofPtr< ofxGenericValueStore > childStore;
    if ( store )
    {
        childStore = store->read( index );
    }
    return childStore;
}
