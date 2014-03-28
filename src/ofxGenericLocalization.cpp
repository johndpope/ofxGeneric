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

static bool _useLprojFolder = false;

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

void ofxGenericLocalization::useLprojFolder(bool useLproj)
{
    _useLprojFolder = useLproj;
}

string ofxGenericLocalization::getLocalizedFileName( string isoLanguage )
{
    if (_useLprojFolder)
    {
        return isoLanguage + ".lproj/localizedStrings.json";
    }
    else
    {
        return "localizedStrings_" + isoLanguage + ".json";
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

string ofxGenericLocalization::getPreferredISOLanguage()
{
    string isoLanguage;
#if TARGET_OS_IPHONE
    NSData *publishedLanguagesData = [NSData dataWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"publishedLanguages" ofType:@"json"]];
    if (publishedLanguagesData) // if a published languages file exists, filter user's preferred languages by available published languages:
    {
        NSError *error = nil;
        NSDictionary *publishedLanguagesDict = [NSJSONSerialization JSONObjectWithData:publishedLanguagesData options:kNilOptions error:&error];
        if (publishedLanguagesDict && !error)
        {
            NSArray *publishedLanguages = publishedLanguagesDict[@"published"];
            NSArray *preferredLanguages = [NSBundle preferredLocalizationsFromArray:publishedLanguages];
            NSString *language = preferredLanguages[0];
            isoLanguage = [language UTF8String];
        }
        else
        {
            NSLog(@"error reading published languages json: %@", error);
        }
    }
    else // if published languages file does not exist, use older implementation:
    {
        isoLanguage = ofxNSStringToString( [ [ NSLocale preferredLanguages ] objectAtIndex:0 ] );
    }
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

string ofxGLocalized( const char* key )
{
    string result;
    if ( key != 0 )
    {
        result = ofxGLocalized( std::string( key ) );
    }
    return result;
}

string ofxGLocalized( const char* key, const char* defaultValue )
{
    string result;
    if ( key != 0 )
    {
        if ( defaultValue != 0 )
        {
            result = ofxGLocalized( std::string( key ), std::string( defaultValue ) );
        } else
        {
            result = ofxGLocalized( std::string( key ) );
        }
    }
    return result;
}