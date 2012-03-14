//
//  ofxGenericConstants.h
//  iOS
//
//  Created by Ian Grossberg on 1/19/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofMain.h"

#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>
typedef UIView* NativeView;
typedef UIWindow* NativeWindow;
#else //if TARGET_ANDROID // FIXME: ARG ECLIPSE WHY DO YOU HATE ME PARSEEEEEEEE DAAAMN YOU
#define TARGET_ANDROID 1
// WTFFFFFFF
#include <jni.h>
typedef jobject NativeView;
typedef jobject NativeWindow;

#endif

#define ofxGenericModuleName "OF::Generic"

enum ofxGenericTextHorizontalAlignment
{
    ofxGenericTextHorizontalAlignmentLeft,
    ofxGenericTextHorizontalAlignmentCenter,
    ofxGenericTextHorizontalAlignmentRight
};

enum ofxGenericTableViewSeparatorStyle
{
    ofxGenericTableViewSeparatorStyleNone,
    ofxGenericTableViewSeparatorStyleSingleLine,
    ofxGenericTableViewSeparatorStyleSingleLineEtched
};
