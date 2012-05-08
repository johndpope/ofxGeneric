//
//  ofxGenericSettings.cpp
//  ofxGeneric
//
//  Created by Ian Grossberg on 5/7/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericSettings.h"
#include "ofxGenericCache.h"

ofxGenericSettings::~ofxGenericSettings()
{
}

void ofxGenericSettings::init( ofPtrWeak< ofxGenericSettings > setThis )
{
    _this = setThis;
    load();
}

void ofxGenericSettings::load()
{
    ofPtr< ofxGenericCache > loaded = ofxGenericCache::create( false );
    loaded->setFileName( getFileName(), false );
    
    // TODO: merge binary's settings with downloaded settings
    if ( loaded->readFromDisk() )
    {
        parse( loaded );
    }
}

ofxGenericSettings::ofxGenericSettings()
{    
}