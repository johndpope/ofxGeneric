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

bool ofxGenericCache::cache(string key, float val)
{
    _map[key] = val;
    return true;
}

bool ofxGenericCache::cache(string key, int val)
{
    _map[key] = val;
    return true;
}

bool ofxGenericCache::cache(string key, bool val)
{
    _map[key] = val;
    return true;
}

bool ofxGenericCache::cache(string key, string val)
{
    _map[key] = val;
    return true;
}

float ofxGenericCache::loadFloat(string key)
{
    if (_map[key] == NULL)
    {
        return 0.0f;
    }
    return _map[key].asDouble();
}

int ofxGenericCache::loadInt(string key)
{
    if (_map[key] == NULL)
    {
        return 0;
    }
    return _map[key].asInt();
}

bool ofxGenericCache::loadBool(string key)
{
    if (_map[key] == NULL)
    {
        return false;
    }
    return (bool) _map[key].asBool();
}

string ofxGenericCache::loadString(string key)
{
    if ( _map[key] == NULL )
    {
        return "";
    }
    return (string) _map[key].asString();
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