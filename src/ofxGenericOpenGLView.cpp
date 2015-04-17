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

// IOSP-40 // #include "ofGraphics.h"

#if TARGET_OS_IPHONE
#import <QuartzCore/QuartzCore.h>

#import "ofCommon.h"

@class EAGLContext;

@interface ofxGenericOpenGLViewNative : UIView
{
    ofPtrWeak< ofxGenericOpenGLView > _ownerView;
}
-( id )initWithFrame:( CGRect )frame owner:( ofPtrWeak< ofxGenericOpenGLView > )ownerView;
-( void )setRenderbufferStorageOnContext:( EAGLContext* )context;

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
    
    _framebufferWidth = ( GLint )setFrame.width;
    _framebufferHeight = ( GLint )setFrame.height;
}
                 
ofxGenericOpenGLView::ofxGenericOpenGLView()
#if TARGET_OS_IPHONE
: _context( nil ),
#endif
_framebufferWidth( 0 ), _framebufferHeight( 0 ), _defaultFramebuffer( 0 ), _colorRenderbuffer( 0 )
{    
}

ofxGenericOpenGLView::~ofxGenericOpenGLView()
{
#if TARGET_OS_IPHONE
    setContext( nil );
#endif
    deleteFramebuffer();
}

NativeView ofxGenericOpenGLView::createNativeView( const ofRectangle& frame )
{
#if TARGET_OS_IPHONE
    ofxGenericOpenGLViewNative* view = [ [ ofxGenericOpenGLViewNative alloc ] initWithFrame:ofxRectangleToCGRect( frame ) ];
    return view;
#elif TARGET_ANDROID
#endif
}

void ofxGenericOpenGLView::didLoad()
{
    // IOSP-40 // ofGetCurrentRenderer()->setBackgroundAuto( true );
    // IOSP-40 // ofGetCurrentRenderer()->background( ofColor( 0, 0, 0, 255 ) );

#if TARGET_OS_IPHONE
    // TODO: break into ES1 and ES2 classes?
    _context = [ [ [ EAGLContext alloc ] initWithAPI:kEAGLRenderingAPIOpenGLES1 ] autorelease ];

    if ( !_context )
    {
        // TODO: exception
        ofxGLogFatalError( "Could not create OpenGL ES context" );
    }
    
    if ( ![ EAGLContext setCurrentContext:_context ] )
    {
        // TODO: exception
        ofxGLogFatalError( "Could not set OpenGL ES context" );
    }
    
    setFramebuffer(); 
#endif
}

void ofxGenericOpenGLView::willAppear()
{
    ofxGenericView::willAppear();
    // IOSP-40 // ofAddListener( ofEvents().update, this, &ofxGenericOpenGLView::update );
}

void ofxGenericOpenGLView::didDisappear()
{
    ofxGenericView::didDisappear();
    // IOSP-40 // ofRemoveListener( ofEvents().update, this, &ofxGenericOpenGLView::update );
}

// IOSP-40 // void ofxGenericOpenGLView::update( ofEventArgs &args )
// IOSP-40 // {
// IOSP-40 //     setFramebuffer();
// IOSP-40 //
// IOSP-40 //     if ( ofGetCurrentRenderer()->bClearBg() )
// IOSP-40 //     {
// IOSP-40 //         ofFloatColor clearColor = ofGetCurrentRenderer()->getBgColor();
// IOSP-40 // 		glClearColor( clearColor.r, clearColor.g, clearColor.b, clearColor.a );
// IOSP-40 //
// IOSP-40 //         GLbitfield clear = 0;
// IOSP-40 //         if ( _colorRenderbuffer )
// IOSP-40 //         {
// IOSP-40 //             clear |= GL_COLOR_BUFFER_BIT;
// IOSP-40 //         }
// IOSP-40 // /*        if ( _depthRenderbuffer )
// IOSP-40 //         {
// IOSP-40 //             clear |= GL_DEPTH_BUFFER_BIT;
// IOSP-40 //         }
// IOSP-40 //  */
// IOSP-40 // 		glClear( clear );
// IOSP-40 // 	}
// IOSP-40 //
// IOSP-40 //     drawFrame();
// IOSP-40 //
// IOSP-40 //     presentFramebuffer();
// IOSP-40 // }

void ofxGenericOpenGLView::drawFrame()
{
    
}

ofColor ofxGenericOpenGLView::getBackgroundColor()
{
    /* TZLA-619 // ofFloatColor backgroundColor = ofGetCurrentRenderer()->getBgColor();
    ofColor result( ( int )( backgroundColor.r * 255 ), ( int )( backgroundColor.g * 255 ), ( int )( backgroundColor.b * 255 ), ( int )( backgroundColor.a * 255 ) );
    return result;*/
    return 0;
}

