//
//  ofxGenericLocalization.cpp
//  ofxGeneric
//
//  Created by Ian Grossberg on 4/24/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericLocalization.h"
#include "ofxGenericUtility.h"
#include "ofxGenericCache.h"

// http://en.wikipedia.org/wiki/List_of_ISO_639-1_codes

ofPtr< ofxGenericLocalization > ofxGenericLocalization::_this;

ofPtr< ofxGenericLocalization > ofxGenericLocalization::create()
{
    ofPtr< ofxGenericLocalization > create( new ofxGenericLocalization() );
    create->init( create );
    return create;
}

void ofxGenericLocalization::init( ofPtrWeak< ofxGenericLocalization > setThis )
{
    if ( !ofxGenericLocalization::_this )
    {
        ofxGenericLocalization::_this = setThis.lock();
    } else
    {
        throw ofxGenericExceptionSubclassedSingletonInitializedTwice( "ofxGenericLocalization" );
    }
    
    _cache = ofxGenericCache::create( false );
    
    string isoLanguage;
#if TARGET_OS_IPHONE
    isoLanguage = ofxNSStringToString( [ [ NSLocale preferredLanguages ] objectAtIndex:0 ] );
#elif TARGET_ANDROID
    // http://stackoverflow.com/questions/4212320/get-the-current-language-in-device
#endif
    ofxGLogVerbose( "Using language " + isoLanguage );
    
    string fileName = "localizedStrings_" + isoLanguage + ".json";
    _cache->setFileName( fileName, false ); 
    if ( !_cache->readFromDisk() )
    {
        ofxGLogVerbose( fileName );
    }
}

string ofxGenericLocalization::getString( string key )
{
    if ( !ofxGenericLocalization::_this )
    {
        ofxGenericLocalization::create();
    }
    if ( ofxGenericLocalization::_this )
    {
        return ofxGenericLocalization::_this->_cache->read( key, key );
    }
    return key;
}


string ofxGenericLocalization::getString( string key, string defaultValue )
{
    if ( !ofxGenericLocalization::_this )
    {
        ofxGenericLocalization::create();
    }
    if ( ofxGenericLocalization::_this )
    {
        return ofxGenericLocalization::_this->_cache->read( key, defaultValue );
    }
    return defaultValue;
}

ofxGenericLocalization::ofxGenericLocalization()
{
}

ofxGenericLocalization::~ofxGenericLocalization()
{
}

string ofxGLocalized( string key )
{
    return ofxGenericLocalization::getString( key );
}

string ofxGLocalized( string key, string defaultValue )
{
    return ofxGenericLocalization::getString( key, defaultValue );
}