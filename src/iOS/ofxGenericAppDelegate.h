//
//  ofxGenericAppDelegate.h
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 12/28/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofMain.h"

#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>
#import "ofxGenericApp.h"

@interface ofxGenericAppDelegate : NSObject < UIApplicationDelegate >  
{
    CADisplayLink* _displayLink;
}

@property (retain, nonatomic) UIWindow* window;

@end
#endif