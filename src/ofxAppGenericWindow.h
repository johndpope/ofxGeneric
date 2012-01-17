//
//  ofxAppGenericWindow.h
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 12/28/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofAppBaseWindow.h"
#ifdef TARGET_OS_IPHONE
#import <UIKit/UIKit.h>
#import "ofAppiPhoneWindow.h"
#endif

#include "ofBaseApp.h"
#include "ofxGenericView.h"

class ofxAppGenericWindow
#ifdef TARGET_OS_IPHONE
: public ofAppiPhoneWindow
#else 
: public ofAppBaseWindow
#endif
{
public:
  	ofxAppGenericWindow();
	virtual ~ofxAppGenericWindow();
    
    virtual void runAppViaInfiniteLoop( ofBaseApp* appPtr );  
    
    ofRectangle getBounds();
    
    ofxGenericView* getRootView();
    void setRootView( ofxGenericView* view );
    
#ifdef TARGET_OS_IPHONE
    UIWindow* getUIWindow();
#endif
    
protected:
    
#ifdef TARGET_OS_IPHONE
    UIWindow* _uiWindow;
#endif
    
    ofxGenericView* _rootView;
};