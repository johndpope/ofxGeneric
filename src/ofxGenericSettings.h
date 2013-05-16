//
//  ofxGenericSettings.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 5/7/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericValueStore.h"

// refactor ofxGenericSettings to inheret from ofxGenericValueStore
class ofxGenericSettings : public ofxGenericValueStore
{
public:
    bool readFromDisk();
    bool writeToDisk();
    void dumpSettings();
    
protected:
    ofxGenericSettings();
    virtual void init( ofPtrWeak< ofxGenericSettings > setThis );

    virtual string getFileName() = 0;
    virtual void parse( ofPtr< ofxGenericValueStore >& settings ) ;
};