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
#include "UIDevice-Hardware.h"
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
    return ofxNSStringToString( [ [ UIDevice currentDevice ] platformString ] );
#endif
}

bool ofxGenericPlatform::isTablet()
{
#if TARGET_OS_IPHONE
#ifdef UI_USER_INTERFACE_IDIOM
    return UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad;
#else
    return false;
#endif
#endif
}

unsigned int ofxGenericPlatform::cpuFrequency()
{
#if TARGET_OS_IPHONE
    return ( unsigned int )( [ [ UIDevice currentDevice ] cpuFrequency ] );
#endif
}

unsigned int ofxGenericPlatform::cpuCount()
{
#if TARGET_OS_IPHONE
    return ( unsigned int )( [ [ UIDevice currentDevice ] cpuCount ] );
#endif
}

unsigned int ofxGenericPlatform::totalMemory()
{
#if TARGET_OS_IPHONE
    return ( unsigned int )( [ [ UIDevice currentDevice ] totalMemory ] );
#endif
}

string ofxGenericPlatform::macAddress()
{
#if TARGET_OS_IPHONE
    return ofxNSStringToString( [ [ UIDevice currentDevice ] macaddress ] );
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
    string udid;
#if TARGET_OS_IPHONE
    NSString* nsUDID;
    UIDevice* currentDevice = [ UIDevice currentDevice ];
    if ( [ currentDevice respondsToSelector:@selector( identifierForVendor ) ] )
    {
        nsUDID = currentDevice.identifierForVendor.UUIDString;
        udid = ofxNSStringToString( nsUDID );
    } else if ( [ currentDevice respondsToSelector:@selector( uniqueIdentifier ) ] )
    {
        nsUDID = currentDevice.uniqueIdentifier;
        udid = ofxNSStringToString( nsUDID );
    } else
    {
        udid = "";
    }
#endif
    return udid;
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

string ofxGenericPlatform::locale()
{
    string result = unknownLocale();
#if TARGET_OS_IPHONE
    result = ofxNSStringToString( [ [ NSLocale currentLocale ] localeIdentifier ] );
#else
    throw ofxGenericExceptionMemberNotImplement( "ofxGenericPlatform", locale() );
#endif
    return result;
}

string ofxGenericPlatform::unknownLocale()
{
    return "unknown";
}

#if TARGET_OS_IPHONE
bool ofxGenericPlatform::isRetinaDisplay()
{
    return ( bool )[ UIDevice hasRetinaDisplay ];
}

string ofxGenericPlatform::imageSuffixRetinaDisplay()
{
    return ofxNSStringToString( [ UIDevice imageSuffixRetinaDisplay ] );
}

bool ofxGenericPlatform::is4InchDisplay()
{
    return ( bool )[ UIDevice has4InchDisplay ];
}

string ofxGenericPlatform::imageSuffix4InchDisplay()
{
    return ofxNSStringToString( [ UIDevice imageSuffix4InchDisplay ] );
}

string ofxGenericPlatform::imageFileName( string originalFileName, bool is4Inch, bool isRetina )
{
    string result = ofFilePath::removeExt( originalFileName );

    if ( is4Inch )
    {
        result += ofxGenericPlatform::imageSuffix4InchDisplay();
    }
    if ( isRetina )
    {
        result += ofxGenericPlatform::imageSuffixRetinaDisplay();
    }
    
    result += "." + ofFilePath::getFileExt( originalFileName );
    
    return result;
}

#endif