//
//  ofxGenericWebView.cpp
//  ofxGeneric
//
//  Created by Ian Grossberg on 9/24/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericWebView.h"

#include "ofxGenericScrollView.h"

#import "ofCommon.h"

#if TARGET_OS_IPHONE
@interface ofxGenericWebViewForwarder : NSObject< UIWebViewDelegate >
{
    ofPtrWeak< ofxGenericWebView > _forwardTo;
}
-( id )initWithForwardTo:( ofPtrWeak< ofxGenericWebView > )forwardTo;

@end
#endif

ofPtr< ofxGenericWebView > ofxGenericWebView::create( const ofRectangle& setFrame )
{
    ofPtr< ofxGenericWebView > create( new ofxGenericWebView() );
    create->init( create, setFrame );
    return create;
}

ofxGenericWebView::ofxGenericWebView()
{
}

ofxGenericWebView::~ofxGenericWebView()
{
#if TARGET_OS_IPHONE
    [ _forwarder release ];
#endif
}

NativeView ofxGenericWebView::createNativeView( const ofRectangle& frame )
{
#if TARGET_OS_IPHONE
    UIWebView* newView = [ [ UIWebView alloc ] initWithFrame:ofxRectangleToCGRect( frame ) ];
    
    if ( [ newView respondsToSelector:@selector( scrollView ) ] )
    {
        _scrollContainer = ofxGenericScrollView::create( frame, [ newView scrollView ] );
    }
    
    _forwarder = [ [ ofxGenericWebViewForwarder alloc ] initWithForwardTo:dynamic_pointer_cast< ofxGenericWebView >( _this ) ];
    [ newView setDelegate:_forwarder ];
    
    return newView;
#endif
}

UIWebView* ofxGenericWebView::getNativeWebView()
{
    if ( [ getNativeView() isKindOfClass:[ UIWebView class ] ] )
    {
        return ( UIWebView* )getNativeView();
    }
    return nil;
}

void ofxGenericWebView::setDelegate( ofPtrWeak< ofxGenericWebViewDelegate > delegate )
{
    _delegate = delegate;
}

void ofxGenericWebView::goToRemoteURL( string URL )
{
#if TARGET_OS_IPHONE
    UIWebView* webView = getNativeWebView();
    if ( webView )
    {
        NSURLRequest* request = [ NSURLRequest requestWithURL:[ NSURL URLWithString:ofxStringToNSString( URL ) ] ];
        [ webView loadRequest:request ];
    }
#endif
}

void ofxGenericWebView::goToLocalURL( string URL )
{
#if TARGET_OS_IPHONE
    UIWebView* webView = getNativeWebView();
    if ( webView )
    {
        NSURLRequest* request = [ NSURLRequest requestWithURL:[ NSURL fileURLWithPath:ofxStringToNSString( URL ) ] ];
        [ webView loadRequest:request ];
    }
#endif
}

void ofxGenericWebView::loadHTML( string baseURL, string HTML )
{
#if TARGET_OS_IPHONE
    UIWebView* webView = getNativeWebView();
    if ( webView )
    {
        [ webView loadHTMLString:ofxStringToNSString( HTML ) baseURL:[ NSURL URLWithString:ofxStringToNSString( baseURL ) ] ];
    }
#endif
}

string ofxGenericWebView::executeJavascript( string javascript )
{
#if TARGET_OS_IPHONE
    UIWebView* webView = getNativeWebView();
    if ( webView )
    {
        return ofxNSStringToString( [ webView stringByEvaluatingJavaScriptFromString: ofxStringToNSString( javascript ) ] );
    }
#endif
    return "";
}

void ofxGenericWebView::stopLoading()
{
#if TARGET_OS_IPHONE
    UIWebView* webView = getNativeWebView();
    if ( webView )
    {
        [ webView stopLoading ];
    }
#endif
}

void ofxGenericWebView::reload()
{
#if TARGET_OS_IPHONE
    UIWebView* webView = getNativeWebView();
    if ( webView )
    {
        [ webView reload ];
    }
#endif
}

bool ofxGenericWebView::canGoBack()
{
#if TARGET_OS_IPHONE
    UIWebView* webView = getNativeWebView();
    if ( webView )
    {
        return ( bool )[ webView canGoBack ];
    }
#endif
    return false;
}

void ofxGenericWebView::goBack()
{
#if TARGET_OS_IPHONE
    UIWebView* webView = getNativeWebView();
    if ( webView )
    {
        [ webView goBack ];
    }
#endif
}

