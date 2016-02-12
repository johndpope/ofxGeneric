//
//  ofxGenericSettings.cpp
//  ofxGeneric
//
//  Created by Ian Grossberg on 5/7/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericSettings.h"

#include "ofCommon.h"

void ofxGenericSettings::init( ofPtrWeak< ofxGenericSettings > setThis )
{
    ofxGenericValueStore::init( setThis, ofxGenericValueStoreTypeObject );

    setFileName( getFileName(), false );
    
    readFromDisk();
}

bool ofxGenericSettings::readFromDisk()
{
    // TODO: merge binary's settings with downloaded settings
    bool result = ofxGenericValueStore::readFromDisk();

    if ( result )
    {
        // old mechanism support
        ofPtr< ofxGenericValueStore > oldParse = _this.lock();
        parse( oldParse );
    } else
    {
        // We should never log paths containing the users id to the console or 3rd party logging systems.
        NSString *objCFileName = ofxStringToNSString(getFileName());
        NSString *lastPathComponent = [objCFileName lastPathComponent];
        ofxGLogError( "Unable to read settings file " + ofxNSStringToString(lastPathComponent) );
    }

    return result;
}

// ofxGenericSettings does not support writing
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

void ofxGenericSettings::parse( ofPtr< ofxGenericValueStore > &settings )
{
}

ofxGenericSettings::ofxGenericSettings()
{    
}