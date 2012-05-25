//
//  ofxGenericCache.cpp
//  ofxGeneric
//
//  Handles saving and loading cache data. Cache is stored in the documents directory
//  and exists until it is manually purged. A purge would usually happen when the cache
//  values become invalid due to more up to date information coming from the server.
//
//  Created by Eli Delventhal on 4/10/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericCache.h"
#include "ofxGenericUtility.h"

#include "ofxJSONElement.h"

ofPtr< ofxGenericCache > ofxGenericCache::create( bool asArray )
{
    ofPtr< ofxGenericCache > create( new ofxGenericCache() );
    Type type;
    if ( asArray )
    {
        type = ofxGenericCacheTypeArray;
    } else 
    {
        type = ofxGenericCacheTypeObject;
    }
    create->init( create, type );
    return create;
}

ofPtr< ofxGenericCache > ofxGenericCache::createWithValue( float value )
{
    ofPtr< ofxGenericCache > create( new ofxGenericCache() );
    create->init( create, ofxGenericCacheTypeFloat );
    create->_floatValue = value;
    return create;
}

ofPtr< ofxGenericCache > ofxGenericCache::createWithValue( int value )
{
    ofPtr< ofxGenericCache > create( new ofxGenericCache() );
    create->init( create, ofxGenericCacheTypeInt );
    create->_intValue = value;
    return create;
}

ofPtr< ofxGenericCache > ofxGenericCache::createWithValue( bool value )
{
    ofPtr< ofxGenericCache > create( new ofxGenericCache() );
    create->init( create, ofxGenericCacheTypeBool );
    create->_boolValue = value;
    return create;
}

ofPtr< ofxGenericCache > ofxGenericCache::createWithValue( string value )
{
    ofPtr< ofxGenericCache > create( new ofxGenericCache() );
    create->init( create, ofxGenericCacheTypeString );
    *( create->_stringValue ) = value;
    return create;
}

ofxGenericCache::ofxGenericCache()
: _type( ofxGenericCacheTypeUninitialized ), _fileInDocuments( false )
{
}

void ofxGenericCache::init( ofPtrWeak< ofxGenericCache > setThis, Type type )
{
    _this = setThis;
    _type = type;
    if ( _type == ofxGenericCacheTypeString )
    {
        _stringValue = new string();
    } else if ( _type == ofxGenericCacheTypeArray )
    {
        _arrayValue = new std::vector< ofPtr< ofxGenericCache > >();
    } else if ( _type == ofxGenericCacheTypeObject )
    {
        _objectValue = new std::map< string, ofPtr< ofxGenericCache > >();
    }
}

ofxGenericCache::~ofxGenericCache()
{
    if ( _type == ofxGenericCacheTypeString )
    {
        delete _stringValue;
    } else if ( _type == ofxGenericCacheTypeArray )
    {
        delete _arrayValue;
    } else if ( _type == ofxGenericCacheTypeObject )
    {
        delete _objectValue;
    }
}

ofxGenericCache::Type ofxGenericCache::getType() const
{
    return _type;
}

bool ofxGenericCache::isFloat() const
{
    return getType() == ofxGenericCacheTypeFloat;
}

bool ofxGenericCache::isInt() const
{
    return getType() == ofxGenericCacheTypeInt;
}

bool ofxGenericCache::isBool() const
{
    return getType() == ofxGenericCacheTypeBool;
}

bool ofxGenericCache::isString() const
{
    return getType() == ofxGenericCacheTypeString;
}

bool ofxGenericCache::isObject() const
{
    return getType() == ofxGenericCacheTypeObject;
}

bool ofxGenericCache::isArray() const
{
    return getType() == ofxGenericCacheTypeArray;
}

float ofxGenericCache::asFloat( float defaultValue )
{
    if ( isFloat() )
    {
        return _floatValue;
    }
    return defaultValue;
}

int ofxGenericCache::asInt( int defaultValue )
{
    if ( isInt() )
    {
        return _intValue;
    }
    return defaultValue;
}