bool ofxGenericWebView::canGoForward()
{
#if TARGET_OS_IPHONE
    UIWebView* webView = getNativeWebView();
    if ( webView )
    {
        return ( bool )[ webView canGoForward ];
    }
#endif
    return false;
}

void ofxGenericWebView::goForward()
{
#if TARGET_OS_IPHONE
    UIWebView* webView = getNativeWebView();
    if ( webView )
    {
        [ webView goForward ];
    }
#endif
}

void ofxGenericWebView::setScrollingEnabled( bool enabled )
{
#if TARGET_OS_IPHONE
    if ( _scrollContainer )
    {
        _scrollContainer->setEnabled( enabled );
    }
#endif
}

void ofxGenericWebView::setScalesPageToFit(bool scales)
{
#if TARGET_OS_IPHONE
    UIWebView *webView = getNativeWebView();
    if (webView) {
        webView.scalesPageToFit = (BOOL)scales;
    }
#endif
}

#if TARGET_OS_IPHONE
ofPtr< ofxGenericScrollView > ofxGenericWebView::getScrollContainer()
{
    if ( !_scrollContainer )
    {
        ofxGLogVerbose( "Internal Scroll Container is NULL, older versions of iOS do not support access" );
    }
    return _scrollContainer;
}
#endif

bool ofxGenericWebView::shouldStartNavigation( string url )
{
    ofPtr< ofxGenericWebViewDelegate > delegate = _delegate.lock();
    if ( delegate )
    {
        return delegate->webView_shouldStartNavigation( dynamic_pointer_cast< ofxGenericWebView >( _this ), url );
    }
    return true;
}

void ofxGenericWebView::started()
{
    ofPtr< ofxGenericWebViewDelegate > delegate = _delegate.lock();
    if ( delegate )
    {
        delegate->webView_started( dynamic_pointer_cast< ofxGenericWebView >( _this ) );
    }
}

void ofxGenericWebView::finished()
{
    ofPtr< ofxGenericWebViewDelegate > delegate = _delegate.lock();
    if ( delegate )
    {
        delegate->webView_finished( dynamic_pointer_cast< ofxGenericWebView >( _this ) );
    }
}

void ofxGenericWebView::failed()
{
    ofxGLogError( "ofxGenericWebview Navigation failed!" );
    ofPtr< ofxGenericWebViewDelegate > delegate = _delegate.lock();
    if ( delegate )
    {
        delegate->webView_failed( dynamic_pointer_cast< ofxGenericWebView >( _this ) );
    }
}

string ofxGenericWebView::dumpCookies()
{
    string result;
#if TARGET_OS_IPHONE
    NSArray* cookies = [ [ NSHTTPCookieStorage sharedHTTPCookieStorage ] cookies ];
    for( NSHTTPCookie* cookie in cookies )
    {
        NSString* dumpCookie = [ NSString stringWithFormat:@"%@", cookie ];
        result += ofxNSStringToString( dumpCookie ) + "\n";
    }
#endif
    return result;
}

#if TARGET_OS_IPHONE
@implementation ofxGenericWebViewForwarder

-( id )initWithForwardTo:( ofPtrWeak< ofxGenericWebView > )forwardTo;
{
    self = [ super init ];
    if ( self )
    {
        _forwardTo = forwardTo;
    }
    return self;
}

-( BOOL )webView:( UIWebView* )webView shouldStartLoadWithRequest:( NSURLRequest* )request navigationType:( UIWebViewNavigationType )navigationType
{
    ofPtr< ofxGenericWebView > forwardTo = _forwardTo.lock();
    if ( forwardTo )
    {
        return ( BOOL )forwardTo->shouldStartNavigation( ofxNSStringToString( [ [ request URL ] relativeString ] ) );
    }
    return YES;
}

-( void )webViewDidStartLoad:( UIWebView* )webView
{
    ofPtr< ofxGenericWebView > forwardTo = _forwardTo.lock();
    if ( forwardTo )
    {
        forwardTo->started();
    }
}

-( void )webViewDidFinishLoad:( UIWebView* )webView
{
    ofPtr< ofxGenericWebView > forwardTo = _forwardTo.lock();
    if ( forwardTo )
    {
        forwardTo->finished();
    }
}

-( void )webView:( UIWebView* )webView didFailLoadWithError:( NSError* )error
{
    if ( [ error code ] != NSURLErrorCancelled ) // throws an error when we send a request before the first is finished
    {
        ofPtr< ofxGenericWebView > forwardTo = _forwardTo.lock();
        if ( forwardTo )
        {
            forwardTo->failed();
        }
    }
}


@end
#endif
