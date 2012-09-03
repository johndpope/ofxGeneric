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

#include "ofxJSONElement.h"
#include <tinyxml.h>

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
}

void ofxGenericValueStore::init( ofPtrWeak< ofxGenericValueStore > setThis, ofPtr< ofxGenericValueStore > from )
{
    Type type;
    if ( from )
    {
        type = from->getType();
    } else
    {
        type = ofxGenericValueStoreTypeObject;
    }
    ofxGenericValueStore::init( setThis, type );
    
    write( from );
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
    } else if ( isBool() )
    {
        write( ( bool )value );
    } else if ( isString() )
    {
        write( ofxGToString( value ) );
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
    } else if ( isBool() )
    {
        write( ( bool )value );
    } else if ( isString() )
    {
        write( ofxGToString( value ) );
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
    } else if ( isBool() )
    {
        _values._boolValue = value;
    } else if ( isString() )
    {
        write( ofxGToString( value ) );
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
    } else if ( isBool() )
    {
        write( ofxGToBool( value ) );
    } else if ( isString() )
    {
        ( *_values._stringValue ) = value;
    }
}

void ofxGenericValueStore::write( const char* value )
{
    write( string( value ) );
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
    } else if ( isBool() )
    {
        write( !asBool() );
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
    } else if ( isBool() )
    {
        write( !asBool() );
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
    } else if ( isString() )
    {
        return ( float )atof( asString().c_str() );
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
    } else if ( isFloat() )
    {
        return ( bool )asFloat();
    } else if ( isString() )
    {
        return ofxGToBool( asString() );
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

ofPtr< ofxGenericValueStore > ofxGenericValueStore::read( string key ) const
{
    if ( asObject() && exists( key ) )
    {
        return ( *asObject() )[ key ];
    }
    return ofPtr< ofxGenericValueStore >();
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

void ofxGenericValueStore::write( unsigned int index, ofPtr< ofxGenericValueStore > value )
{
    if ( asArray() )
    {
        ensureIndexAvailable( index );
        ( *asArray() )[ index ] = value;
    }
}

float ofxGenericValueStore::read( unsigned int index, float defaultValue ) const
{
    ofPtr< ofxGenericValueStore > value = read( index );
    if ( value )
    {
        return value->asInt();
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

ofPtr< ofxGenericValueStore > ofxGenericValueStore::read( unsigned int index ) const
{
    if ( asArray() && length() > index )
    {
        return ( *asArray() )[ index ];
    }
    return ofPtr< ofxGenericValueStore >();
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::operator[]( unsigned int index ) const
{
    return read( index );
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

bool ofxGenericValueStore::readFromDisk()
{
    if ( _fileName.length() > 0 )
    {
        ofxJSONElement read;
        if ( read.openLocal( _fileName, _fileInDocuments ) )
        {
            purge();
            convertFrom( read );
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
            return createWithValue( convert.asInt() ); // TODO: fix range or support uint
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
        ofxGmkdir( ofxGGetPathFromFileName( _fileName ), _fileInDocuments );
        Json::Value* root = convertTo();
        ofxJSONElement write( *root );
        delete root;
        return write.save( _fileName, true, _fileInDocuments );
    }
    return false;
}

string ofxGenericValueStore::toJSONString() const
{
    Json::Value* asJSON = convertTo();
    if ( asJSON )
    {
        return ofxJSONElement( *asJSON ).getRawString();
    }
    return string();
}

Json::Value* ofxGenericValueStore::convertTo() const
{
    Json::Value* node = NULL;
    if ( isFloat() )
    {
        node = new Json::Value( ( double )asFloat() );
    } else if ( isInt() )
    {
        node = new Json::Value( asInt() );
    } else if ( isBool() )
    {
        node = new Json::Value( asBool() );
    } else if ( isString() )
    {
        node = new Json::Value( asString() );
    } else if ( isObject() )
    {
        node = new Json::Value( Json::objectValue );
        for( ofxGenericValueStoreObjectConstIterator travMembers = objectBegin(); travMembers !=objectEnd(); travMembers ++ )
        {
            if ( ( *travMembers ).second )
            {
                string key = ( *travMembers ).first;
                Json::Value* member = ( *travMembers ).second->convertTo();
                if ( member )
                {
                    ( *node )[ key ] = *member;
                    delete member;
                }
            } else 
            {
                ofxGLogError( "In writing to JSON a node " + ( *travMembers ).first + " is empty" );
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
                Json::Value* index = ( *travIndices )->convertTo();
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
    return node;
}

//empties the entire cache. a sync call must still be made to put this change onto the disk
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