bool ofxGenericCache::asBool( bool defaultValue )
{
    if ( isBool() )
    {
        return _boolValue;
    }
    return defaultValue;
}

string ofxGenericCache::asString( string defaultValue )
{
    if ( isString() )
    {
        return *_stringValue;
    }
    return defaultValue;
}


void ofxGenericCache::write(string key, float value )
{
    if ( isObject() )
    {
        ( *_objectValue )[ key ] = createWithValue( value );
    }
}

void ofxGenericCache::write( string key, int value )
{
    if ( isObject() )
    {
        ( *_objectValue )[ key ] = createWithValue( value );
    }
}

void ofxGenericCache::write( string key, bool value )
{
    if ( isObject() )
    {
        ( *_objectValue )[ key ] = createWithValue( value );
    }
}

void ofxGenericCache::write( string key, string value )
{
    if ( isObject() )
    {
        ( *_objectValue )[ key ] = createWithValue( value );
    }
}

void ofxGenericCache::write( string key, const char* value )
{
    if ( isObject() )
    {
        ( *_objectValue )[ key ] = createWithValue( string( value ) );
    }
}

void ofxGenericCache::write(string key, ofPtr< ofxGenericCache > value )
{
    if ( isObject() )
    {
        ( *_objectValue )[ key ] = value;
    }
}

float ofxGenericCache::read( string key, float defaultValue )
{
    if ( isObject() )
    {
        ofPtr< ofxGenericCache > value = ( *_objectValue )[ key ];
        if ( value )
        {
            return value->asFloat();
        }
    }
    return defaultValue;
}

int ofxGenericCache::read( string key, int defaultValue )
{
    if ( isObject() )
    {
        ofPtr< ofxGenericCache > value = ( *_objectValue )[ key ];
        if ( value )
        {
            return value->asInt();
        }
    }
    return defaultValue;
}

bool ofxGenericCache::read( string key, bool defaultValue )
{
    if ( isObject() )
    {
        ofPtr< ofxGenericCache > value = ( *_objectValue )[ key ];
        if ( value )
        {
            return value->asBool();
        }
    }
    return defaultValue;
}

string ofxGenericCache::read( string key, string defaultValue )
{
    if ( isObject() )
    {
        ofPtr< ofxGenericCache > value = ( *_objectValue )[ key ];
        if ( value )
        {
            return value->asString();
        }
    }
    return defaultValue;
}

string ofxGenericCache::read( string key, const char* defaultValue )
{
    if ( isObject() )
    {
        ofPtr< ofxGenericCache > value = ( *_objectValue )[ key ];
        if ( value )
        {
            return value->asString();
        }
    }
    return string( defaultValue );
}

ofPtr< ofxGenericCache > ofxGenericCache::read( string key )
{
    if ( isObject() )
    {
        return ( *_objectValue )[ key ];
    }
    return ofPtr< ofxGenericCache >();
}

ofPtr< ofxGenericCache > ofxGenericCache::operator[]( string key )
{
    return read( key );
}

void ofxGenericCache::drop( string key )
{
    if ( isObject() )
    {
        ( *_objectValue ).erase( key );
    }
}

#pragma Array

void ofxGenericCache::write( unsigned int index, float value )
{
    if ( isArray() )
    {
        if ( index >= ( *_arrayValue ).size() )
        {
            ( *_arrayValue ).resize( index + 1 );
        }
        ( *_arrayValue )[ index ] = createWithValue( value );
    }
}

void ofxGenericCache::write( unsigned int index, int value )
{
    if ( isArray() )
    {
        if ( index >= ( *_arrayValue ).size() )
        {
            ( *_arrayValue ).resize( index + 1 );
        }
        ( *_arrayValue )[ index ] = createWithValue( value );
    }
}

void ofxGenericCache::write( unsigned int index, bool value )
{
    if ( isArray() )
    {
        if ( index >= ( *_arrayValue ).size() )
        {
            ( *_arrayValue ).resize( index + 1 );
        }
        ( *_arrayValue )[ index ] = createWithValue( value );
    }
}

