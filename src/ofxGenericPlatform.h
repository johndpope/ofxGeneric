//
//  ofxGenericPlatform.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 3/27/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"

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
    
    static string dumpSupportedFontNames();
    static std::vector< string > supportedFontNames();
    
#if TARGET_OS_IPHONE
    static bool isRetinaDisplay();
#endif
};