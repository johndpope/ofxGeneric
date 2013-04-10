//
//  ofxGenericActivityView.cpp
//  ofxGeneric
//
//  Created by Ian Grossberg on 3/9/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericActivityView.h"
#include "ofxGenericUtility.h"

ofPtr< ofxGenericActivityView > ofxGenericActivityView::create( const ofRectangle& setFrame )
{
    ofPtr< ofxGenericActivityView > create = ofPtr< ofxGenericActivityView >( new ofxGenericActivityView() );
    create->init( create, setFrame );
    return create;
}

NativeView ofxGenericActivityView::createNativeView( const ofRectangle& frame )
{
#if TARGET_OS_IPHONE
    UIActivityIndicatorView* newView = [ [ UIActivityIndicatorView alloc ] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge ];
    [ newView setFrame:ofxRectangleToCGRect( frame ) ];
    return newView;
#endif
}

void ofxGenericActivityView::startAnimating()
{
#if TARGET_OS_IPHONE
    UIActivityIndicatorView* nativeView = ( UIActivityIndicatorView* )*this;
    if ( nativeView )
    {
        [ nativeView startAnimating ];
    }
#endif
}

void ofxGenericActivityView::stopAnimating()
{
#if TARGET_OS_IPHONE
    UIActivityIndicatorView* nativeView = ( UIActivityIndicatorView* )*this;
    if ( nativeView )
    {
        [ nativeView stopAnimating ];
    }
#endif
}

bool ofxGenericActivityView::isAnimating()
{
#if TARGET_OS_IPHONE
    UIActivityIndicatorView* nativeView = ( UIActivityIndicatorView* )*this;
    if ( nativeView )
    {
        return ( bool )nativeView.isAnimating;
    }
#endif
    return false;
}

void ofxGenericActivityView::setHidesWhenStoppedAnimating( bool hide )
{
#if TARGET_OS_IPHONE
    UIActivityIndicatorView* nativeView = ( UIActivityIndicatorView* )*this;
    if ( nativeView )
    {
        [ nativeView setHidesWhenStopped:( BOOL )hide ];
    }
#endif
}

bool ofxGenericActivityView::hidesWhenStoppedAnimating()
{
#if TARGET_OS_IPHONE
    UIActivityIndicatorView* nativeView = ( UIActivityIndicatorView* )*this;
    if ( nativeView )
    {
        return [ nativeView hidesWhenStopped ];
    }
#endif
    return false;
}

void ofxGenericActivityView::setColor( const ofColor& color )
{
#if TARGET_OS_IPHONE
    UIActivityIndicatorView* nativeView = ( UIActivityIndicatorView* )*this;
    if ( nativeView )
    {
         if ( [ nativeView respondsToSelector:@selector(setColor:) ] ) // not available in iOS < 5.0
         {
             [ nativeView setColor:ofxColorToUIColor( color ) ];
         }
    }
#endif    
}

ofColor ofxGenericActivityView::getColor()
{
#if TARGET_OS_IPHONE
    UIActivityIndicatorView* nativeView = ( UIActivityIndicatorView* )*this;
    if ( nativeView )
    {
        if ( [ nativeView respondsToSelector:@selector(setColor:) ] ) // not available in iOS < 5.0
        {
            return ofxUIColorToofColor( nativeView.color );
        }
    }
#endif
    return ofColor( 255, 255, 255, 255 );
}

void ofxGenericActivityView::setIndicatorViewStyle( ofxGenericActivityViewStyle style )
{
#if TARGET_OS_IPHONE
    UIActivityIndicatorView* nativeView = ( UIActivityIndicatorView* )*this;
    if ( nativeView )
    {
        [ nativeView setActivityIndicatorViewStyle:ofxGenericActivityViewStyleToiOS(style) ];
    }
#endif
}


ofxGenericActivityViewStyle ofxGenericActivityView::getIndicatorViewStyle()
{
#if TARGET_OS_IPHONE
    UIActivityIndicatorView* nativeView = ( UIActivityIndicatorView* )*this;
    if ( nativeView )
    {
        return iOSToofxGenericActivityViewStyle(nativeView.activityIndicatorViewStyle);
    }
#endif
    return ofxGenericActivityViewStyleWhite;
}

#if TARGET_OS_IPHONE

ofxGenericUIViewCastOperator( ofxGenericActivityView, UIActivityIndicatorView );

#endif
