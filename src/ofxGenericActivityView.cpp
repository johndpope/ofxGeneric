//
//  ofxGenericActivityView.cpp
//  ofxGeneric
//
//  Created by Ian Grossberg on 3/9/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericActivityView.h"
#include "ofxGenericUtility.h"

NativeView ofxGenericActivityView::createNativeView( const ofRectangle& frame )
{
#if TARGET_OS_IPHONE
    UIActivityIndicatorView* newView = [ [ UIActivityIndicatorView alloc ] initWithFrame:ofRectangleToCGRect( frame ) ];
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
        nativeView.color = ofColorToUIColor( color );
    }
#endif    
}

ofColor ofxGenericActivityView::getColor()
{
#if TARGET_OS_IPHONE
    UIActivityIndicatorView* nativeView = ( UIActivityIndicatorView* )*this;
    if ( nativeView )
    {
        return UIColorToofColor( nativeView.color );
    }
#endif
    return ofColor( 255, 255, 255, 255 );
}


#if TARGET_OS_IPHONE

ofxGenericUIViewCastOperator( ofxGenericActivityView, UIActivityIndicatorView );

#endif
