//
//  ofxGenericCacheIterator.cpp
//  ofxGeneric
//
//  Created by Ian Grossberg on 5/23/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericCacheIterator.h"
#include "ofxGenericCache.h"

ofxGenericCacheIterator::ofxGenericCacheIterator( ofPtr< ofxGenericCache > cache )
: _typeIterator( NULL )
{    
    _cache = cache;
    if ( _cache )
    {
        if ( _cache->isObject() )
        {
            _typeIterator = new ofxGenericCacheObjectIterator( _cache );
        } else if ( _cache->isArray() )
        {
            _typeIterator = new ofxGenericCacheArrayIterator( _cache );
        }
    }
}

ofxGenericCacheIterator::~ofxGenericCacheIterator()
{
    if ( _typeIterator )
    {
        delete _typeIterator;
    }
}
    
string ofxGenericCacheIterator::key() const
{    
    if ( _typeIterator )
    {
        return _typeIterator->key();
    }
    return "";
}

unsigned int ofxGenericCacheIterator::index() const
{    
    if ( _typeIterator )
    {
        return _typeIterator->index();
    }
    return ofxGenericCacheIteratorInvalidIndex;
}

ofPtr< ofxGenericCache > ofxGenericCacheIterator::value()
{    
    if ( _typeIterator )
    {
        return _typeIterator->value();
    }
    return ofPtr< ofxGenericCache >();
}

ofPtr< ofxGenericCache > ofxGenericCacheIterator::operator*()
{    
    return value();
}

bool ofxGenericCacheIterator::operator==( const ofxGenericCacheIterator& compare )
{  
    if ( _typeIterator )
    {
        return ( *_typeIterator ) == compare;
    }
    return false;
}

bool ofxGenericCacheIterator::operator!=( const ofxGenericCacheIterator& compare )
{    
    if ( _typeIterator )
    {
        return ( *_typeIterator ) != compare;
    }
    return false;
}

ofxGenericCacheIterator& ofxGenericCacheIterator::operator++()
{    
    if ( _typeIterator )
    {
        ( *_typeIterator )++;
    }
    return *this;
}

ofxGenericCacheIterator& ofxGenericCacheIterator::operator--()
{    
    if ( _typeIterator )
    {
        ( *_typeIterator )--;
    }
    return *this;
}

ofxGenericCacheIterator ofxGenericCacheIterator::operator++( int )
{    
    if ( _typeIterator )
    {
        ( *_typeIterator )++;
    }
    return *this;
}

ofxGenericCacheIterator ofxGenericCacheIterator::operator--( int )
{    
    if ( _typeIterator )
    {
        ( *_typeIterator )--;
    }
    return *this;
}

void ofxGenericCacheIterator::goToEnd()
{
    if ( _typeIterator )
    {
        _typeIterator->goToEnd();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////

string ofxGenericCacheObjectIterator::key() const
{
    return _key;
}

ofPtr< ofxGenericCache > ofxGenericCacheObjectIterator::value()
{
    if ( _cache )
    {
        return ( *_cache )[ _key ];
    }
    return ofPtr< ofxGenericCache >();
}

ofPtr< ofxGenericCache > ofxGenericCacheObjectIterator::operator*()
{
    return value();
}

bool ofxGenericCacheObjectIterator::operator==( const ofxGenericCacheIterator& compare )
{
    return _key == compare.key();
}

bool ofxGenericCacheObjectIterator::operator!=( const ofxGenericCacheIterator& compare )
{
    return _key != compare.key();
}

ofxGenericCacheIterator& ofxGenericCacheObjectIterator::operator++()
{
    return *this;
}

ofxGenericCacheIterator& ofxGenericCacheObjectIterator::operator--()
{
    return *this;
}

ofxGenericCacheIterator ofxGenericCacheObjectIterator::operator++( int )
{
    return *this;
}

ofxGenericCacheIterator ofxGenericCacheObjectIterator::operator--( int )
{
    return *this;
}

void ofxGenericCacheObjectIterator::goToEnd()
{
    
}

////////////////////////////////////////////////////////////////////////////////////////////

ofxGenericCacheArrayIterator::ofxGenericCacheArrayIterator( ofPtr< ofxGenericCache > cache )
: ofxGenericCacheIterator( ofPtr< ofxGenericCache >() )
{
    _cache = cache;
}

unsigned int ofxGenericCacheArrayIterator::index() const
{
    return _index;
}

ofPtr< ofxGenericCache > ofxGenericCacheArrayIterator::value()
{
    if ( _cache )
    {
        return ( *_cache )[ _index ];
    }
    return ofPtr< ofxGenericCache >();
}

ofPtr< ofxGenericCache > ofxGenericCacheArrayIterator::operator*()
{
    return value();
}

bool ofxGenericCacheArrayIterator::operator==( const ofxGenericCacheIterator& compare )
{
    return _index == compare.index();
}

bool ofxGenericCacheArrayIterator::operator!=( const ofxGenericCacheIterator& compare )
{
    return _index == compare.index();
}

ofxGenericCacheIterator& ofxGenericCacheArrayIterator::operator++()
{
    _index++;
    return *this;
}

ofxGenericCacheIterator& ofxGenericCacheArrayIterator::operator--()
{
    _index--;
    return *this;
}

ofxGenericCacheIterator ofxGenericCacheArrayIterator::operator++( int )
{
    _index++;
    return *this;
}

ofxGenericCacheIterator ofxGenericCacheArrayIterator::operator--( int )
{
    _index--;
    return *this;
}

void ofxGenericCacheArrayIterator::goToEnd()
{
    _index = _cache->length();
}
