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
    
    static bool isTablet();
    
    static unsigned int cpuFrequency();
    static unsigned int cpuCount();
    static unsigned int totalMemory();

    static string macAddress();
    
    static string operatingSystemName();
    static string operatingSystemGroupName();
    static string operatingSystemVersion();
    
    static string uniqueIdentifier();
    
    static ofOrientation orientation();
    
    static float batteryLevel();
    
    static string dumpSupportedFontNames();
    static std::vector< string > supportedFontNames();
    
    static string locale();
    static string unknownLocale();
    
    static string imageFileName( string originalFileName );
    
#if TARGET_OS_IPHONE
    static bool isRetinaDisplay();
    static string imageSuffixRetinaDisplay();
    
    static bool is4InchDisplay();
    static string imageSuffix4InchDisplay();
    
    static string imageSuffixiPadPortrait();
    
    static string imageFileName( string originalFileName, bool is4Inch, bool isiPad, bool isRetina );
    
    static bool isiOSVersionOrNewer( int version );
#endif
    
    static ofPoint cameraFieldOfView();
};