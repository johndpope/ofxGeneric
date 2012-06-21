//
//  ofxGenericSettings.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 5/7/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"

class ofxGenericValueStore;

class ofxGenericSettings
{
public:
    virtual ~ofxGenericSettings();
    
    void load();
    
protected:
    ofxGenericSettings();
    virtual void init( ofPtrWeak< ofxGenericSettings > setThis );
    ofPtrWeak< ofxGenericSettings > _this;

    virtual string getFileName() = 0;
    virtual void parse( ofPtr< ofxGenericValueStore >& settings ) = 0;
};