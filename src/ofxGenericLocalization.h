//
//  ofxGenericLocalization.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 4/24/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericCache.h"

class ofxGenericCache;

class ofxGenericLocalization
{
public:
    static string getString( string key, string defaultValue = string() );
    
    virtual ~ofxGenericLocalization();
protected:
    static ofPtr< ofxGenericLocalization > create();
    ofxGenericLocalization();    
    virtual void init( ofPtrWeak< ofxGenericLocalization > setThis );
    static ofPtr< ofxGenericLocalization > _this;
    
    ofPtr< ofxGenericCache > _cache;
};

string ofxGLocalized( string key );
string ofxGLocalized( string key, string defaultValue );