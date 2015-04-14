//
//  ofxGenericMain.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 4/18/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

// TZLA-619 // #include "ofMain.h"

#if TARGET_OS_IPHONE

// TZLA-619 // #import <UIKit/UIKit.h>

#elif TARGET_ANDROID

// TZLA-619 // #include "JNIObject.h"

#endif

#include "ofxGenericConstants.h"
#include "ofxGenericUtility.h"
#include "ofxGenericClass.h"
#include "ofxGenericDelegateContainer.h"
#include "ofxGenericLocalization.h"
#include "ofxGenericException.h"
