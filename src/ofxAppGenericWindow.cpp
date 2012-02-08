//
//  ofxAppGenericWindow.cpp
//
//  Created by Ian Grossberg on 12/29/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#include "ofxAppGenericWindow.h"
#if TARGET_OS_IPHONE
#import <Foundation/Foundation.h>
#endif

#include "ofxGenericApp.h"

ofxAppGenericWindow::ofxAppGenericWindow()
#if TARGET_OS_IPHONE
: _uiWindow( nil )
#endif
{
#if TARGET_OS_IPHONE
    _uiWindow = [ [ UIWindow alloc ] initWithFrame:[ [ UIScreen mainScreen ] bounds ] ];
#endif
}

ofxAppGenericWindow::~ofxAppGenericWindow()
{
#if TARGET_OS_IPHONE
    releaseView( _uiWindow );
#endif
}

#if TARGET_OS_IPHONE
UIWindow* ofxAppGenericWindow::getUIWindow()
{
    return _uiWindow;
}
#endif

void ofxAppGenericWindow::runAppViaInfiniteLoop( ofBaseApp* appPtr )
{
    // HACK: for now unsafely assume we've got an ofxGenericApp, TODO: does it need to be called from the app?
    ofxGenericApp* app = ( ofxGenericApp* )appPtr;
    ofPtr< ofxAppGenericWindow > passThis( this );
    app->runViaInfiniteLoop( passThis );
}

ofRectangle ofxAppGenericWindow::getBounds()
{
#if TARGET_OS_IPHONE
    return CGRectToofRectangle( [ _uiWindow bounds ] );
#endif
}

ofPtr< ofxGenericView > ofxAppGenericWindow::getRootView()
{
    return _rootView;
}

void ofxAppGenericWindow::setRootView( ofPtr< ofxGenericView > view )
{
    _rootView = view;
#if TARGET_OS_IPHONE
    [ _uiWindow setRootViewController:_rootView->getUIViewController() ];
#endif
}


