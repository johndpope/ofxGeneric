//
//  ofxGenericPlatform.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 3/27/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericUtility.h"

class ofxGenericPlatform
{
public:
    virtual ~ofxGenericPlatform();
    static ofxGenericPlatform* getInstance();
    
    bool multitaskingSupported();
    string deviceName();
    string deviceModel();
    string operatingSystemName();
    string operatingSystemGroupName();
    string operatingSystemVersion();
    string getUniqueIdentifier();
    ofOrientation orientation();
    float batteryLevel();
    
protected:
    ofxGenericPlatform();
    static ofxGenericPlatform* _instance;
    void setofxGenericPlatformInstanceToThis();
};