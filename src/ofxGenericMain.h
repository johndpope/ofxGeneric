//
//  ofxGenericMain.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 4/18/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofMain.h"

#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>

#elif TARGET_ANDROID
#include "JNIObject.h"

#endif

#include "ofxGenericConstants.h"
#include "ofxGenericUtility.h"