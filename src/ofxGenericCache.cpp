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
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

ofPtr< ofxGenericCache > ofxGenericCache::create()
{
    ofPtr< ofxGenericCache > create( new ofxGenericCache() );
    create->init( create );
    return create;
}

ofxGenericCache::ofxGenericCache()
: _root( "{}" ), _fileInDocuments( false )
{
}

ofxGenericCache::ofxGenericCache( ofxJSONElement& root )
: _root( root ), _fileInDocuments( false )
{
}

void ofxGenericCache::init( ofPtrWeak< ofxGenericCache > setThis )
{
    _this = setThis;
}

ofxGenericCache::~ofxGenericCache()
{
}

void ofxGenericCache::write(string key, float val)
{
    _root[key] = val;
}

void ofxGenericCache::write(string key, int val)
{
    _root[key] = val;
}

void ofxGenericCache::write(string key, bool val)
{
    _root[key] = val;
}

void ofxGenericCache::write(string key, string val)
{
    _root[key] = val;
}

void ofxGenericCache::write(string key, ofPtr< ofxGenericCache >& val )
{
    _root[ key ] = val->_root;
}

bool ofxGenericCache::checkNotNullThrowIfUnexpectedType( string key, Json::Value& element, Json::ValueType expected )
{
    if ( element.type() == Json::nullValue )
    {
        return false;
    }
    if ( element.type() != expected )
    {
        throw ofxGenericExceptionKeyValueUnexpectedType( key.c_str(), expected, element.type() );
    }
    return true;
}

bool ofxGenericCache::read(string key, float& val)
{
    return read( key, _root[ key ], val );
}

bool ofxGenericCache::read( string key, float& val, float defaultValue )
{
    if ( !read( key, val ) )
    {
        val = defaultValue;
        return false;
    }
    return true;
}

bool ofxGenericCache::read(string key, int& val)
{
    return read( key, _root[ key ], val );
}

bool ofxGenericCache::read( string key, int& val, int defaultValue )
{
    if ( !read( key, val ) )
    {
        val = defaultValue;
        return false;
    }
    return true;
}

bool ofxGenericCache::read(string key, bool& val)
{
    return read( key, _root[ key ], val );
}

bool ofxGenericCache::read( string key, bool& val, bool defaultValue )
{
    if ( !read( key, val ) )
    {
        val = defaultValue;
        return false;
    }
    return true;
}

bool ofxGenericCache::read(string key, string& val)
{
    return read( key, _root[ key ], val );
}

bool ofxGenericCache::read( string key, string& val, string defaultValue )
{
    if ( !read( key, val ) )
    {
        val = defaultValue;
        return false;
    }
    return true;
}

bool ofxGenericCache::read( string key, ofPtr< ofxGenericCache >& val )
{
    return read( key, _root[ key ], val );
}

{
    if ( !read( key, val ) )
    {
        val = defaultValue;
        return false;
    }
    return true;
}

bool ofxGenericCache::read( string key, Json::Value& element, float& value )
{
    if ( checkNotNullThrowIfUnexpectedType( key, element, Json::realValue ) )
    {
        value = element.asDouble();
        return true;        
    }
    return false;
}

bool ofxGenericCache::read( string key, Json::Value& element, int& value )
{
    if ( checkNotNullThrowIfUnexpectedType( key, element, Json::intValue ) )
    {
        value = element.asInt();
        return true;
    }
    return false;
}

bool ofxGenericCache::read( string key, Json::Value& element, bool& value )
{
    if ( checkNotNullThrowIfUnexpectedType( key, element, Json::booleanValue ) )
    {
        value = element.asBool();
        return true;
    }
    return false;
}

bool ofxGenericCache::read( string key, Json::Value& element, string& value )
{
    if ( checkNotNullThrowIfUnexpectedType( key, element, Json::stringValue ) )
    {
        value = element.asString();
        return true;
    }
    return false;
}

bool ofxGenericCache::read( string key, Json::Value& element, ofPtr< ofxGenericCache >& value )
{
    if ( checkNotNullThrowIfUnexpectedType( key, element, Json::objectValue ) )
    {
        ofxJSONElement jsonElement( element );
        ofxGenericCache* wrap = new ofxGenericCache( jsonElement );
        value = ofPtr< ofxGenericCache >( wrap );
        return true;
    }
    return false;
}

bool ofxGenericCache::drop( string key )
{
    const Json::Value& element = _root.removeMember( key );
    return element.type() != Json::nullValue;
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
        return _root.openLocal( _fileName, _fileInDocuments );
    }
    return false;
}

//writes the cache to disk
bool ofxGenericCache::writeToDisk()
{
    if ( _fileName.length() > 0 )
    {
        return _root.save( _fileName, true, _fileInDocuments );
    }
    return false;
}

//empties the entire cache. a sync call must still be made to put this change onto the disk
void ofxGenericCache::purge( string path )
{
    if (path == "")
    {
        _root.parse( "{}" );
    }
    else
    {
        //todo figure this crap out later
        /*path.
        vector<string> tokens;
        istringstream iss(path);
        copy(istream_iterator<string>(iss),
             istream_iterator<string>(),
             back_inserter<vector<string> >(tokens));
        for (int i = 0; i < tokens.size(); i++)
        {
            
        }*/
    }
}


///////////////////////////////////////////////////////////////////////////////////////

ofxGenericExceptionKeyValueUnexpectedType::ofxGenericExceptionKeyValueUnexpectedType( const char* key, Json::ValueType expected, Json::ValueType actual ) throw()
: ofxGenericException( "Unexcepted key value type" ), _key( NULL ), _expected( NULL ), _actual( NULL )
{
    allocAndCopy( _key, key );
    allocAndCopy( _expected, valueTypeToString( expected ) );
    allocAndCopy( _actual, valueTypeToString( actual ) );
}

const char* ofxGenericExceptionKeyValueUnexpectedType::key() const throw()
{
    return _key;
}

const char* ofxGenericExceptionKeyValueUnexpectedType::excepted() const throw()
{
    return _expected;
}

const char* ofxGenericExceptionKeyValueUnexpectedType::actual() const throw()
{
    return _actual;
}

ofxGenericExceptionKeyValueUnexpectedType::~ofxGenericExceptionKeyValueUnexpectedType() throw()
{
    dealloc( _key );
    dealloc( _expected );
    dealloc( _actual );    
}
