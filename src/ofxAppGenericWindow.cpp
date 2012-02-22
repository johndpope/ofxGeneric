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
#include "JNIRect.h"
#include "JNIMethod.h"

jclass ofxAppGenericWindow::_jniClass = NULL;
#endif

ofxAppGenericWindow::ofxAppGenericWindow()
#if TARGET_OS_IPHONE
: _window( nil )
#endif
{
#if TARGET_ANDROID
	registerJNIMethods();
#endif

	// TODO: move to AppGWindow::init
    _window = createNativeWindow();
}

ofxAppGenericWindow::~ofxAppGenericWindow()
{
#if TARGET_OS_IPHONE
    releaseView( _window );
#elif TARGET_ANDROID
	destroyJNIReference( _window );
#endif
}

NativeWindow ofxAppGenericWindow::createNativeWindow()
{
#if TARGET_OS_IPHONE
    return [ [ UIWindow alloc ] initWithFrame:[ [ UIScreen mainScreen ] bounds ] ];
#elif TARGET_ANDROID
    // TODO: exception handling
    string signature = JNIEncodeMethodSignature( 0, JNIType_object, ofxGenericView::className ); //"(Lcc/openframeworks/ofxGeneric/View;)V",

    JNIMethod createWindow(
    		JNIFindClass( ofxGenericApp::ActivityClassName ),
    		true,
    		"createWindow",
    		signature,
    		true
    		);

    jobject window = JNICallStaticObjectMethod( createWindow.getClass(), createWindow.getID() );
    return createJNIReference( window );
#else
	return NULL;
#endif
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

ofRectangle ofxAppGenericWindow::getFrame()
{
#if TARGET_OS_IPHONE
    return CGRectToofRectangle( [ _window bounds ] );
#elif TARGET_ANDROID
    // TODO: cache
    string signature = JNIEncodeMethodSignature( 0, JNIType_object, JNIRect::className );
    JNIMethod getFrame(
    		JNIFindClass( ofxGenericApp::ActivityClassName ),
    		true,
    		"getWindowFrame",
    		signature,
    		true
    		);

    jobject frame = JNICallStaticObjectMethod( getFrame.getClass(), getFrame.getID() );
    JNIRect jniRect( frame );
    return JNIRectToofRectangle( jniRect );
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
#elif TARGET_ANDROID
    JNIMethod setRootView(
    		JNIFindClass( ofxGenericApp::ActivityClassName ),
    		true,
    		"setRootView",
    		JNIEncodeMethodSignature( 1, JNIType_void, JNIType_object, ofxGenericView::className ), //"(Lcc/openframeworks/ofxGeneric/View;)V",
    		true
    		);

    JNICallStaticVoidMethod( setRootView.getClass(), setRootView.getID(), _rootView->getNativeView() );
#endif
}

#if TARGET_ANDROID
jclass ofxAppGenericWindow::getJNIClassStatic()
{
    if ( !_jniClass )
    	_jniClass = ( jclass )ofxAppGenericWindow::createJNIReferenceStatic( JNIFindClass( ofxGenericView::className ) );
    return _jniClass;
}

jclass ofxAppGenericWindow::getJNIClass()
{
	return ofxAppGenericWindow::getJNIClassStatic();
}

jobject ofxAppGenericWindow::getJNIInstance()
{
	return _window;
}
#endif

#if TARGET_ANDROID
void ofxAppGenericWindow::registerJNIMethods()
{
}
#endif
