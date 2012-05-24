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
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

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

ofPtr< ofxGenericCache > ofxGenericCache::read( string key )
{
    if ( isObject() )
    {
        return ( *_objectValue )[ key ];
    }
    return ofPtr< ofxGenericCache >();
}

void ofxGenericCache::drop( string key )
{
    if ( isObject() )
    {
        ( *_objectValue ).erase( key );
    }
}

#pragma Array

void ofxGenericCache::write( int index, float value )
{
    if ( isArray() )
    {
        ( *_arrayValue )[ index ] = createWithValue( value );
    }
}

void ofxGenericCache::write( int index, int value )
{
    if ( isArray() )
    {
        ( *_arrayValue )[ index ] = createWithValue( value );
    }
}

void ofxGenericCache::write( int index, bool value )
{
    if ( isArray() )
    {
        ( *_arrayValue )[ index ] = createWithValue( value );
    }
}

void ofxGenericCache::write( int index, string value )
{
    if ( isArray() )
    {
        ( *_arrayValue )[ index ] = createWithValue( value );
    }
}

void ofxGenericCache::write( int index, ofPtr< ofxGenericCache > value )
{
    if ( isArray() )
    {
        ( *_arrayValue )[ index ] = value;
    }
}

float ofxGenericCache::read( int index, float defaultValue )
{
    if ( isArray() )
    {
        ofPtr< ofxGenericCache > value = ( *_arrayValue )[ index ];
        if ( value )
        {
            return value->asInt();
        }
    }
    return defaultValue;
}

int ofxGenericCache::read( int index, int defaultValue )
{
    if ( isArray() )
    {
        ofPtr< ofxGenericCache > value = ( *_arrayValue )[ index ];
        if ( value )
        {
            return value->asInt();
        }
    }
    return defaultValue;
}

bool ofxGenericCache::read( int index, bool defaultValue )
{
    if ( isArray() )
    {
        ofPtr< ofxGenericCache > value = ( *_arrayValue )[ index ];
        if ( value )
        {
            return value->asBool();
        }
    }
    return defaultValue;
}

string ofxGenericCache::read( int index, string defaultValue )
{
    if ( isArray() )
    {
        ofPtr< ofxGenericCache > value = ( *_arrayValue )[ index ];
        if ( value )
        {
            return value->asString();
        }
    }
    return defaultValue;
}

ofPtr< ofxGenericCache > ofxGenericCache::read( int index )
{
    if ( isArray() )
    {
        return ( *_arrayValue )[ index ];
    }
    return ofPtr< ofxGenericCache >();
}

unsigned int ofxGenericCache::length() const 
{
    if ( isArray() )
    {
        return ( *_arrayValue ).size();
    }
    return 0;
}

ofxGenericCacheIterator ofxGenericCache::begin()
{
    return ofxGenericCacheIterator( _this.lock() );
}

ofxGenericCacheIterator ofxGenericCache::end()
{
    ofxGenericCacheIterator create( _this.lock() );
    create.goToEnd();
    return create;
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
//        return _root.openLocal( _fileName, _fileInDocuments );
    }
    return false;
}

//writes the cache to disk
bool ofxGenericCache::writeToDisk()
{
    if ( _fileName.length() > 0 )
    {
//        return _root.save( _fileName, true, _fileInDocuments );
    }
    return false;
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

/*ofxGenericCacheIterator ofxGenericCache::begin()
{
    return _root.begin();
}

ofxGenericCacheConstIterator ofxGenericCache::begin() const
{
    return _root.begin();
}

ofxGenericCacheIterator ofxGenericCache::end()
{
    return _root.end();
}

ofxGenericCacheConstIterator ofxGenericCache::end() const
{
    return _root.end();
}*/