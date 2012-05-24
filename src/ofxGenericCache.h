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

class ofxGenericCacheIterator;
class ofxGenericCacheConstIterator;

class ofxGenericCache
{
public:
    enum Type
    {
        ofxGenericCacheTypeUninitialized,
        ofxGenericCacheTypeFloat,
        ofxGenericCacheTypeInt,
        ofxGenericCacheTypeBool,
        ofxGenericCacheTypeString,
        ofxGenericCacheTypeObject,
        ofxGenericCacheTypeArray
    };
    static ofPtr< ofxGenericCache > create( bool asArray );
    Type getType() const;
    bool isFloat() const;
    bool isInt() const;
    bool isBool() const;
    bool isString() const;
    bool isObject() const;
    bool isArray() const;
    
    float asFloat( float defaultValue = 0.0f );
    int asInt( int defaultValue = 0 );
    bool asBool( bool defaultValue = false );
    string asString( string defaultValue = string() );

    // Object methods    
    virtual void write( string key, float value );
    virtual void write( string key, int value );
    virtual void write( string key, bool value );
    virtual void write( string key, string value );
    virtual void write( string key, const char* value );
    virtual void write( string key, ofPtr< ofxGenericCache > value );
    
    virtual float   read( string key, float defaultValue );
    virtual int     read( string key, int defaultValue );
    virtual bool    read( string key, bool defaultValue );
    virtual string  read( string key, string defaultValue );
    virtual string  read( string key, const char* defaultValue );
    virtual ofPtr< ofxGenericCache > read( string key );
    ofPtr< ofxGenericCache > operator[]( string key );    

    virtual void drop( string key );

    // Array methods
    unsigned int length() const;
    virtual void write( int index, float value );
    virtual void write( int index, int value );
    virtual void write( int index, bool value );
    virtual void write( int index, string value );
    virtual void write( int index, const char* value );
    virtual void write( int index, ofPtr< ofxGenericCache > value );
    
    virtual float   read( int index, float defaultValue );
    virtual int     read( int index, int defaultValue );
    virtual bool    read( int index, bool defaultValue );
    virtual string  read( int index, string defaultValue );
    virtual string  read( int index, const char* defaultValue );
    virtual ofPtr< ofxGenericCache > read( int index );
    ofPtr< ofxGenericCache > operator[]( int index );

    virtual void drop( int index );
    
    void setFileName( string fileName, bool fileInDocuments );
    
    //loads the cache from disk
    virtual bool readFromDisk();
    
    //writes the cache to disk
    virtual bool writeToDisk();
    
    //empties the entire cache
    virtual void purge();
    
    ofxGenericCacheIterator begin();
    ofxGenericCacheIterator end();
    
    virtual ~ofxGenericCache();
protected:
    ofxGenericCache();
    virtual void init( ofPtrWeak< ofxGenericCache > setThis, Type type );
    ofPtrWeak< ofxGenericCache > _this;
    static ofPtr< ofxGenericCache > createWithValue( float value );
    static ofPtr< ofxGenericCache > createWithValue( int value );
    static ofPtr< ofxGenericCache > createWithValue( bool value );
    static ofPtr< ofxGenericCache > createWithValue( string value );
    
    Type _type;
    union
    {
        float _floatValue;
        int _intValue;
        bool _boolValue;
        string* _stringValue;
        std::map< string, ofPtr< ofxGenericCache > >* _objectValue;
        std::vector< ofPtr< ofxGenericCache > >* _arrayValue;
    };  

    string _fileName;
    bool _fileInDocuments;    

    friend class ofxGenericCacheIterator;
    friend class ofxGenericCacheConstIterator;
};

#include "ofxGenericCacheIterator.h"