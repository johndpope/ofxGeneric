//
//  ofxGenericWebView.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 9/24/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericView.h"

class ofxGenericWebViewDelegate;
#if TARGET_OS_IPHONE
class ofxGenericScrollView;
@class ofxGenericWebViewForwarder;
#endif

class ofxGenericWebView : public ofxGenericView
{
public:
    static ofPtr< ofxGenericWebView > create( const ofRectangle& setFrame = ofRectangle() );
    
    void setDelegate( ofPtrWeak< ofxGenericWebViewDelegate > delegate );
    
    void goToRemoteURL( string URL );
    void goToLocalURL( string URL );
    void loadHTML( string baseURL, string HTML );
    void stopLoading();
    void reload();
    
    bool canGoBack();
    void goBack();
    bool canGoForward();
    void goForward();
    
    void setScrollingEnabled( bool enabled );

    virtual bool shouldStartNavigation();
    virtual void started();
    virtual void finished();
    virtual void failed();
    
    virtual ~ofxGenericWebView();
    
protected:
    ofxGenericWebView();
    ofPtrWeak< ofxGenericWebViewDelegate > _delegate;
#if TARGET_OS_IPHONE
    ofxGenericWebViewForwarder* _forwarder;
#endif
    
    NativeView createNativeView( const ofRectangle& frame );
#if TARGET_OS_IPHONE
    UIWebView* getNativeWebView();
    ofPtr< ofxGenericScrollView > _scrollContainer;
#endif
};

class ofxGenericWebViewDelegate
{
public:
    virtual ~ofxGenericWebViewDelegate() {};
    
    virtual bool webView_shouldStartNavigation( ofPtr< ofxGenericWebView > view ) { return true; };
    virtual void webView_started( ofPtr< ofxGenericWebView > view ) {};
    virtual void webView_finished( ofPtr< ofxGenericWebView > view ) {};
    virtual void webView_failed( ofPtr< ofxGenericWebView > view ) {};
};