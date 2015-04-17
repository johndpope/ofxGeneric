//
//  ofxAppGenericWindow.h
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 12/28/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"

#if TARGET_OS_IPHONE
// IOSP-40 // #include "ofAppiPhoneWindow.h"
#elif TARGET_ANDROID
// IOSP-40 // #include "ofAppBaseWindow.h"
#endif

#include "ofxGenericView.h"

class ofxAppGenericWindow
#if TARGET_OS_IPHONE
// IOSP-40 //: public ofAppiPhoneWindow
: public ofxGenericView
#elif TARGET_ANDROID
: public ofAppBaseWindow, public JNIObject
#endif
{
public:
#if TARGET_OS_IPHONE
  	ofxAppGenericWindow( bool universalApp );
#endif
	virtual ~ofxAppGenericWindow();
    
    virtual void runAppViaInfiniteLoop( ofBaseApp* appPtr );  
    
    ofRectangle getFrame();
    
    ofRectangle convertTo( const ofRectangle& rectangle, ofPtr< ofxGenericView > fromSpace );
    ofRectangle convertFrom( const ofRectangle& rectangle, ofPtr< ofxGenericView > toSpace );
    
    ofPtr< ofxGenericView > getRootView();
    void setRootView( ofPtr< ofxGenericView > view );
    
    NativeWindow getNativeWindow();
    
    void setBackgroundColor( const ofColor& color );

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
#if TARGET_OS_IPHONE
    bool _universalApp;
#endif
};
