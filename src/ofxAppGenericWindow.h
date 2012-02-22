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
#include "ofxGenericJNIObject.h"
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
    
    ofPtr< ofxGenericView > getRootView();
    void setRootView( ofPtr< ofxGenericView > view );
    
    NativeWindow getNativeWindow();

#if TARGET_ANDROID

    virtual jclass getJNIClass();
    static jclass getJNIClassStatic();

    virtual jobject getJNIInstance();
#endif
    
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
