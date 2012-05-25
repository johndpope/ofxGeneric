//
//  ofxGenericScrollView.cpp
//  ofxGeneric
//
//  Created by Ian Grossberg on 3/14/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericScrollView.h"
#include "ofxGenericUtility.h"

ofxGenericScrollView::ofxGenericScrollView()
{
}

ofPtr< ofxGenericScrollView > ofxGenericScrollView::create( const ofRectangle& setFrame )
{
    ofPtr< ofxGenericScrollView > create = ofPtr< ofxGenericScrollView >( new ofxGenericScrollView() );
    create->init( create, setFrame );
    return create;
}

NativeView ofxGenericScrollView::createNativeView( const ofRectangle& frame )
{
#if TARGET_OS_IPHONE
    UIScrollView* newView = [ [ UIScrollView alloc ] initWithFrame:ofRectangleToCGRect( frame ) ];
    [ newView setBackgroundColor:[ UIColor clearColor ] ];
    return newView;
#elif TARGET_ANDROID
    return ofxGenericView::createNativeView( frame );
#endif
}

void ofxGenericScrollView::setContentSize( const ofPoint& contentSize )
{
#if TARGET_OS_IPHONE
    UIScrollView* view = ( UIScrollView* )*this;
    if ( view )
    {
        [ view setContentSize:CGSizeMake( contentSize.x, contentSize.y ) ];
    }
#elif TARGET_ANDROID
#endif
}

void ofxGenericScrollView::setContentOffset( const ofPoint& contentOffset, bool animated )
{
#if TARGET_OS_IPHONE
    UIScrollView* view = *this;
    if ( view )
    {
        [ view setContentOffset:CGPointMake( contentOffset.x, contentOffset.y ) animated:( BOOL )animated ];
    }
#elif TARGET_ANDROID
#endif    
}

void ofxGenericScrollView::setEnabled( bool enabled )
{
#if TARGET_OS_IPHONE
    UIScrollView* view = *this;
    if ( view )
    {
        [ view setScrollEnabled:( BOOL )enabled ];
    }
#elif TARGET_ANDROID
#endif
}

void ofxGenericScrollView::setDirectionalLockEnabled( bool enabled )
{
#if TARGET_OS_IPHONE
    UIScrollView* view = *this;
    if ( view )
    {
        [ view setDirectionalLockEnabled:( BOOL )enabled ];
    }
#elif TARGET_ANDROID
#endif
}

void ofxGenericScrollView::setPagingEnabled( bool enabled )
{
#if TARGET_OS_IPHONE
    UIScrollView* view = *this;
    if ( view )
    {
        [ view setPagingEnabled:( BOOL )enabled ];
    }
#elif TARGET_ANDROID
#endif
}

void ofxGenericScrollView::setShowHorizontalScrollIndicator( bool show )
{
#if TARGET_OS_IPHONE
    UIScrollView* view = *this;
    if ( view )
    {
        [ view setShowsHorizontalScrollIndicator:( BOOL )show ];
    }
#elif TARGET_ANDROID
#endif
}

void ofxGenericScrollView::setShowVerticalScrollIndicator( bool show )
{
#if TARGET_OS_IPHONE
    UIScrollView* view = *this;
    if ( view )
    {
        [ view setShowsVerticalScrollIndicator:( BOOL )show ];
    } 
#elif TARGET_ANDROID
#endif
}

void ofxGenericScrollView::scrollRectToVisible( const ofRectangle& rect, bool animated )
{
#if TARGET_OS_IPHONE
    UIScrollView* view = *this;
    if ( view )
    {
        [ view scrollRectToVisible:CGRectMake(rect.x, rect.y, rect.width, rect.height) animated:(animated ? YES : NO) ];
    } 
#elif TARGET_ANDROID
#endif
}

ofPoint ofxGenericScrollView::getContentSize()
{
#if TARGET_OS_IPHONE
    UIScrollView* view = *this;
    if ( view )
    {
        CGSize s = view.contentSize;
        return ofPoint( s.width, s.height );
    } 
#elif TARGET_ANDROID
#endif  
    return ofPoint(0,0);
}

void ofxGenericScrollView::setAlwaysBouncesVertical( bool bounce )
{
#if TARGET_OS_IPHONE
    UIScrollView* view = *this;
    if ( view )
    {
        view.alwaysBounceVertical = bounce;
    } 
#elif TARGET_ANDROID
#endif
}

void ofxGenericScrollView::setAlwaysBouncesHorizontal( bool bounce )
{
#if TARGET_OS_IPHONE
    UIScrollView* view = *this;
    if ( view )
    {
        view.alwaysBounceHorizontal = bounce;
    } 
#elif TARGET_ANDROID
#endif
}

#if TARGET_OS_IPHONE

ofxGenericUIViewCastOperator( ofxGenericScrollView, UIScrollView );

#endif