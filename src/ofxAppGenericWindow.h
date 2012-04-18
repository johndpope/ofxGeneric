//
//  ofxAppGenericWindow.h
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 12/28/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofAppBaseWindow.h"

#include "ofBaseApp.h"
#include "ofxGenericView.h"


#if TARGET_OS_IPHONE
#include "ofAppiPhoneWindow.h"
#elif TARGET_ANDROID
#include "JNIObject.h"
#endif

class ofxAppGenericWindow
#if TARGET_OS_IPHONE
: public ofAppiPhoneWindow
#elif TARGET_ANDROID
: public ofAppBaseWindow, public JNIObject
#endif
{
public:
  	ofxAppGenericWindow();
	virtual ~ofxAppGenericWindow();
    
    virtual void runAppViaInfiniteLoop( ofBaseApp* appPtr );  
    
    ofRectangle getFrame();
    
    ofRectangle convertTo( const ofRectangle& rectangle, ofPtr< ofxGenericView > fromSpace );
    ofRectangle convertFrom( const ofRectangle& rectangle, ofPtr< ofxGenericView > toSpace );
    
    ofPtr< ofxGenericView > getRootView();
    void setRootView( ofPtr< ofxGenericView > view );
    
    NativeWindow getNativeWindow();

#if TARGET_ANDROID

    virtual jclass getJNIClass();
    static jclass getJNIClassStatic();

    virtual jobject getJNIInstance();
#endif
    
    virtual ofOrientation getOrientation();

    string dumpViewGraph();
    
protected:
    virtual NativeWindow createNativeWindow();
    NativeWindow _window;

#if TARGET_ANDROID

    static jclass _jniClass;

    enum ofxAppGenericWindowMethods
    {
    	Last
    };

    void registerJNIMethods();
#endif
    
    ofPtr< ofxGenericView > _rootView;
};
