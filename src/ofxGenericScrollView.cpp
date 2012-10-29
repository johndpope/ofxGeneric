//
//  ofxGenericScrollView.cpp
//  ofxGeneric
//
//  Created by Ian Grossberg on 3/14/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericScrollView.h"
#include "ofxGenericUtility.h"

#if TARGET_OS_IPHONE

@interface ofxGenericScrollViewForwarder : NSObject <UIScrollViewDelegate>
{
    ofPtrWeak< ofxGenericScrollView > _delegate;
}
- ( id )initWithDelegate:( ofPtrWeak< ofxGenericScrollView > )delegate;
- (void)scrollViewDidScroll:(UIScrollView *)scrollView;
//other UIScrollViewDelegate functions can be put in here if desired

@end

#endif

ofxGenericScrollView::ofxGenericScrollView()
:
#if TARGET_OS_IPHONE
 _forwarder ( nil ),
#endif
 _autoContentSizeToFit( false )
{
}

ofxGenericScrollView::~ofxGenericScrollView()
{
#if TARGET_OS_IPHONE
    if ( _forwarder )
    {
        [ _forwarder release ];
        _forwarder = nil;
    }
#endif
}

ofPtr< ofxGenericScrollView > ofxGenericScrollView::create( const ofRectangle& setFrame, NativeView nativeView )
{
    ofPtr< ofxGenericScrollView > create = ofPtr< ofxGenericScrollView >( new ofxGenericScrollView() );
    create->init( create, setFrame, nativeView );
    return create;
}

NativeView ofxGenericScrollView::createNativeView( const ofRectangle& frame )
{
#if TARGET_OS_IPHONE
    UIScrollView* newView = [ [ UIScrollView alloc ] initWithFrame:ofRectangleToCGRect( frame ) ];
    [ newView setBackgroundColor:[ UIColor clearColor ] ];
    _forwarder = [ [ ofxGenericScrollViewForwarder alloc ] initWithDelegate:dynamic_pointer_cast< ofxGenericScrollView >( _this ) ];
    newView.delegate = _forwarder;
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

void ofxGenericScrollView::setAutoContentSizeToFit( bool autosize )
{
    _autoContentSizeToFit = autosize;
}

void ofxGenericScrollView::addChildView( ofPtr< ofxGenericView > add )
{
    ofxGenericView::addChildView( add );
    
    if ( _autoContentSizeToFit && add )
    {
        ofRectangle addFrame = add->getFrame();
        ofRectangle contentFrame = getContentFrame();
        contentFrame.growToFit( addFrame );
        setContentFrame( contentFrame );
    }
}

void ofxGenericScrollView::sizeToFitContents()
{
    ofRectangle originalContentFrame;
    ofRectangle contentFrame = originalContentFrame = getContentFrame();
    for( std::list< ofPtr< ofxGenericView > >::iterator travChildren = _children.begin(); travChildren != _children.end(); travChildren ++ )
    {
        if ( *travChildren )
        {
            contentFrame.growToFit( ( *travChildren )->getFrame() );
        }
    }
    if ( contentFrame != originalContentFrame )
    {
        setContentFrame( contentFrame );
    }
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

void ofxGenericScrollView::setContentFrame( const ofRectangle& contentFrame, bool animated )
{
    setContentOffset( ofPoint( contentFrame.x, contentFrame.y ), animated );
    setContentSize( ofPoint( contentFrame.width, contentFrame.height ) );
}

ofRectangle ofxGenericScrollView::getContentFrame()
{
    ofPoint contentOffset = getContentOffset();
    ofPoint contentSize = getContentSize();
    return ofRectangle( contentOffset.x, contentOffset.y, contentSize.x, contentSize.y );
}

ofPoint ofxGenericScrollView::getContentOffset()
{
#if TARGET_OS_IPHONE
    UIScrollView* view = *this;
    if ( view )
    {
        return ofPoint( view.contentOffset.x, view.contentOffset.y );
    }
#elif TARGET_ANDROID
#endif
    return ofPoint( 0, 0 );
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

void ofxGenericScrollView::setBounceEnabled( bool enabled )
{
#if TARGET_OS_IPHONE
    UIScrollView* view = *this;
    if ( view )
    {
        view.bounces = ( BOOL )enabled;
    }
#elif TARGET_ANDROID
#endif
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

void ofxGenericScrollView::setDelegate( ofPtrWeak< ofxGenericScrollViewDelegate > delegate )
{
    _delegate = delegate;
}

void ofxGenericScrollView::didScroll()
{
    if ( _delegate )
    {
        _delegate.lock()->scrollView_didScroll( dynamic_pointer_cast< ofxGenericScrollView >( _this ) );
    }
}

#if TARGET_OS_IPHONE

ofxGenericUIViewCastOperator( ofxGenericScrollView, UIScrollView );

@implementation ofxGenericScrollViewForwarder

-( id )initWithDelegate:( ofPtrWeak< ofxGenericScrollView > )delegate
{
    self = [ super init ];
    if ( self )
    {
        _delegate = delegate;
    }
    return self;
}

- (void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    if ( _delegate )
    {
        _delegate.lock()->didScroll();
    }
}

@end

#endif