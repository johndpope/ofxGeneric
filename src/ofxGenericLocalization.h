//
//  ofxGenericLocalization.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 4/24/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericValueStore.h"

class ofxGenericValueStore;

class ofxGenericLocalization
{
public:
    static string getString( string key );
    static string getString( string key, string defaultValue );
    
    static string getPreferredISOLanguage();
    
    static void useLprojFolder( bool useLproj );
    
    virtual ~ofxGenericLocalization();
protected:
    static ofPtr< ofxGenericLocalization > create();
    ofxGenericLocalization();    
    virtual void init( ofPtrWeak< ofxGenericLocalization > setThis );
    static ofPtr< ofxGenericLocalization > _this;
    
    ofPtr< ofxGenericValueStore > _cache;
    
    static string getLocalizedFileName( string isoLanguage );
};

string ofxGLocalized( string key );
string ofxGLocalized( string key, string defaultValue );
string ofxGLocalized( const char* key );
string ofxGLocalized( const char* key, const char* defaultValue );