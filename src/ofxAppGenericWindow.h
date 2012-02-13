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
#endif

class ofxAppGenericWindow
#if TARGET_OS_IPHONE
: public ofAppiPhoneWindow
#else 
: public ofAppBaseWindow
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
    
protected:
    virtual NativeWindow createNativeWindow();
    NativeWindow _window;
    
    ofPtr< ofxGenericView > _rootView;
};
