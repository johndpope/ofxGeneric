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
#include "ofxGenericConstants.h"

#if TARGET_ANDROID
#include "ofxGenericJNI.h"
#endif

ofxAppGenericWindow::ofxAppGenericWindow()
#if TARGET_OS_IPHONE
: _window( nil )
#endif
{
    _window = createNativeWindow();
}

ofxAppGenericWindow::~ofxAppGenericWindow()
{
#if TARGET_OS_IPHONE
    releaseView( _window );
#endif
}

NativeWindow ofxAppGenericWindow::createNativeWindow()
{
#if TARGET_OS_IPHONE
    return [ [ UIWindow alloc ] initWithFrame:[ [ UIScreen mainScreen ] bounds ] ];
#endif

#if TARGET_ANDROID
    return JNICallObjectMethod( true, "cc/openframeworks/ofxGeneric/View", "createAndInit", "(Landroid/graphics/Rect;)Lcc/openframeworks/ofxGeneric/View;", NULL );
#endif
	return NULL;
}

NativeWindow ofxAppGenericWindow::getNativeWindow()
{
    return _window;
}

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
    return CGRectToofRectangle( [ _window bounds ] );
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
    [ _window setRootViewController:_rootView->getUIViewController() ];
#endif
#if TARGET_ANDROID
#endif
}


