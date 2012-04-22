//
//  ofxGenericOpenGLView.cpp
//  ofxGeneric
//
//  Created by Ian Grossberg on 4/21/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericOpenGLView.h"
#include "ofxGenericUtility.h"
#include "ofxGenericApp.h"

#if TARGET_OS_IPHONE
#import <QuartzCore/QuartzCore.h>

@class EAGLContext;

@interface ofxGenericOpenGLViewNative : UIView
{
    // The OpenGL ES names for the framebuffer and renderbuffer used to render to this view.
    GLuint _defaultFramebuffer;
    GLuint _colorRenderbuffer;    
    
    // The pixel dimensions of the CAEAGLLayer.
    GLint _framebufferWidth;
    GLint _framebufferHeight;    
}
-( id )initWithFrame:( CGRect )frame;
@property ( nonatomic, retain ) EAGLContext* context;

-( void )createFramebuffer;
-( void )setFramebuffer;
-( void )deleteFramebuffer;
-( BOOL )presentFramebuffer;
@end

#endif

ofPtr< ofxGenericOpenGLView > ofxGenericOpenGLView::create( const ofRectangle& setFrame )
{
    ofPtr< ofxGenericOpenGLView > create( new ofxGenericOpenGLView() );
    create->init( create, setFrame );
    return create;
}

void ofxGenericOpenGLView::init( ofPtr< ofxGenericOpenGLView > setThis, const ofRectangle& setFrame )
{
    ofxGenericView::init( setThis, setFrame );
}
                 
ofxGenericOpenGLView::ofxGenericOpenGLView()
{    
}

ofxGenericOpenGLView::~ofxGenericOpenGLView()
{
#if TARGET_OS_IPHONE    
    if ( [ _view isKindOfClass:[ ofxGenericOpenGLViewNative class ] ] )
    {
        ofxGenericOpenGLViewNative* nativeView = ( ofxGenericOpenGLViewNative* )_view;
        [ nativeView setContext:nil ];
    }
#endif

}

NativeView ofxGenericOpenGLView::createNativeView( const ofRectangle& frame )
{
#if TARGET_OS_IPHONE
    ofxGenericOpenGLViewNative* view = [ [ ofxGenericOpenGLViewNative alloc ] initWithFrame:ofRectangleToCGRect( frame ) ];
    return view;
#elif TARGET_ANDROID
#endif
}

void ofxGenericOpenGLView::didLoad()
{
#if TARGET_OS_IPHONE
    EAGLContext* context = nil;
//    context = [ [ EAGLContext alloc ] initWithAPI:kEAGLRenderingAPIOpenGLES2 ];
    if ( !context )
    {
        context = [ [ EAGLContext alloc ] initWithAPI:kEAGLRenderingAPIOpenGLES1 ];
    }
    if ( !context )
    {
        // TODO: exception
        ofxGLogFatalError( "Could not create OpenGL ES context" );
    }
    
    if ( ![ EAGLContext setCurrentContext:context ] )
    {
        // TODO: exception
        ofxGLogFatalError( "Could not set OpenGL ES context" );
    }
    
    if ( [ _view isKindOfClass:[ ofxGenericOpenGLViewNative class ] ] )
    {
        ofxGenericOpenGLViewNative* nativeView = ( ofxGenericOpenGLViewNative* )_view;
        [ nativeView setContext:context ];
        [ nativeView setFramebuffer ];
    }
    if ( [ context API ] == kEAGLRenderingAPIOpenGLES2 )
    {
        // loadShaders();
    }
    
    [ context release ];
    
#endif
}

void ofxGenericOpenGLView::willAppear()
{
    ofxGenericView::willAppear();
    ofAddListener( ofEvents.update, this, &ofxGenericOpenGLView::update );
}

void ofxGenericOpenGLView::didDisappear()
{
    ofxGenericView::didDisappear();
    ofRemoveListener( ofEvents.update, this, &ofxGenericOpenGLView::update );
}

void ofxGenericOpenGLView::update( ofEventArgs &args )
{
    setFramebuffer();
    
    drawFrame();
    
    presentFramebuffer();
}

void ofxGenericOpenGLView::drawFrame()
{
    
}

