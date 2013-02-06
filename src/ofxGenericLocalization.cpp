//
//  ofxGenericLocalization.cpp
//  ofxGeneric
//
//  Created by Ian Grossberg on 4/24/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericLocalization.h"
#include "ofxGenericUtility.h"
#include "ofxGenericValueStore.h"
#include "ofxGenericException.h"

// TODO: proper fallback language setup
#define FallbackIsoLanguage "en"

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
    
    _cache = ofxGenericValueStore::create( false );
    
    string isoLanguage = getPreferredISOLanguage();
    string fileName = ofxGenericLocalization::getLocalizedFileName( isoLanguage );
    if ( !ofxGFileExists( ofToPath( fileName, false, true ) ) )
    {
        isoLanguage = FallbackIsoLanguage;
        fileName = ofxGenericLocalization::getLocalizedFileName( isoLanguage );
    }
    
    ofxGLogVerbose( "Using language " + isoLanguage );
    
    _cache->setFileName( fileName, false );
    if ( !_cache->readFromDisk() )
    {
        ofxGLogError( "Error reading localization file " + fileName );
    }
}

string ofxGenericLocalization::getLocalizedFileName( string isoLanguage )
{
    return "localizedStrings_" + isoLanguage + ".json";
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

string ofxGenericLocalization::getPreferredISOLanguage()
{
    string isoLanguage;
#if TARGET_OS_IPHONE
    isoLanguage = ofxNSStringToString( [ [ NSLocale preferredLanguages ] objectAtIndex:0 ] );
#elif TARGET_ANDROID
    // http://stackoverflow.com/questions/4212320/get-the-current-language-in-device
#endif
    return isoLanguage;
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