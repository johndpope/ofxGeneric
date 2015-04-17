//
//  ofxGenericAppDelegate.h
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 12/28/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#pragma once

// IOSP-40 // #include "ofMain.h"

class ofxAppGenericWindow;

// IOSP-40 // #if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>
#import "ofxGenericApp.h"

@interface ofxGenericAppDelegate : UIResponder < UIApplicationDelegate >
{
    CADisplayLink* _displayLink;
    ofPtr< ofxAppGenericWindow > _ofxWindow;

}

-( void )installUncaughtExceptionHandler;

void HandleUncaughtException( NSException *exception );
void SignalHandler( int signal );

-( BOOL )application:( UIApplication* )application didFinishLaunchingWithOptions:( NSDictionary* )launchOptions;
-( void )application:( UIApplication* )application didReceiveLocalNotification:( UILocalNotification* )notification;

-( BOOL )shouldInstallDefaultUncaughtExceptionHandler;

-( void )setupUpdateThroughDisplayLink;
-( void )setupDeviceOrientationChangesNotifications;
-( void )setupKeyboardVisibilityNotifications;

@end
// IOSP-40 // #endif
