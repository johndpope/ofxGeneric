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
    static bool multitaskingSupported();
    static string deviceName();
    static string deviceModel();
    static string deviceModelVersion();
    static string operatingSystemName();
    static string operatingSystemGroupName();
    static string operatingSystemVersion();
    static string uniqueIdentifier();
    static ofOrientation orientation();
    static float batteryLevel();
};