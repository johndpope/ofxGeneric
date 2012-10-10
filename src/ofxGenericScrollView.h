//
//  ofxGenericScrollView.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 3/14/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericView.h"

class ofxGenericScrollViewDelegate;

#if TARGET_OS_IPHONE
@class ofxGenericScrollViewForwarder;
#endif

class ofxGenericScrollView : public ofxGenericView
{
public:
    ~ofxGenericScrollView();
    static ofPtr< ofxGenericScrollView > create( const ofRectangle& setFrame = ofRectangle(), NativeView nativeView = NativeNull );
    
    void setContentSize( const ofPoint& contentSize );
    void setContentOffset( const ofPoint& contentOffset, bool animated );
    void setEnabled( bool enabled );
    
    void setDirectionalLockEnabled( bool enabled );
    void setPagingEnabled( bool enabled );
    
    void setShowHorizontalScrollIndicator( bool show );
    void setShowVerticalScrollIndicator( bool show );
    
    void setBounceEnabled( bool enabled );
    
    void setAlwaysBouncesVertical( bool bounce );
    void setAlwaysBouncesHorizontal( bool bounce );
    
    void scrollRectToVisible( const ofRectangle& rect, bool animated );
    
    ofPoint getContentSize();
    ofPoint getContentOffset();
    
    void setDelegate( ofPtrWeak< ofxGenericScrollViewDelegate > delegate );
    
    void didScroll();
    
#if TARGET_OS_IPHONE
    operator UIScrollView*();
    ofxGenericScrollViewForwarder* _forwarder;
#endif
    
protected:
    ofxGenericScrollView();
    virtual NativeView createNativeView( const ofRectangle& frame );
    
    ofPtrWeak< ofxGenericScrollViewDelegate > _delegate;
};

class ofxGenericScrollViewDelegate
{
public:
    virtual ~ofxGenericScrollViewDelegate() {};
    
    virtual void scrollView_didScroll( ofPtr< ofxGenericScrollView > scrollView ) {};
};