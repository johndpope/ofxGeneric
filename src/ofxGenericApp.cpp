//
//  ofxGenericApp.cpp
//
//  Created by Ian Grossberg on 12/29/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#include "ofxGenericApp.h"

#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>

#import "ofxGenericAppDelegate.h"
#endif

#if TARGET_ANDROID
JNIEnv* ofxGenericApp::jniEnv = NULL;
#endif

ofxGenericApp::ofxGenericApp()
{
}

ofxGenericApp::~ofxGenericApp()
{
    ofxGenericApp::_instance = NULL;
}

singletonInheretableSourceBase( ofxGenericApp );

void ofxGenericApp::runViaInfiniteLoop( ofPtr< ofxAppGenericWindow > window )
{
	ofLog( ofxGenericModuleName, OF_LOG_VERBOSE, "App loop starting..." );
    // TODO: strong references
    _window = window;

#if TARGET_OS_IPHONE
    NSString* delegateClassName = NSStringFromClass( [ ofxGenericAppDelegate class ] );
    UIApplicationMain( nil, nil, nil, delegateClassName );
#endif
#if defined(TARGET_ANDROID)
    finishedLaunching();
#endif
}

// TODO: come up with calling scheme, friending doesn't seem to be possible :(
void ofxGenericApp::finishedLaunching()
{
	createRootView();

    /*
     //----- DAMIAN
     // set data path root for ofToDataPath()
     // path on iPhone will be ~/Applications/{application GUID}/openFrameworks.app/data
     // get the resource path for the bundle (ie '~/Applications/{application GUID}/openFrameworks.app')
     NSString *bundle_path_ns = [[NSBundle mainBundle] resourcePath];
     // convert to UTF8 STL string
     string path = [bundle_path_ns UTF8String];
     // append data
     //path.append( "/data/" ); // ZACH
     path.append( "/" ); // ZACH
     ofLog(OF_LOG_VERBOSE, "setting data path root to " + path);
     ofSetDataPathRoot( path );
     //-----

     iPhoneSetOrientation(OFXIPHONE_ORIENTATION_PORTRAIT);


     // call testApp::setup()
     ofRegisterTouchEvents((ofxiPhoneApp*)ofGetAppPtr());
     */

	ofNotifySetup();
	ofNotifyUpdate();

     /*
     // show or hide status bar depending on OF_WINDOW or OF_FULLSCREEN
     [[UIApplication sharedApplication] setStatusBarHidden:(iPhoneGetOFWindow()->windowMode == OF_FULLSCREEN) animated:YES];

     // Listen to did rotate event
*/
}

void ofxGenericApp::createRootView()
{
    ofPtr< ofxGenericView > rootView( new ofxGenericView() );
    rootView->init( rootView, _window->getBounds() );

    _window->setRootView( rootView );
}

void ofxGenericApp::didBecomeActive()
{
}
void ofxGenericApp::willResignActive()
{
}
void ofxGenericApp::willTerminate()
{
}

void ofxGenericApp::didReceiveMemoryWarning()
{
}

void ofxGenericApp::deviceOrientationDidChange( ofOrientation newOrientation )
{
}


ofPtr< ofxAppGenericWindow > ofxGenericApp::getWindow()
{
    return _window;
}

ofPtr< ofxGenericView > ofxGenericApp::getRootView()
{
    if ( _window )
    {
        return _window->getRootView();
    }
    return ofPtr< ofxGenericView >();
}