void ofxGenericOpenGLView::setFramebuffer()
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ ofxGenericOpenGLViewNative class ] ] )
    {
        ofxGenericOpenGLViewNative* nativeView = ( ofxGenericOpenGLViewNative* )_view;
        [ nativeView setFramebuffer ];
    }
#endif
}

void ofxGenericOpenGLView::presentFramebuffer()
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ ofxGenericOpenGLViewNative class ] ] )
    {
        ofxGenericOpenGLViewNative* nativeView = ( ofxGenericOpenGLViewNative* )_view;
        [ nativeView presentFramebuffer ];
    }
#endif
}


#if TARGET_OS_IPHONE

@implementation ofxGenericOpenGLViewNative

@synthesize context;

+( Class )layerClass
{
    return [ CAEAGLLayer class ];
}

-( id )initWithFrame:( CGRect )frame
{
    self = [ super initWithFrame:frame ];
	if ( self ) 
    {
        CAEAGLLayer* eaglLayer = ( CAEAGLLayer* )self.layer;
        
        eaglLayer.opaque = TRUE;
        eaglLayer.drawableProperties = [ NSDictionary dictionaryWithObjectsAndKeys:
                                        [ NSNumber numberWithBool:FALSE ], kEAGLDrawablePropertyRetainedBacking,
                                        kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat,
                                        nil ];
        
        _framebufferWidth = frame.size.width;
        _framebufferHeight = frame.size.height;
    }
    
    return self;
}

-( void )dealloc
{
    [ self deleteFramebuffer ];    
    [ context release ];
    
    [ super dealloc ];
}

-( void )setContext:( EAGLContext* )newContext
{
    if ( context != newContext ) 
    {
        [ self deleteFramebuffer ];
        
        [ context release ];
        context = [ newContext retain ];
        
        [ EAGLContext setCurrentContext:nil ];
    }
}

-( void )createFramebuffer
{
    if ( context && !_defaultFramebuffer ) 
    {
        [ EAGLContext setCurrentContext:context ];
        
        // Create default framebuffer object.
        glGenFramebuffers( 1, &_defaultFramebuffer );
        glBindFramebuffer( GL_FRAMEBUFFER, _defaultFramebuffer );
        
        // Create color render buffer and allocate backing store.
        glGenRenderbuffers( 1, &_colorRenderbuffer );
        glBindRenderbuffer( GL_RENDERBUFFER, _colorRenderbuffer );
        [ context renderbufferStorage:GL_RENDERBUFFER fromDrawable:( CAEAGLLayer* )self.layer ];
        glGetRenderbufferParameteriv( GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &_framebufferWidth );
        glGetRenderbufferParameteriv( GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &_framebufferHeight );
        
        glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _colorRenderbuffer);
        
        if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
        {
            NSLog( @"Failed to make complete framebuffer object %x", glCheckFramebufferStatus( GL_FRAMEBUFFER ) );
        }
    }
}

-( void )deleteFramebuffer
{
    if ( context ) 
    {
        [ EAGLContext setCurrentContext:context ];
        
        if ( _defaultFramebuffer) 
        {
            glDeleteFramebuffers( 1, &_defaultFramebuffer );
            _defaultFramebuffer = 0;
        }
        
        if ( _colorRenderbuffer ) 
        {
            glDeleteRenderbuffers( 1, &_colorRenderbuffer );
            _colorRenderbuffer = 0;
        }
    }
}

-( void )setFramebuffer
{
    if ( context ) 
    {
        [ EAGLContext setCurrentContext:context ];
        
        if ( !_defaultFramebuffer )
        {
            [ self createFramebuffer ];
        }
        
        glBindFramebuffer( GL_FRAMEBUFFER, _defaultFramebuffer );
        
        glViewport( 0, 0, _framebufferWidth, _framebufferHeight );
    }
}

-( BOOL )presentFramebuffer
{
    BOOL success = FALSE;
    
    if ( context ) 
    {
        [ EAGLContext setCurrentContext:context ];
        
        glBindRenderbuffer( GL_RENDERBUFFER, _colorRenderbuffer );
        
        success = [ context presentRenderbuffer:GL_RENDERBUFFER ];
    }
    
    return success;
}

-( void )layoutSubviews
{
    // The framebuffer will be re-created at the beginning of the next setFramebuffer method call.
    [ self deleteFramebuffer ];
}

@end

#endif
