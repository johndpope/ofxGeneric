//
//  ofxGenericAppiPhone.mm
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 12/28/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#include "ofxGenericApp.h"

#import <UIKit/UIKit.h>

#import "ofxGenericAppDelegate.h"

ofxGenericApp::ofxGenericApp()
: _window( NULL )
{
}

ofxGenericApp::~ofxGenericApp()
{
    if ( _window )
    {
        // handled for us by the ofPtr reference counter, TODO: hey I should switch over to using them :D
        //        delete _window;
        _window = NULL;
    }
    ofxGenericApp::_instance = NULL;
}

singletonInheretableSourceBase( ofxGenericApp );

void ofxGenericApp::runViaInfiniteLoop( ofxAppGenericWindow* window )
{
    // TODO: strong references
    _window = window;
    
    NSString* delegateClassName = NSStringFromClass( [ ofxGenericAppDelegate class ] );
    UIApplicationMain( nil, nil, nil, delegateClassName );
}

// TODO: come up with calling scheme, friending doesn't seem to be possible :(
void ofxGenericApp::finishedLaunching()
{
    ofxGenericView* rootView = new ofxGenericView();
    rootView->init( _window->getBounds() );
    
    _window->setRootView( rootView );
    
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
    
    setup();
          
     /*
     // show or hide status bar depending on OF_WINDOW or OF_FULLSCREEN
     [[UIApplication sharedApplication] setStatusBarHidden:(iPhoneGetOFWindow()->windowMode == OF_FULLSCREEN) animated:YES];
     
     // Listen to did rotate event
*/
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


ofxAppGenericWindow* ofxGenericApp::getWindow()
{
    return _window;
}

ofxGenericView* ofxGenericApp::getRootView()
{
    if ( _window )
    {
        return _window->getRootView();
    }
    return NULL;
}