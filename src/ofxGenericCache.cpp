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
: _map( "{}" ), _fileInDocuments( false )
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
    _map[key] = val;
}

void ofxGenericCache::write(string key, int val)
{
    _map[key] = val;
}

void ofxGenericCache::write(string key, bool val)
{
    _map[key] = val;
}

void ofxGenericCache::write(string key, string val)
{
    _map[key] = val;
}

bool ofxGenericCache::read(string key, float& val)
{
    Json::Value& element = _map[ key ];
    if ( element.type() != Json::nullValue )
    {
        val = element.asDouble();
        return true;
    }
    return false;
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
    Json::Value& element = _map[ key ];
    if ( element.type() != Json::nullValue )
    {
        val = element.asInt();
        return true;
    }
    return false;
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
    Json::Value& element = _map[ key ];
    if ( element.type() != Json::nullValue )
    {
        val = element.asBool();
        return true;
    }
    return false;
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
    Json::Value& element = _map[ key ];
    if ( element.type() != Json::nullValue )
    {
        val = element.asString();
        return true;
    }
    return false;
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
        return _map.openLocal( _fileName, _fileInDocuments );
    }
    return false;
}

//writes the cache to disk
bool ofxGenericCache::writeToDisk()
{
    if ( _fileName.length() > 0 )
    {
        return _map.save( _fileName, true, _fileInDocuments );
    }
    return false;
}

//empties the entire cache. a sync call must still be made to put this change onto the disk
void ofxGenericCache::purge( string path )
{
    if (path == "")
    {
        _map.parse( "{}" );
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