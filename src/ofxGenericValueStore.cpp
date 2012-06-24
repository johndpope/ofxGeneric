//
//  ofxGenericValueStore.cpp
//  ofxGeneric
//
//  Handles saving and loading cache data. Cache is stored in the documents directory
//  and exists until it is manually purged. A purge would usually happen when the cache
//  values become invalid due to more up to date information coming from the server.
//
//  Created by Eli Delventhal on 4/10/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericValueStore.h"
#include "ofxGenericUtility.h"

#include "ofxJSONElement.h"

ofPtr< ofxGenericValueStore > ofxGenericValueStore::create( bool asArray )
{
    ofPtr< ofxGenericValueStore > create( new ofxGenericValueStore() );
    Type type;
    if ( asArray )
    {
        type = ofxGenericValueStoreTypeArray;
    } else 
    {
        type = ofxGenericValueStoreTypeObject;
    }
    create->init( create, type );
    return create;
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::createWithValue( float value )
{
    ofPtr< ofxGenericValueStore > create( new ofxGenericValueStore() );
    create->init( create, ofxGenericValueStoreTypeFloat );
    create->_floatValue = value;
    return create;
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::createWithValue( int value )
{
    ofPtr< ofxGenericValueStore > create( new ofxGenericValueStore() );
    create->init( create, ofxGenericValueStoreTypeInt );
    create->_intValue = value;
    return create;
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::createWithValue( bool value )
{
    ofPtr< ofxGenericValueStore > create( new ofxGenericValueStore() );
    create->init( create, ofxGenericValueStoreTypeBool );
    create->_boolValue = value;
    return create;
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::createWithValue( string value )
{
    ofPtr< ofxGenericValueStore > create( new ofxGenericValueStore() );
    create->init( create, ofxGenericValueStoreTypeString );
    *( create->_stringValue ) = value;
    return create;
}

ofxGenericValueStore::ofxGenericValueStore()
: _type( ofxGenericValueStoreTypeUninitialized ), _fileInDocuments( false )
{
}

void ofxGenericValueStore::init( ofPtrWeak< ofxGenericValueStore > setThis, Type type )
{
    _this = setThis;
    _type = type;
    if ( _type == ofxGenericValueStoreTypeString )
    {
        _stringValue = new string();
    } else if ( _type == ofxGenericValueStoreTypeArray )
    {
        _arrayValue = new std::vector< ofPtr< ofxGenericValueStore > >();
    } else if ( _type == ofxGenericValueStoreTypeObject )
    {
        _objectValue = new std::map< string, ofPtr< ofxGenericValueStore > >();
    }
}

ofxGenericValueStore::~ofxGenericValueStore()
{
    if ( _type == ofxGenericValueStoreTypeString )
    {
        delete _stringValue;
    } else if ( _type == ofxGenericValueStoreTypeArray )
    {
        delete _arrayValue;
    } else if ( _type == ofxGenericValueStoreTypeObject )
    {
        delete _objectValue;
    }
}

ofxGenericValueStore::Type ofxGenericValueStore::getType() const
{
    return _type;
}

bool ofxGenericValueStore::isFloat() const
{
    return getType() == ofxGenericValueStoreTypeFloat;
}

bool ofxGenericValueStore::isInt() const
{
    return getType() == ofxGenericValueStoreTypeInt;
}

bool ofxGenericValueStore::isBool() const
{
    return getType() == ofxGenericValueStoreTypeBool;
}

bool ofxGenericValueStore::isString() const
{
    return getType() == ofxGenericValueStoreTypeString;
}

bool ofxGenericValueStore::isObject() const
{
    return getType() == ofxGenericValueStoreTypeObject;
}

bool ofxGenericValueStore::isArray() const
{
    return getType() == ofxGenericValueStoreTypeArray;
}

float ofxGenericValueStore::asFloat( float defaultValue )
{
    if ( isFloat() )
    {
        return _floatValue;
    } else if ( isInt() )
    {
        return ( float )_intValue;
    }
    return defaultValue;
}

int ofxGenericValueStore::asInt( int defaultValue )
{
    if ( isInt() )
    {
        return _intValue;
    } else if ( isFloat() )
    {
        return ( int )_floatValue;
    }
    return defaultValue;
}

bool ofxGenericValueStore::asBool( bool defaultValue )
{
    if ( isBool() )
    {
        return _boolValue;
    }
    return defaultValue;
}

string ofxGenericValueStore::asString( string defaultValue )
{
    if ( isString() )
    {
        return *_stringValue;
    }
    return defaultValue;
}

bool ofxGenericValueStore::exists( string key )
{
    if ( isObject() )
    {
        ofxGenericValueStoreObjectIterator find = _objectValue->find( key );
        return find != _objectValue->end();
    }
    return false;
}

void ofxGenericValueStore::write(string key, float value )
{
    if ( isObject() )
    {
        ( *_objectValue )[ key ] = createWithValue( value );
    }
}

void ofxGenericValueStore::write( string key, int value )
{
    if ( isObject() )
    {
        ( *_objectValue )[ key ] = createWithValue( value );
    }
}

void ofxGenericValueStore::write( string key, bool value )
{
    if ( isObject() )
    {
        ( *_objectValue )[ key ] = createWithValue( value );
    }
}

void ofxGenericValueStore::write( string key, string value )
{
    if ( isObject() )
    {
        ( *_objectValue )[ key ] = createWithValue( value );
    }
}

void ofxGenericValueStore::write( string key, const char* value )
{
    if ( isObject() )
    {
        ( *_objectValue )[ key ] = createWithValue( string( value ) );
    }
}

void ofxGenericValueStore::write(string key, ofPtr< ofxGenericValueStore > value )
{
    if ( isObject() )
    {
        ( *_objectValue )[ key ] = value;
    }
}

float ofxGenericValueStore::read( string key, float defaultValue )
{
    if ( isObject() && exists( key ) )
    {
        ofPtr< ofxGenericValueStore > value = ( *_objectValue )[ key ];
        if ( value )
        {
            return value->asFloat();
        }
    }
    return defaultValue;
}

int ofxGenericValueStore::read( string key, int defaultValue )
{
    if ( isObject() && exists( key ) )
    {
        ofPtr< ofxGenericValueStore > value = ( *_objectValue )[ key ];
        if ( value )
        {
            return value->asInt();
        }
    }
    return defaultValue;
}

bool ofxGenericValueStore::read( string key, bool defaultValue )
{
    if ( isObject() && exists( key ) )
    {
        ofPtr< ofxGenericValueStore > value = ( *_objectValue )[ key ];
        if ( value )
        {
            return value->asBool();
        }
    }
    return defaultValue;
}

string ofxGenericValueStore::read( string key, string defaultValue )
{
    if ( isObject() && exists( key ) )
    {
        ofPtr< ofxGenericValueStore > value = ( *_objectValue )[ key ];
        if ( value )
        {
            return value->asString();
        }
    }
    return defaultValue;
}

string ofxGenericValueStore::read( string key, const char* defaultValue )
{
    if ( isObject() && exists( key ) )
    {
        ofPtr< ofxGenericValueStore > value = ( *_objectValue )[ key ];
        if ( value )
        {
            return value->asString();
        }
    }
    return string( defaultValue );
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::read( string key )
{
    if ( isObject() && exists( key ) )
    {
        return ( *_objectValue )[ key ];
    }
    return ofPtr< ofxGenericValueStore >();
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::operator[]( string key )
{
    return read( key );
}

void ofxGenericValueStore::drop( string key )
{
    if ( isObject() )
    {
        ( *_objectValue ).erase( key );
    }
}

#pragma Array

void ofxGenericValueStore::write( unsigned int index, float value )
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

void ofxGenericValueStore::write( unsigned int index, int value )
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

void ofxGenericValueStore::write( unsigned int index, bool value )
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

void ofxGenericValueStore::write( unsigned int index, string value )
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

void ofxGenericValueStore::write( unsigned int index, const char* value )
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

void ofxGenericValueStore::write( unsigned int index, ofPtr< ofxGenericValueStore > value )
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

float ofxGenericValueStore::read( unsigned int index, float defaultValue )
{
    if ( isArray() && length() > index )
    {
        ofPtr< ofxGenericValueStore > value = ( *_arrayValue )[ index ];
        if ( value )
        {
            return value->asInt();
        }
    }
    return defaultValue;
}

int ofxGenericValueStore::read( unsigned int index, int defaultValue )
{
    if ( isArray() && length() > index )
    {
        ofPtr< ofxGenericValueStore > value = ( *_arrayValue )[ index ];
        if ( value )
        {
            return value->asInt();
        }
    }
    return defaultValue;
}

bool ofxGenericValueStore::read( unsigned int index, bool defaultValue )
{
    if ( isArray() && length() > index )
    {
        ofPtr< ofxGenericValueStore > value = ( *_arrayValue )[ index ];
        if ( value )
        {
            return value->asBool();
        }
    }
    return defaultValue;
}

string ofxGenericValueStore::read( unsigned int index, string defaultValue )
{
    if ( isArray() && length() > index )
    {
        ofPtr< ofxGenericValueStore > value = ( *_arrayValue )[ index ];
        if ( value )
        {
            return value->asString();
        }
    }
    return defaultValue;
}

string ofxGenericValueStore::read( unsigned int index, const char* defaultValue )
{
    if ( isArray() && length() > index )
    {
        ofPtr< ofxGenericValueStore > value = ( *_arrayValue )[ index ];
        if ( value )
        {
            return value->asString();
        }
    }
    return string( defaultValue ); 
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::read( unsigned int index )
{
    if ( isArray() && length() > index )
    {
        return ( *_arrayValue )[ index ];
    }
    return ofPtr< ofxGenericValueStore >();
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::operator[]( unsigned int index )
{
    return read( index );
}


unsigned int ofxGenericValueStore::length() const 
{
    if ( isArray() )
    {
        return ( *_arrayValue ).size();
    } else if ( isObject() )
    {
        return ( *_objectValue ).size();
    }
    return 0;
}

ofxGenericValueStoreObjectIterator ofxGenericValueStore::objectBegin()
{
    if ( isObject() )
    {
        return _objectValue->begin();
    }
    return ofxGenericValueStoreObjectIterator();
}

ofxGenericValueStoreObjectIterator ofxGenericValueStore::objectEnd()
{
    if ( isObject() )
    {
        return _objectValue->end();
    }
    return ofxGenericValueStoreObjectIterator();
}

ofxGenericValueStoreArrayIterator ofxGenericValueStore::arrayBegin()
{
    if ( isArray() )
    {
        return _arrayValue->begin();
    }
    return ofxGenericValueStoreArrayIterator();    
}

ofxGenericValueStoreArrayIterator ofxGenericValueStore::arrayEnd()
{
    if ( isArray() )
    {
        return _arrayValue->end();
    }
    return ofxGenericValueStoreArrayIterator();   
}

void ofxGenericValueStore::drop( int index )
{
    if ( isArray() )
    {
        ( *_arrayValue )[ index ] = ofPtr< ofxGenericValueStore >();
    }
}

void ofxGenericValueStore::setFileName( string fileName, bool fileInDocuments )
{
    _fileName = fileName;
    _fileInDocuments = fileInDocuments;
}

//loads the cache from disk
bool ofxGenericValueStore::readFromDisk()
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

void ofxGenericValueStore::convertFrom( Json::Value& convert )
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
            write( key, createFrom( *travMembers ) );
        }    
    } else if ( convert.type() == Json::arrayValue && isArray() )
    {
        _arrayValue->resize( convert.size() );
        for( unsigned int index = 0; index < convert.size(); index ++ )
        {
            write( index, ofxGenericValueStore::createFrom( convert[ index ] ) ); 
        }
    }
}

ofPtr< ofxGenericValueStore > ofxGenericValueStore::createFrom( Json::Value& convert )
{
    switch( convert.type() )
    {
        case Json::nullValue:
            return ofPtr< ofxGenericValueStore >();
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
            ofPtr< ofxGenericValueStore > object = ofxGenericValueStore::create( false );
            for( Json::Value::iterator travMembers = convert.begin(); travMembers != convert.end(); travMembers ++ )
            {
                string key = travMembers.key().asString();
                object->write( key, ofxGenericValueStore::createFrom( *travMembers ) );            
            }
            return object;
        }
            break;
        case Json::arrayValue:
        {
            ofPtr< ofxGenericValueStore > array = ofxGenericValueStore::create( true );
            for( unsigned int index = 0; index < convert.size(); index ++ )
            {
                array->write( index, ofxGenericValueStore::createFrom( convert[ index ] ) );
            }    
            return array;
        }
            break;
    }
    return ofPtr< ofxGenericValueStore >();
}


//writes the cache to disk
bool ofxGenericValueStore::writeToDisk()
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

Json::Value* ofxGenericValueStore::convertTo()
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
        for( ofxGenericValueStoreObjectIterator travMembers = objectBegin(); travMembers !=objectEnd(); travMembers ++ )
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
        for( ofxGenericValueStoreArrayIterator travIndices = arrayBegin(); travIndices !=arrayEnd(); travIndices ++ )
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
void ofxGenericValueStore::purge()
{
    if ( isObject() )
    {
        ( *_objectValue ).clear();
    } else if ( isArray() )
    {
        ( *_arrayValue ).clear();
    }
}