void ofxGenericOpenGLView::setBackgroundColor( const ofColor& setColor )
{
    // IOSP-40 // ofGetCurrentRenderer()->background( setColor );
}

#if TARGET_OS_IPHONE    
void ofxGenericOpenGLView::setContext( EAGLContext* newContext )
{
    if ( _context != newContext ) 
    {
        [ _context release ];
        _context = [ newContext retain ];
        
        [ EAGLContext setCurrentContext:nil ];
    }
}
#endif

void ofxGenericOpenGLView::createFramebuffer()
{
    if ( _context && !_defaultFramebuffer ) 
    {
#if TARGET_OS_IPHONE
        [ EAGLContext setCurrentContext:_context ];
#endif
        
        // Create default framebuffer object.
        glGenFramebuffers( 1, &_defaultFramebuffer );
        glBindFramebuffer( GL_FRAMEBUFFER, _defaultFramebuffer );
        
        // Create color render buffer and allocate backing store.
        glGenRenderbuffers( 1, &_colorRenderbuffer );
        glBindRenderbuffer( GL_RENDERBUFFER, _colorRenderbuffer );
#if TARGET_OS_IPHONE
        if ( [ _view isKindOfClass:[ ofxGenericOpenGLViewNative class ] ] )
        {
            ofxGenericOpenGLViewNative* nativeView = ( ofxGenericOpenGLViewNative* )_view;
            [ nativeView setRenderbufferStorageOnContext:_context ];
        }
#endif
        glGetRenderbufferParameteriv( GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &_framebufferWidth );
        glGetRenderbufferParameteriv( GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &_framebufferHeight );    
        
        glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _colorRenderbuffer);
        
        if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
        {
            ofxGLogFatalError( "Failed to make complete framebuffer object" );
        }
    }
}

void ofxGenericOpenGLView::setFramebuffer()
{
    if ( _context ) 
    {
#if TARGET_OS_IPHONE
        [ EAGLContext setCurrentContext:_context ];
#endif
        
        if ( !_defaultFramebuffer )
        {
            createFramebuffer();
        }
        
        glBindFramebuffer( GL_FRAMEBUFFER, _defaultFramebuffer );
        
        glViewport( 0, 0, _framebufferWidth, _framebufferHeight );
    }
}

bool ofxGenericOpenGLView::presentFramebuffer()
{
    bool success = false;
    
    if ( _context ) 
    {
#if TARGET_OS_IPHONE
        [ EAGLContext setCurrentContext:_context ];
#endif
        
        glBindRenderbuffer( GL_RENDERBUFFER, _colorRenderbuffer );
        
#if TARGET_OS_IPHONE
        success = [ _context presentRenderbuffer:GL_RENDERBUFFER ];
#endif
    }
    
    return success;
}

void ofxGenericOpenGLView::deleteFramebuffer()
{
    if ( _context ) 
    {
#if TARGET_OS_IPHONE
        [ EAGLContext setCurrentContext:nil ];
#endif        
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



#if TARGET_OS_IPHONE

@implementation ofxGenericOpenGLViewNative

+( Class )layerClass
{
    return [ CAEAGLLayer class ];
}

-( id )initWithFrame:( CGRect )frame owner:( ofPtrWeak< ofxGenericOpenGLView > )ownerView
{
    self = [ super initWithFrame:frame ];
	if ( self ) 
    {
        CAEAGLLayer* eaglLayer = ( CAEAGLLayer* )self.layer;
        
        // TODO: configurable
        BOOL retainBacking = NO;
        BOOL isOpaque = YES;
        NSString* colorFormat = kEAGLColorFormatRGBA8;
        
        eaglLayer.opaque = isOpaque;
        eaglLayer.drawableProperties = [ NSDictionary dictionaryWithObjectsAndKeys: [ NSNumber numberWithBool:retainBacking ], kEAGLDrawablePropertyRetainedBacking,
                                         colorFormat, kEAGLDrawablePropertyColorFormat, nil ];
        _ownerView = ownerView;
    }
    
    return self;
}

-( void )setRenderbufferStorageOnContext:( EAGLContext* )context
{
    [ context renderbufferStorage:GL_RENDERBUFFER fromDrawable:( CAEAGLLayer* )self.layer ];
}

-( void )layoutSubviews
{
    if ( _ownerView )
    {
        _ownerView.lock()->deleteFramebuffer();
    }
}

@end

#endif
