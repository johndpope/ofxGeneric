//
//  ofxGenericValueStore.h
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

class ofxGenericValueStore;
namespace Json
{ class Value; }

typedef std::map< string, ofPtr< ofxGenericValueStore > >::iterator ofxGenericValueStoreObjectIterator;
typedef std::vector< ofPtr< ofxGenericValueStore > >::iterator ofxGenericValueStoreArrayIterator;

class ofxGenericValueStore
{
public:
    enum Type
    {
        ofxGenericValueStoreTypeUninitialized,
        ofxGenericValueStoreTypeFloat,
        ofxGenericValueStoreTypeInt,
        ofxGenericValueStoreTypeBool,
        ofxGenericValueStoreTypeString,
        ofxGenericValueStoreTypeObject,
        ofxGenericValueStoreTypeArray
    };
    static ofPtr< ofxGenericValueStore > create( bool asArray );
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
    virtual const std::vector< string >& getObjectKeys();
    virtual bool exists( string key );
    
    virtual void write( string key, float value );
    virtual void write( string key, int value );
    virtual void write( string key, bool value );
    virtual void write( string key, string value );
    virtual void write( string key, const char* value );
    virtual void write( string key, ofPtr< ofxGenericValueStore > value );
    
    virtual float   read( string key, float defaultValue );
    virtual int     read( string key, int defaultValue );
    virtual bool    read( string key, bool defaultValue );
    virtual string  read( string key, string defaultValue );
    virtual string  read( string key, const char* defaultValue );
    virtual ofPtr< ofxGenericValueStore > read( string key );
    ofPtr< ofxGenericValueStore > operator[]( string key );    

    virtual void drop( string key );

    // Array methods
    unsigned int length() const;
    virtual void write( unsigned int index, float value );
    virtual void write( unsigned int index, int value );
    virtual void write( unsigned int index, bool value );
    virtual void write( unsigned int index, string value );
    virtual void write( unsigned int index, const char* value );
    virtual void write( unsigned int index, ofPtr< ofxGenericValueStore > value );
    
    virtual float   read( unsigned int index, float defaultValue );
    virtual int     read( unsigned int index, int defaultValue );
    virtual bool    read( unsigned int index, bool defaultValue );
    virtual string  read( unsigned int index, string defaultValue );
    virtual string  read( unsigned int index, const char* defaultValue );
    virtual ofPtr< ofxGenericValueStore > read( unsigned int index );
    ofPtr< ofxGenericValueStore > operator[]( unsigned int index );

    virtual void drop( int index );
    
    void setFileName( string fileName, bool fileInDocuments );
    
    //loads the cache from disk
    virtual bool readFromDisk();
    
    //writes the cache to disk
    virtual bool writeToDisk();
    
    //empties the entire cache
    virtual void purge();
    
    ofxGenericValueStoreObjectIterator objectBegin();
    ofxGenericValueStoreObjectIterator objectEnd();
    ofxGenericValueStoreArrayIterator arrayBegin();
    ofxGenericValueStoreArrayIterator arrayEnd();
    
    virtual ~ofxGenericValueStore();
protected:
    ofxGenericValueStore();
    virtual void init( ofPtrWeak< ofxGenericValueStore > setThis, Type type );
    ofPtrWeak< ofxGenericValueStore > _this;
    static ofPtr< ofxGenericValueStore > createWithValue( float value );
    static ofPtr< ofxGenericValueStore > createWithValue( int value );
    static ofPtr< ofxGenericValueStore > createWithValue( bool value );
    static ofPtr< ofxGenericValueStore > createWithValue( string value );
    
    Type _type;
    union
    {
        float _floatValue;
        int _intValue;
        bool _boolValue;
        string* _stringValue;
        std::map< string, ofPtr< ofxGenericValueStore > >* _objectValue;
        std::vector< ofPtr< ofxGenericValueStore > >* _arrayValue;
    };
    std::vector< string > _objectKeys;
    void addObjectKey( string key );
    bool objectKeyExists( string key );
    void dropObjectKey( string key );

    string _fileName;
    bool _fileInDocuments;    
    
    void convertFrom( Json::Value& convert );
    Json::Value* convertTo();
    static ofPtr< ofxGenericValueStore > createFrom( Json::Value& convert );
};

