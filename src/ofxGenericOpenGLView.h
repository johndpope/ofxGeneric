//
//  ofxGenericOpenGLView.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 4/21/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericView.h"

#if TARGET_OS_IPHONE
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#endif

#include "ofEvents.h"

class ofxGenericOpenGLView : public ofxGenericView
{
public:
    static ofPtr< ofxGenericOpenGLView > create( const ofRectangle& setFrame );
    
    virtual ~ofxGenericOpenGLView();
    
    void didLoad();
    void willAppear();
    void didDisappear();
    
    void update( ofEventArgs &args );
    
protected:
    ofxGenericOpenGLView();
    virtual void init( ofPtr< ofxGenericOpenGLView > setThis, const ofRectangle& setFrame );
    
    virtual void drawFrame();

    NativeView createNativeView( const ofRectangle& frame );
    
    void setFramebuffer();
    void presentFramebuffer();
};