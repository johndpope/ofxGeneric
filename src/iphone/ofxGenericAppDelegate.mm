//
//  ofxGenericAppDelegate.mm
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 12/28/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#import "ofxGenericAppDelegate.h"
#import "ofxGenericApp.h"

class ofxGenericAppFriend
{
public:
    ofxGenericApp* getFriend()
    { return ofxGenericApp::getInstance(); }
    
    void applicationDidFinishLaunching()
    { getFriend()->finishedLaunching(); }
    void applicationDidBecomeActive()
    { getFriend()->didBecomeActive(); }
    void applicationWillResignActive()
    { getFriend()->willResignActive(); }
    void applicationDidReceiveMemoryWarning()
    { getFriend()->didReceiveMemoryWarning(); }
    void applicationWillTerminate()
    { getFriend()->willTerminate(); }
    void deviceOrientationDidChange( ofOrientation newOrientation )
    { getFriend()->deviceOrientationDidChange( newOrientation ); }
};

@interface ofxGenericAppDelegate()
{
    ofxGenericAppFriend genericAppFriend;
}

@end

@implementation ofxGenericAppDelegate

@synthesize window = _window;

-( void )applicationDidFinishLaunching:( UIApplication* )application
{
    ofxAppGenericWindow* window = ofxGenericApp::getInstance()->getWindow();

    if ( window )
    {
        self.window = window->getUIWindow();
        
        [ self.window setScreen:[ UIScreen mainScreen ] ];
        [ self.window makeKeyAndVisible ];
        
        genericAppFriend.applicationDidFinishLaunching(); 
        
        [ [ UIDevice currentDevice ] beginGeneratingDeviceOrientationNotifications ];
        [ [ NSNotificationCenter defaultCenter ] addObserver: self 
                                                    selector: @selector( deviceOrientationDidChange: ) 
                                                        name: UIDeviceOrientationDidChangeNotification 
                                                      object: nil ];
    } else
    {
        // TODO: error
    }
}

-( void )deviceOrientationDidChange:( NSNotification* )notification
{
	UIDeviceOrientation interfaceOrientation = [ [ UIDevice currentDevice ] orientation];
    //    ofLog(OF_LOG_NOTICE, "Device orientation changed to %i", interfaceOrientation);
	
    if( interfaceOrientation != UIDeviceOrientationUnknown )
    {
        genericAppFriend.deviceOrientationDidChange( ( ofOrientation )interfaceOrientation );
    }
}


- (void)applicationDidBecomeActive:(UIApplication *)application
{ genericAppFriend.applicationDidBecomeActive(); }

- (void)applicationWillResignActive:(UIApplication *)application
{ genericAppFriend.applicationWillResignActive(); }

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application
{ genericAppFriend.applicationDidReceiveMemoryWarning(); }

- (void)applicationWillTerminate:(UIApplication *)application
{ genericAppFriend.applicationWillTerminate(); }

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
{
}

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


@end

