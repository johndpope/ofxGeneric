//
//  ofxGenericCacheIterator.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 5/23/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"

class ofxGenericCache;

const unsigned int ofxGenericCacheIteratorInvalidIndex = -1;

class ofxGenericCacheIterator
{
public:
    ofxGenericCacheIterator( ofPtr< ofxGenericCache > cache );
    virtual ~ofxGenericCacheIterator();
    
    virtual string key() const;
    virtual unsigned int index() const;
    virtual ofPtr< ofxGenericCache > value();
    virtual ofPtr< ofxGenericCache > operator*();
    virtual bool operator==( const ofxGenericCacheIterator& compare );
    virtual bool operator!=( const ofxGenericCacheIterator& compare );
    virtual ofxGenericCacheIterator& operator++();
    virtual ofxGenericCacheIterator& operator--();
    virtual ofxGenericCacheIterator operator++( int );
    virtual ofxGenericCacheIterator operator--( int );
    
    virtual void goToEnd();
    
protected:
    ofPtr< ofxGenericCache > _cache;
    ofxGenericCacheIterator* _typeIterator;
};

class ofxGenericCacheObjectIterator : public ofxGenericCacheIterator
{
public:
    ofxGenericCacheObjectIterator( ofPtr< ofxGenericCache > cache );
    
    virtual string key() const;
    virtual ofPtr< ofxGenericCache > value();
    virtual ofPtr< ofxGenericCache > operator*();
    virtual bool operator==( const ofxGenericCacheIterator& compare );
    virtual bool operator!=( const ofxGenericCacheIterator& compare );
    virtual ofxGenericCacheIterator& operator++();
    virtual ofxGenericCacheIterator& operator--();
    virtual ofxGenericCacheIterator operator++( int );
    virtual ofxGenericCacheIterator operator--( int );
    
    virtual void goToEnd();
    
protected:
    string _key;
};

class ofxGenericCacheArrayIterator : public ofxGenericCacheIterator
{
public:
    ofxGenericCacheArrayIterator( ofPtr< ofxGenericCache > cache );
    
    virtual unsigned int index() const;
    virtual ofPtr< ofxGenericCache > value();
    virtual ofPtr< ofxGenericCache > operator*();
    virtual bool operator==( const ofxGenericCacheIterator& compare );
    virtual bool operator!=( const ofxGenericCacheIterator& compare );
    virtual ofxGenericCacheIterator& operator++();
    virtual ofxGenericCacheIterator& operator--();
    virtual ofxGenericCacheIterator operator++( int );
    virtual ofxGenericCacheIterator operator--( int );

    virtual void goToEnd();

protected:
    int _index;
};