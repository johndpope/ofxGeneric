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
    string executeJavascript( string javascript );
    void stopLoading();
    void reload();
    
    bool canGoBack();
    void goBack();
    bool canGoForward();
    void goForward();
    
    void setScrollingEnabled( bool enabled );
#if TARGET_OS_IPHONE
    ofPtr< ofxGenericScrollView > getScrollContainer();
#endif

    virtual bool shouldStartNavigation( string url );
    virtual void started();
    virtual void finished();
    virtual void failed();
    
    virtual ~ofxGenericWebView();
    
    static string dumpCookies();
    
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
    
    virtual bool webView_shouldStartNavigation( ofPtr< ofxGenericWebView > view, string url ) { return true; };
    virtual void webView_started( ofPtr< ofxGenericWebView > view ) {};
    virtual void webView_finished( ofPtr< ofxGenericWebView > view ) {};
    virtual void webView_failed( ofPtr< ofxGenericWebView > view ) {};
};