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
#include "ofxGenericException.h"

class ofxGenericCache
{
public:
    static ofPtr< ofxGenericCache > create();
    virtual ~ofxGenericCache();
    
    virtual void write(string key, float val);
    virtual void write(string key, int val);
    virtual void write(string key, bool val);
    virtual void write(string key, string val);
    virtual void write(string key, ofPtr< ofxGenericCache >& val );
    
    virtual bool read(string key, float& value);
    virtual bool read(string key, float& value, float defaultValue );
    virtual bool read(string key, int& value);
    virtual bool read(string key, int& value, int defaultValue );
    virtual bool read(string key, bool& value);
    virtual bool read(string key, bool& value, bool defaultValue );
    virtual bool read(string key, string& value);
    virtual bool read(string key, string& value, string defaultValue );
    virtual bool read(string key, ofPtr< ofxGenericCache >& val, bool asArray = false );
    
    static ofPtr< ofxGenericCache > createArray();
    bool isArray();
    int length();
    virtual void write( int index, float val);
    virtual void write( int index, int val);
    virtual void write( int index, bool val);
    virtual void write( int index, string val);
    virtual void write( int index, ofPtr< ofxGenericCache >& val );
    
    virtual bool read( int index, float& value);
    virtual bool read( int index, float& value, float defaultValue );
    virtual bool read( int index, int& value);
    virtual bool read( int index, int& value, int defaultValue );
    virtual bool read( int index, bool& value);
    virtual bool read( int index, bool& value, bool defaultValue );
    virtual bool read( int index, string& value);
    virtual bool read( int index, string& value, string defaultValue );
    virtual bool read( int index, ofPtr< ofxGenericCache >& val, bool asArray = false );

    virtual bool drop( string key );
    
    void setFileName( string fileName, bool fileInDocuments );
    
    //loads the cache from disk
    virtual bool readFromDisk();
    
    //writes the cache to disk
    virtual bool writeToDisk();
    
    //empties the entire cache. a sync call must still be made to put this change onto the disk
    virtual void purge( string path="" );
    
protected:
    ofxGenericCache();
    ofxGenericCache( ofxJSONElement& root );
    virtual void init( ofPtrWeak< ofxGenericCache > setThis );
    ofPtrWeak< ofxGenericCache > _this;
    
    ofxJSONElement _root;
    
    //Json::Value resolvePath( );
    string _fileName;
    bool _fileInDocuments;
    
    static bool checkNotNullThrowIfUnexpectedType( string key, Json::Value& element, Json::ValueType expected );
    
    virtual bool read( string key, Json::Value& element, float& value );
    virtual bool read( string key, Json::Value& element, int& value );
    virtual bool read( string key, Json::Value& element, bool& value );
    virtual bool read( string key, Json::Value& element, string& value );
    virtual bool read( string key, Json::Value& element, ofPtr< ofxGenericCache >& value, bool asArray = false );
};

class ofxGenericExceptionKeyValueUnexpectedType : public ofxGenericException
{
public:
    ofxGenericExceptionKeyValueUnexpectedType( const char* key, Json::ValueType expected, Json::ValueType actual ) throw();
    
    virtual const char* key() const throw();
    virtual const char* excepted() const throw();
    virtual const char* actual() const throw();
    
    virtual ~ofxGenericExceptionKeyValueUnexpectedType() throw();
    
protected:
    char* _key;
    char* _expected;
    char* _actual;
};