void ofxGenericCache::write( unsigned int index, string value )
{
    if ( isArray() )
    {
        if ( index >= ( *_arrayValue ).size() )
        {
            ( *_arrayValue ).resize( index + 1 );
        }
        ( *_arrayValue )[ index ] = createWithValue( value );
    }
}

void ofxGenericCache::write( unsigned int index, const char* value )
{
    if ( isArray() )
    {
        if ( index >= ( *_arrayValue ).size() )
        {
            ( *_arrayValue ).resize( index + 1 );
        }
        ( *_arrayValue )[ index ] = createWithValue( string( value ) );
    }
}

void ofxGenericCache::write( unsigned int index, ofPtr< ofxGenericCache > value )
{
    if ( isArray() )
    {
        if ( index >= ( *_arrayValue ).size() )
        {
            ( *_arrayValue ).resize( index + 1 );
        }
        ( *_arrayValue )[ index ] = value;
    }
}

float ofxGenericCache::read( unsigned int index, float defaultValue )
{
    if ( isArray() && length() > index )
    {
        ofPtr< ofxGenericCache > value = ( *_arrayValue )[ index ];
        if ( value )
        {
            return value->asInt();
        }
    }
    return defaultValue;
}

int ofxGenericCache::read( unsigned int index, int defaultValue )
{
    if ( isArray() && length() > index )
    {
        ofPtr< ofxGenericCache > value = ( *_arrayValue )[ index ];
        if ( value )
        {
            return value->asInt();
        }
    }
    return defaultValue;
}

bool ofxGenericCache::read( unsigned int index, bool defaultValue )
{
    if ( isArray() && length() > index )
    {
        ofPtr< ofxGenericCache > value = ( *_arrayValue )[ index ];
        if ( value )
        {
            return value->asBool();
        }
    }
    return defaultValue;
}

string ofxGenericCache::read( unsigned int index, string defaultValue )
{
    if ( isArray() && length() > index )
    {
        ofPtr< ofxGenericCache > value = ( *_arrayValue )[ index ];
        if ( value )
        {
            return value->asString();
        }
    }
    return defaultValue;
}

string ofxGenericCache::read( unsigned int index, const char* defaultValue )
{
    if ( isArray() && length() > index )
    {
        ofPtr< ofxGenericCache > value = ( *_arrayValue )[ index ];
        if ( value )
        {
            return value->asString();
        }
    }
    return string( defaultValue ); 
}

ofPtr< ofxGenericCache > ofxGenericCache::read( unsigned int index )
{
    if ( isArray() && length() > index )
    {
        return ( *_arrayValue )[ index ];
    }
    return ofPtr< ofxGenericCache >();
}

ofPtr< ofxGenericCache > ofxGenericCache::operator[]( unsigned int index )
{
    return read( index );
}


unsigned int ofxGenericCache::length() const 
{
    if ( isArray() )
    {
        return ( *_arrayValue ).size();
    }
    return 0;
}

ofxGenericCacheObjectIterator ofxGenericCache::objectBegin()
{
    if ( isObject() )
    {
        return _objectValue->begin();
    }
    return ofxGenericCacheObjectIterator();
}

ofxGenericCacheObjectIterator ofxGenericCache::objectEnd()
{
    if ( isObject() )
    {
        return _objectValue->end();
    }
    return ofxGenericCacheObjectIterator();
}

ofxGenericCacheArrayIterator ofxGenericCache::arrayBegin()
{
    if ( isArray() )
    {
        return _arrayValue->begin();
    }
    return ofxGenericCacheArrayIterator();    
}

ofxGenericCacheArrayIterator ofxGenericCache::arrayEnd()
{
    if ( isArray() )
    {
        return _arrayValue->end();
    }
    return ofxGenericCacheArrayIterator();   
}

void ofxGenericCache::drop( int index )
{
    if ( isArray() )
    {
        ( *_arrayValue )[ index ] = ofPtr< ofxGenericCache >();
    }
}

void ofxGenericCache::setFileName( string fileName, bool fileInDocuments )
{
    _fileName = fileName;
    _fileInDocuments = fileInDocuments;
}

