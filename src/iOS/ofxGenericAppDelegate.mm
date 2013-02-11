//
//  ofxGenericAppDelegate.mm
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 12/28/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#import "ofxGenericAppDelegate.h"
#import "ofxGenericApp.h"
#import "ofMain.h"

#include "ofxGenericException.h"

@interface ofxGenericAppDelegate ( Private )

-( void )installUncaughtExceptionHandler;

@end

void HandleUncaughtException( NSException *exception );
void SignalHandler( int signal );

@implementation ofxGenericAppDelegate

@synthesize window = _window;

-( void )applicationDidFinishLaunching:( UIApplication* )application
{
    [ self installUncaughtExceptionHandler ];

    ofPtr< ofxAppGenericWindow > window = ofxGenericApp::getInstance()->getWindow();

    if ( window )
    {
        self.window = window->getNativeWindow();
        
        [ self.window setScreen:[ UIScreen mainScreen ] ];
        [ self.window makeKeyAndVisible ];
    } else
    {
        ofxGLogError( "ofxGenericApp::getWindow() returned NULL, unable to initialize screen" );
    }
    ofxGenericApp::getInstance()->finishedLaunching();
    
    [ [ UIDevice currentDevice ] beginGeneratingDeviceOrientationNotifications ];
    [ [ NSNotificationCenter defaultCenter ] addObserver: self 
                                                selector: @selector( deviceOrientationDidChange: ) 
                                                    name: UIDeviceOrientationDidChangeNotification 
                                                  object: nil ];
    
    _displayLink = [ [ CADisplayLink displayLinkWithTarget:self selector:@selector( update ) ] retain ];
    [ _displayLink addToRunLoop:[ NSRunLoop currentRunLoop ] forMode:NSDefaultRunLoopMode ];
    
    [ [ NSNotificationCenter defaultCenter ] addObserver:self
                                                selector:@selector( keyboardWillShow: ) 
                                                    name:UIKeyboardWillShowNotification
                                                  object:nil ];
    [ [ NSNotificationCenter defaultCenter ] addObserver:self
                                                selector:@selector( keyboardWillHide: ) 
                                                    name:UIKeyboardWillHideNotification
                                                  object:nil ];
}

-( void )update
{
    ofNotifyUpdate();
}

-( void )deviceOrientationDidChange:( NSNotification* )notification
{
	UIDeviceOrientation interfaceOrientation = [ [ UIDevice currentDevice ] orientation];
    //    ofLog(OF_LOG_NOTICE, "Device orientation changed to %i", interfaceOrientation);
	
    if( interfaceOrientation != UIDeviceOrientationUnknown )
    {
        ofxGenericApp::getInstance()->deviceOrientationDidChange( ( ofOrientation )interfaceOrientation );
    }
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{ ofxGenericApp::getInstance()->didBecomeActive(); }

- (void)applicationWillResignActive:(UIApplication *)application
{ ofxGenericApp::getInstance()->willResignActive(); }

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application
{ ofxGenericApp::getInstance()->didReceiveMemoryWarning(); }

- (void)applicationWillTerminate:(UIApplication *)application
{ ofxGenericApp::getInstance()->willTerminate(); }

- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
}

- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error
{
}

- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo
{
}

#pragma mark iPhone 4.0
//- (void)application:(UIApplication *)application didReceiveLocalNotification:(UILocalNotification *)notification __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_4_0);
- (void)applicationDidEnterBackground:(UIApplication *)application
{
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{ ofxGenericApp::getInstance()->willEnterForeground(); }

/*
 - (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_3_0);
 
 
 - (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url;  // Will be deprecated at some point, please replace with application:openURL:sourceApplication:annotation:
 - (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_4_2); // no equiv. notification. return NO if the application can't open for some reason
 
 - (void)applicationSignificantTimeChange:(UIApplication *)application;        // midnight, carrier time update, daylight savings time change
 
 - (void)application:(UIApplication *)application willChangeStatusBarOrientation:(UIInterfaceOrientation)newStatusBarOrientation duration:(NSTimeInterval)duration;
 - (void)application:(UIApplication *)application didChangeStatusBarOrientation:(UIInterfaceOrientation)oldStatusBarOrientation;
 
 - (void)application:(UIApplication *)application willChangeStatusBarFrame:(CGRect)newStatusBarFrame;   // in screen coordinates
 - (void)application:(UIApplication *)application didChangeStatusBarFrame:(CGRect)oldStatusBarFrame;
 
 // one of these will be called after calling -registerForRemoteNotifications
 
 - (void)applicationProtectedDataWillBecomeUnavailable:(UIApplication *)application __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_4_0);
 - (void)applicationProtectedDataDidBecomeAvailable:(UIApplication *)application    __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_4_0);
 
 @property (nonatomic, retain) UIWindow *window __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_5_0);
 */

-( void )keyboardWillShow:( NSNotification* )notification
{
    NSDictionary* userInfo = [ notification userInfo ];
    CGRect keyboardEndFrame;
    [ [ userInfo objectForKey:UIKeyboardFrameEndUserInfoKey] getValue:&keyboardEndFrame ];
    
    ofxGenericApp::getInstance()->keyboardWillShow( CGRectToofRectangle( keyboardEndFrame ) );
}

-( void )keyboardWillHide:( NSNotification* )notification
{
    ofxGenericApp::getInstance()->keyboardWillHide();
}

-( void )dealloc
{
    release( _displayLink );
    [ super dealloc ];
}

-( void )installUncaughtExceptionHandler
{
	NSSetUncaughtExceptionHandler( &HandleUncaughtException );
    
    struct sigaction newSignalAction;
    memset( &newSignalAction, 0, sizeof( newSignalAction ) );
    newSignalAction.sa_handler = &SignalHandler;
    sigaction( SIGABRT, &newSignalAction, NULL );
    sigaction( SIGILL, &newSignalAction, NULL );
    sigaction( SIGSEGV, &newSignalAction, NULL );//
    sigaction( SIGFPE, &newSignalAction, NULL );//
    sigaction( SIGBUS, &newSignalAction, NULL );
    sigaction( SIGPIPE, &newSignalAction, NULL );//
}

-( void )alertView:( UIAlertView* )anAlertView clickedButtonAtIndex:( NSInteger )anIndex
{
    if ( ofxGenericApp::getInstance() )
    {
        ofxGenericApp::getInstance()->fatalErrorDismissed();
    }
}

-( UIViewController* )glViewController
{
    return nil;
}

@end

void HandleUncaughtException( NSException* nsException )
{
    if ( ofxGenericApp::getInstance() )
    {
        ofxGenericException exception( nsException );
        ofxGenericApp::getInstance()->handleUncaughtException( exception );
    } else 

    {
        ofxGLogFatalError( "Unable to retrieve ofxGenericApp to handle uncaught exception" );
    }
}

void SignalHandler( int signal )
{
    if ( ofxGenericApp::getInstance() )
    {
        ofxGenericApp::getInstance()->handleSignal( signal );
    } else 
    {
        ofxGLogFatalError( "Unable to retrieve ofxGenericApp to handle signal" );
    }
}
