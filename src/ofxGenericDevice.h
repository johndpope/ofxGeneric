//
//  ofxGenericDevice.h
//  ofxGeneric
//
//  Created by Konstantin Yavichev on 8/6/13.
//  Copyright (c) 2013 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"

#if TARGET_OS_IPHONE
#import <Foundation/Foundation.h>
#endif

class ofxGenericDevice
{
public:
    static string getDeviceId();
};
