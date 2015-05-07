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

// IOSP-40 // #include "ofEvents.h"

// TODO: depth, fsaa, retina
class ofxGenericOpenGLView : public ofxGenericView
{
public:
    static ofPtr< ofxGenericOpenGLView > create( const ofRectangle& setFrame );
    
    virtual ~ofxGenericOpenGLView();
    
    void didLoad();
    void willAppear();
    void didDisappear();
    
    // IOSP-40 // void update( ofEventArgs &args );

    void deleteFramebuffer();

    virtual ofColor getBackgroundColor();
    virtual void setBackgroundColor( const ofColor& setColor );

protected:
    ofxGenericOpenGLView();
    virtual void init( ofPtr< ofxGenericOpenGLView > setThis, const ofRectangle& setFrame );
    
    virtual void drawFrame();

    NativeView createNativeView( const ofRectangle& frame );
    
#if TARGET_OS_IPHONE
    EAGLContext* _context;
    void setContext( EAGLContext* newContext );
#endif
    
    void createFramebuffer();
    void setFramebuffer();
    bool presentFramebuffer();
  
    // The OpenGL ES names for the framebuffer and renderbuffer used to render to this view.
    GLint _framebufferWidth;
    GLint _framebufferHeight;    
    GLuint _defaultFramebuffer;
    GLuint _colorRenderbuffer;    
};