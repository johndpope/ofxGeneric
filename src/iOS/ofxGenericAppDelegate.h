//
//  ofxGenericAppDelegate.h
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 12/28/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofMain.h"
class ofxAppGenericWindow;

#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>
#import "ofxGenericApp.h"

@interface ofxGenericAppDelegate : UIResponder < UIApplicationDelegate >
{
    CADisplayLink* _displayLink;
    ofPtr< ofxAppGenericWindow > _ofxWindow;
}

-( BOOL )application:( UIApplication* )application didFinishLaunchingWithOptions:( NSDictionary* )launchOptions;
-( void )application:( UIApplication* )application didReceiveLocalNotification:( UILocalNotification* )notification;

-( BOOL )shouldInstallDefaultUncaughtExceptionHandler;

-( void )setupUpdateThroughDisplayLink;
-( void )setupDeviceOrientationChangesNotifications;
-( void )setupKeyboardVisibilityNotifications;

@end
#endif
