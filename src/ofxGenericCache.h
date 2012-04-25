//
//  ofxGenericCache.h
//  ofxGeneric
//
//  Handles saving and loading cache data. Cache is stored in the documents directory
//  and exists until it is manually purged. A purge would usually happen when the cache
//  values become invalid due to more up to date information coming from the server.
//
//  Created by Eli Delventhal on 4/10/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"
#include "ofxJSONElement.h"

class ofxGenericCache
{
public:
    static ofPtr< ofxGenericCache > create();
    virtual ~ofxGenericCache();
    
    virtual bool cache(string key, float val);
    virtual bool cache(string key, int val);
    virtual bool cache(string key, bool val);
    virtual bool cache(string key, string val);
    
    virtual float loadFloat(string key);
    virtual int loadInt(string key);
    virtual bool loadBool(string key);
    virtual string loadString(string key);
    
    void setFileName( string fileName, bool fileInDocuments );
    
    //loads the cache from disk
    virtual bool readFromDisk();
    
    //writes the cache to disk
    virtual bool writeToDisk();
    
    //empties the entire cache. a sync call must still be made to put this change onto the disk
    virtual void purge( string path="" );
    
protected:
    ofxGenericCache();
    virtual void init( ofPtrWeak< ofxGenericCache > setThis );
    ofPtrWeak< ofxGenericCache > _this;
    
    ofxJSONElement _map;
    
    //Json::Value resolvePath( );
    string _fileName;
    bool _fileInDocuments;
};