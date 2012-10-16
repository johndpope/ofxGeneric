//
//  ofxGenericSettings.cpp
//  ofxGeneric
//
//  Created by Ian Grossberg on 5/7/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericSettings.h"

void ofxGenericSettings::init( ofPtrWeak< ofxGenericSettings > setThis )
{
    ofxGenericValueStore::init( setThis, ofxGenericValueStoreTypeObject );
    load();
}

bool ofxGenericSettings::readFromDisk()
{
    return false;
}

bool ofxGenericSettings::writeToDisk()
{
    return false;
}

void ofxGenericSettings::dumpSettings()
{
    setFileName( getFileName(), true );
    
    ofxGenericValueStore::writeToDisk();
    
    setFileName( getFileName(), false );
}

void ofxGenericSettings::load()
{
    setFileName( getFileName(), false );

    // TODO: merge binary's settings with downloaded settings
    ofxGenericValueStore::readFromDisk();
    // old mechanism support
    ofPtr< ofxGenericValueStore > oldParse = _this.lock();
    parse( oldParse );
}

void ofxGenericSettings::parse( ofPtr< ofxGenericValueStore > &settings )
{
}

ofxGenericSettings::ofxGenericSettings()
{    
}