//loads the cache from disk
bool ofxGenericCache::readFromDisk()
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

void ofxGenericCache::convertFrom( Json::Value& convert )
{
    if ( convert.type() == Json::realValue && isFloat() )
    {
        _floatValue = ( float )convert.asDouble();
    } else if ( convert.type() == Json::intValue && isInt() )
    {
        _intValue = convert.asInt();
    } else if ( convert.type() == Json::booleanValue && isBool() )
    {
        _boolValue = convert.asBool();
    } else if ( convert.type() == Json::stringValue && isString() )
    {
        ( *_stringValue ) = convert.asString();
    } else if ( convert.type() == Json::objectValue && isObject() )
    {
        for( Json::Value::iterator travMembers = convert.begin(); travMembers != convert.end(); travMembers ++ )
        {
            string key = travMembers.key().asString();
            ofPtr< ofxGenericCache > test = createFrom( *travMembers );
            ( *this )[ key ] = test;
        }    
    } else if ( convert.type() == Json::arrayValue && isArray() )
    {
        _arrayValue->resize( convert.size() );
        for( unsigned int index = 0; index < convert.size(); index ++ )
        {
            ( *this )[ index ] = ofxGenericCache::createFrom( convert[ index ] ); 
        }
    }
}

ofPtr< ofxGenericCache > ofxGenericCache::createFrom( Json::Value& convert )
{
    switch( convert.type() )
    {
        case Json::nullValue:
            return ofPtr< ofxGenericCache >();
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
            ofPtr< ofxGenericCache > object = ofxGenericCache::create( false );
            for( Json::Value::iterator travMembers = convert.begin(); travMembers != convert.end(); travMembers ++ )
            {
                string key = travMembers.key().asString();
                ofPtr< ofxGenericCache > test = ofxGenericCache::createFrom( *travMembers );
                ( *object )[ key ] = test;            
            }
            return object;
        }
            break;
        case Json::arrayValue:
        {
            ofPtr< ofxGenericCache > array = ofxGenericCache::create( true );
            for( unsigned int index = 0; index < convert.size(); index ++ )
            {
                ( *array )[ index ] = ofxGenericCache::createFrom( convert[ index ] );
            }    
            return array;
        }
            break;
    }
    return ofPtr< ofxGenericCache >();
}


//writes the cache to disk
bool ofxGenericCache::writeToDisk()
{
    if ( _fileName.length() > 0 )
    {
        Json::Value* root = convertTo();
        ofxJSONElement write( *root );
        delete root;
        return write.save( _fileName, true, _fileInDocuments );
    }
    return false;
}

Json::Value* ofxGenericCache::convertTo()
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
        node = new Json::Value();
        for( ofxGenericCacheObjectIterator travMembers = objectBegin(); travMembers !=objectEnd(); travMembers ++ )
        {
            if ( ( *travMembers ).second )
            {
                Json::Value* member = ( *travMembers ).second->convertTo();
                if ( member )
                {
                    ( *node )[ ( *travMembers ).first ] = *member;
                    delete member;
                }
            } else 
            {
                ofxGLogError( "In writing to JSON a node " + ( *travMembers ).first + " is empty" );
            }
        }
    } else if ( isArray() )
    {
        node = new Json::Value();
        int indexCount = 0;
        for( ofxGenericCacheArrayIterator travIndices = arrayBegin(); travIndices !=arrayEnd(); travIndices ++ )
        {
            if ( *travIndices )
            {
                Json::Value* index = ( *travIndices )->convertTo();
                if ( index )
                {
                    ( *node )[ indexCount ] = *index;
                    delete index;
                }
                indexCount ++;
            } else 
            {
                ofxGLogError( "In writing to JSON a node at index " + ofxGIntegerToString( indexCount ) + " is empty" );
            }
        }
    }
    return node;
}

//empties the entire cache. a sync call must still be made to put this change onto the disk
void ofxGenericCache::purge()
{
    if ( isObject() )
    {
        ( *_objectValue ).clear();
    } else if ( isArray() )
    {
        ( *_arrayValue ).clear();
    }
}