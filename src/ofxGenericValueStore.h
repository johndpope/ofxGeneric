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

class TiXmlDocument;
class TiXmlElement;
class TiXmlNode;

typedef std::map< string, ofPtr< ofxGenericValueStore > > ofxGenericValueStoreObject;
typedef std::vector< ofPtr< ofxGenericValueStore > > ofxGenericValueStoreArray;

typedef ofxGenericValueStoreObject::iterator ofxGenericValueStoreObjectIterator;
typedef ofxGenericValueStoreArray::iterator ofxGenericValueStoreArrayIterator;
typedef ofxGenericValueStoreObject::const_iterator ofxGenericValueStoreObjectConstIterator;
typedef ofxGenericValueStoreArray::const_iterator ofxGenericValueStoreArrayConstIterator;

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
    static ofPtr< ofxGenericValueStore > create( Type type );
    
    static ofPtr< ofxGenericValueStore > createWithValue( float value );
    static ofPtr< ofxGenericValueStore > createWithValue( int value );
    static ofPtr< ofxGenericValueStore > createWithValue( bool value );
    static ofPtr< ofxGenericValueStore > createWithValue( string value );
    static ofPtr< ofxGenericValueStore > createWithValue( const char* value );

    static ofPtr< ofxGenericValueStore > createFromJSON( string JSON );
    static ofPtr< ofxGenericValueStore > createFromXML( string xml );
    static ofPtr< ofxGenericValueStore > createFromXML( TiXmlDocument& xml );
    
    Type getType() const;
    
    virtual void write( float value );
    virtual void write( int value );
    virtual void write( bool value );
    virtual void write( string value );
    virtual void write( const char* value );

    void operator=( float value );
    void operator=( int value );
    void operator=( bool value );
    void operator=( string value );
    void operator=( const char* value );
    
    void operator++();
    void operator--();
    
    void operator++( int );
    void operator--( int );

    bool isFloat() const;
    bool isInt() const;
    bool isBool() const;
    bool isString() const;
    bool isObject() const;
    bool isArray() const;
    
    float asFloat( float defaultValue = 0.0f ) const;
    int asInt( int defaultValue = 0 ) const;
    bool asBool( bool defaultValue = false ) const;
    string asString( string defaultValue = string() ) const;

    bool operator==( ofPtr< ofxGenericValueStore > compare ) const;

    // Object methods    
    virtual const std::vector< string >& getObjectKeys() const;
    virtual bool exists( string key ) const;
    
    virtual void write( string key, float value );
    virtual void write( string key, int value );
    virtual void write( string key, bool value );
    virtual void write( string key, string value, bool onlyIfFilled = false );
    virtual void write( string key, const char* value, bool onlyIfFilled = false );
    virtual void write( string key, ofPtr< ofxGenericValueStore > value );
    
    virtual float   read( string key, float defaultValue ) const;
    virtual int     read( string key, int defaultValue ) const;
    virtual bool    read( string key, bool defaultValue ) const;
    virtual string  read( string key, string defaultValue ) const;
    virtual string  read( string key, const char* defaultValue ) const;
    virtual ofPtr< ofxGenericValueStore > read( string key ) const;
    ofPtr< ofxGenericValueStore > operator[]( string key ) const;    

    virtual void drop( string key );

    // Array methods
    unsigned int length() const;
    virtual void write( unsigned int index, float value );
    virtual void write( unsigned int index, int value );
    virtual void write( unsigned int index, bool value );
    virtual void write( unsigned int index, string value, bool onlyIfFilled = false );
    virtual void write( unsigned int index, const char* value, bool onlyIfFilled = false );
    virtual void write( unsigned int index, ofPtr< ofxGenericValueStore > value );
    
    virtual float   read( unsigned int index, float defaultValue ) const;
    virtual int     read( unsigned int index, int defaultValue ) const;
    virtual bool    read( unsigned int index, bool defaultValue ) const;
    virtual string  read( unsigned int index, string defaultValue ) const;
    virtual string  read( unsigned int index, const char* defaultValue ) const;
    virtual ofPtr< ofxGenericValueStore > read( unsigned int index ) const;
    ofPtr< ofxGenericValueStore > operator[]( unsigned int index ) const;

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

    ofxGenericValueStoreObjectConstIterator objectBegin() const;
    ofxGenericValueStoreObjectConstIterator objectEnd() const;
    ofxGenericValueStoreArrayConstIterator arrayBegin() const;
    ofxGenericValueStoreArrayConstIterator arrayEnd() const;

    string toJSONString() const;
    
    virtual ~ofxGenericValueStore();
protected:
    ofxGenericValueStore();
    virtual void init( ofPtrWeak< ofxGenericValueStore > setThis, Type type );
    ofPtrWeak< ofxGenericValueStore > _this;
    
    Type _type;
    union
    {
        float _floatValue;
        int _intValue;
        bool _boolValue;
        string* _stringValue;
        ofxGenericValueStoreObject* _objectValue;
        ofxGenericValueStoreArray* _arrayValue;
    } _values;
    std::vector< string > _objectKeys;
    void addObjectKey( string key );
    bool objectKeyExists( string key ) const;
    void dropObjectKey( string key );

    string _fileName;
    bool _fileInDocuments;    
    
    void convertFrom( Json::Value& convert );
    static ofPtr< ofxGenericValueStore > createFrom( Json::Value& convert );

    static ofPtr< ofxGenericValueStore > createFrom( TiXmlNode* convert );

    virtual Json::Value* convertTo() const;
    
    ofxGenericValueStoreObject* asObject() const;
    ofxGenericValueStoreArray* asArray() const;
    
    void ensureIndexAvailable( unsigned int index );
};

