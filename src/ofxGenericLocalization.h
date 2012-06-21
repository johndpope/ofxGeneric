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
    
    virtual ~ofxGenericLocalization();
protected:
    static ofPtr< ofxGenericLocalization > create();
    ofxGenericLocalization();    
    virtual void init( ofPtrWeak< ofxGenericLocalization > setThis );
    static ofPtr< ofxGenericLocalization > _this;
    
    ofPtr< ofxGenericValueStore > _cache;
};

string ofxGLocalized( string key );
string ofxGLocalized( string key, string defaultValue );