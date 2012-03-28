//
//  ofxGenericPlatform.cpp
//  ofxGeneric
//
//  Created by Ian Grossberg on 3/27/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericPlatform.h"

#if TARGET_OS_IPHONE
#include "UIDevice+Model.h"
#endif

ofxGenericPlatform* ofxGenericPlatform::_instance = NULL;

ofxGenericPlatform::ofxGenericPlatform()
{
}

ofxGenericPlatform::~ofxGenericPlatform()
{
}

ofxGenericPlatform* ofxGenericPlatform::getInstance()
{
    if ( ofxGenericPlatform::_instance == NULL )
    {
        ( new ofxGenericPlatform() )->setofxGenericPlatformInstanceToThis();
    }
    return ofxGenericPlatform::_instance;
}

void ofxGenericPlatform::setofxGenericPlatformInstanceToThis()
{
    if ( ofxGenericPlatform::_instance == NULL )
    {
        ofxGenericPlatform::_instance = this;
    } else
    {
        // TODO: exception
    }
}

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

string ofxGenericPlatform::getUniqueIdentifier()
{
#if TARGET_OS_IPHONE
    return ofxNSStringToString( [ [ UIDevice currentDevice ] uniqueIdentifier ] ); // TODO: uniqueIdentifier being deprecated
    //https://github.com/gekitz/UIDevice-with-UniqueIdentifier-for-iOS-5
#endif
}

ofOrientation ofxGenericPlatform::orientation()
{
#if TARGET_OS_IPHONE
    return iOSToofOrientation( [ [ UIDevice currentDevice ] orientation ] );
#endif
}

float ofxGenericPlatform::batteryLevel()
{
#if TARGET_OS_IPHONE
    return [ [ UIDevice currentDevice ] batteryLevel ];
#endif
}
