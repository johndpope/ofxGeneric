//
//  ofxGenericDevice.cpp
//  ofxGeneric
//
//  Created by Konstantin Yavichev on 8/6/13.
//  Copyright (c) 2013 Lumos Labs. All rights reserved.
//

#include "ofxGenericDevice.h"

#include "ofxGenericUtility.h"

#import "ofCommon.h"

string ofxGenericDevice::getDeviceId()
{
    
#if TARGET_OS_IPHONE
    
    UIDevice* uiDevice = [ UIDevice currentDevice ];
    if ( uiDevice )
    {
        if ( [ uiDevice respondsToSelector:@selector( identifierForVendor ) ] )
        {
            return ofxNSStringToString( [ [ uiDevice identifierForVendor ] UUIDString ] );
        }
    }
    
#elif TARGET_ANDROID
    
    return "android";
    
#endif

    return "unknown";
}