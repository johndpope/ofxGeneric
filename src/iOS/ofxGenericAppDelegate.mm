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
#include "ofxGenericUtilityiOS.h"
#include "ofxAppGenericWindow.h"

@interface ofxGenericAppDelegate ( Private )

-( void )installUncaughtExceptionHandler;

@end

void HandleUncaughtException( NSException *exception );
void SignalHandler( int signal );

@implementation ofxGenericAppDelegate


-( BOOL )application:( UIApplication* )application didFinishLaunchingWithOptions:( NSDictionary* )launchOptions
{
    ofPtr< ofxAppGenericWindow > window( new ofxAppGenericWindow( false ) );
    
    if ( window )
    {
        _ofxWindow = window;
        [ _ofxWindow->getNativeWindow() setScreen:[ UIScreen mainScreen ] ];
        _ofxWindow->getNativeWindow().frame = [UIScreen mainScreen].bounds;
        [ _ofxWindow->getNativeWindow() makeKeyAndVisible ];
    } else
    {
        ofxGLogError( "ofxGenericApp::getWindow() returned NULL, unable to initialize screen" );
    }
    
    ofPtr< ofxGenericApp > app = ofxGenericApp::getInstance();
    
    // TODO: don't use ofSetupOpenGL
    ofSetupOpenGL( window, 1024, 768, OF_FULLSCREEN );
    ofRunApp( app );
    
    
    
    if ( [ self shouldInstallDefaultUncaughtExceptionHandler ] )
    {
        [ self installUncaughtExceptionHandler ];
    }
    
    std::map< string, string > options = std::map< string, string >();
    for ( id k in launchOptions )
    {
        id val = [ launchOptions objectForKey:k ];
        NSString *str = @"";
        if ( [ val isKindOfClass:[ UILocalNotification class ] ] )
        {
            str = ((UILocalNotification *)val).alertAction;
        }
        else
        {
            str = [ val description ];
            //stringValue is incorrect, and is throwing selector issues. Use description instead
            //str = [ val stringValue ];
        }
        
        NSString *key = (NSString *) k;
        options[ ofxGToString( key ) ] = ofxGToString( str );
    }
    
    ofxGenericApp::getInstance()->setLaunchOptions( options );
    ofxGenericApp::getInstance()->finishedLaunching();
    
    return YES;
}

-( void )setupDeviceOrientationChangesNotifications
{
    [ [ UIDevice currentDevice ] beginGeneratingDeviceOrientationNotifications ];
    [ [ NSNotificationCenter defaultCenter ] addObserver: self 
                                                selector: @selector( deviceOrientationDidChange: ) 
                                                    name: UIDeviceOrientationDidChangeNotification 
                                                  object: nil ];
}

-( void )setupUpdateThroughDisplayLink
{
    _displayLink = [ [ CADisplayLink displayLinkWithTarget:self selector:@selector( update ) ] retain ];
    [ _displayLink addToRunLoop:[ NSRunLoop currentRunLoop ] forMode:NSDefaultRunLoopMode ];
}

-( void )setupKeyboardVisibilityNotifications
{
    [ [ NSNotificationCenter defaultCenter ] addObserver:self
                                                selector:@selector( keyboardWillShow: ) 
                                                    name:UIKeyboardWillShowNotification
                                                  object:nil ];
    [ [ NSNotificationCenter defaultCenter ] addObserver:self
                                                selector:@selector( keyboardWillHide: ) 
                                                    name:UIKeyboardWillHideNotification
                                                  object:nil ];
}

-( void )application:( UIApplication* )application didReceiveLocalNotification:( UILocalNotification* )notification
{
    ofxGenericApp::getInstance()->gotNotification( ofxGToString( notification.alertAction ) );
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
{
    ofxGenericApp::getInstance()->willResignActive();
}

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application
{ ofxGenericApp::getInstance()->didReceiveMemoryWarning(); }

- (void)applicationWillTerminate:(UIApplication *)application
{
    ofxGenericApp::getInstance()->willTerminate();
}

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
    CGRect keyboardBeginFrame;
    [ [ userInfo objectForKey:UIKeyboardFrameBeginUserInfoKey] getValue:&keyboardBeginFrame ];
    
    ofxGenericApp::getInstance()->keyboardWillShow(  ofxCGRectToofRectangle( keyboardBeginFrame ), ofxCGRectToofRectangle( keyboardEndFrame ) );
}

-( void )keyboardWillHide:( NSNotification* )notification
{
    ofxGenericApp::getInstance()->keyboardWillHide();
}

-( void )dealloc
{
    [ _displayLink release ];
    _displayLink = nil;
    [ super dealloc ];
}

-( BOOL )shouldInstallDefaultUncaughtExceptionHandler
{
    return YES;
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
