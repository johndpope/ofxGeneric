//
//  ofxGenericPlatform.cpp
//  ofxGeneric
//
//  Created by Ian Grossberg on 3/27/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericPlatform.h"
#include "ofxGenericUtility.h"

#if TARGET_OS_IPHONE
#include "UIDevice+Model.h"
#include "OpenUDID.h"
#endif

bool ofxGenericPlatform::multitaskingSupported()
{
#if TARGET_OS_IPHONE
    return [ [ UIDevice currentDevice ] isMultitaskingSupported ];
#endif
}

string ofxGenericPlatform::deviceName()
{
#if TARGET_OS_IPHONE
    return ofxNSStringToString( [ [ UIDevice currentDevice ] name ] );
#endif
}

string ofxGenericPlatform::deviceModel()
{
#if TARGET_OS_IPHONE
    return ofxNSStringToString( [ [ UIDevice currentDevice ] model ] );
#endif
}

string ofxGenericPlatform::deviceModelVersion()
{
#if TARGET_OS_IPHONE
    return ofxNSStringToString( [ [ UIDevice currentDevice ] modelVersionPretty ] );
#endif
}

string ofxGenericPlatform::operatingSystemName()
{
#if TARGET_OS_IPHONE
    return ofxNSStringToString( [ [ UIDevice currentDevice ] systemName ] );
#endif
}

string ofxGenericPlatform::operatingSystemGroupName()
{
    string name;
#if TARGET_OS_IPHONE
    name = ofxNSStringToString( [ [ UIDevice currentDevice ] systemName ] );
#endif
    if ( name == "iPhone OS" )
        return "iOS";
    
    ofxGLog( OF_LOG_ERROR, "Unknown operating system, cannot find matching group for: " + name );
    
    return "Unknown";
}

string ofxGenericPlatform::operatingSystemVersion()
{
#if TARGET_OS_IPHONE
    return ofxNSStringToString( [ [ UIDevice currentDevice ] systemVersion ] );
#endif
}

string ofxGenericPlatform::uniqueIdentifier()
{
#if TARGET_OS_IPHONE
    return ofxNSStringToString( [ OpenUDID value ] );
    //https://github.com/gekitz/UIDevice-with-UniqueIdentifier-for-iOS-5
#endif
}

ofOrientation ofxGenericPlatform::orientation()
{
#if TARGET_OS_IPHONE
    return iOSDeviceToofOrientation( [ [ UIDevice currentDevice ] orientation ] );
#endif
}

float ofxGenericPlatform::batteryLevel()
{
#if TARGET_OS_IPHONE
    return [ [ UIDevice currentDevice ] batteryLevel ];
#endif
}

string ofxGenericPlatform::dumpSupportedFontNames()
{
    string supported;
    std::vector< string > supportedList = supportedFontNames();
    for( std::vector< string >::iterator travSupported = supportedList.begin(); travSupported != supportedList.end(); travSupported ++ )
    {
        supported += ( *travSupported ) + "\n";
    }
    return supported;
}

std::vector< string > ofxGenericPlatform::supportedFontNames()
{
    std::vector< string > supported;
#if TARGET_OS_IPHONE
    for( NSString* familyName in [ UIFont familyNames ] ) 
    {
        for ( NSString* fontName in [ UIFont fontNamesForFamilyName:familyName ] ) 
        {
            supported.push_back( ofxNSStringToString( fontName ) );
        }
    }
#endif
    return supported;
}