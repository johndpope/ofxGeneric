//
//  ofxGenericAlertView.cpp
//  ofxGeneric
//
//  Created by Ian Grossberg on 4/18/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericAlertView.h"
#include "ofxGenericUtility.h"

#if TARGET_OS_IPHONE
@interface ofxGenericAlertViewForwarder : NSObject< UIAlertViewDelegate >
{
    ofPtrWeak< ofxGenericAlertView > _forwardTo;
}
-( id )initWithForwardTo:( ofPtrWeak< ofxGenericAlertView > )forwardTo;

@end

#endif

ofPtr< ofxGenericAlertView > ofxGenericAlertView::create( string title, string message, ofPtrWeak< ofxGenericAlertViewDelegate > delegate )
{
    ofPtr< ofxGenericAlertView > create( new ofxGenericAlertView() );
    create->init( create, title, message, delegate );
    return create;
}

ofxGenericAlertView::ofxGenericAlertView()
{
    
}

void ofxGenericAlertView::init( ofPtrWeak< ofxGenericAlertView > setThis, string title, string message, ofPtrWeak< ofxGenericAlertViewDelegate > delegate )
{
    _this = setThis;
    _delegate = delegate;
    
    createNativeView();
    setTitle( title );
    setMessage( message );
}

void ofxGenericAlertView::createNativeView()
{
#if TARGET_OS_IPHONE
    _forwarder = [ [ ofxGenericAlertViewForwarder alloc ] initWithForwardTo:_this ];
    _view = [ [ UIAlertView alloc ] initWithTitle:@"" message:@"" delegate:_forwarder cancelButtonTitle:@"Ok" otherButtonTitles:nil ];
#endif
}

ofxGenericAlertView::~ofxGenericAlertView()
{
#if TARGET_OS_IPHONE
    _view.delegate = nil;
    [ _view release ];
    _view = nil;
    
    [ _forwarder release ];
    _forwarder = nil;
#endif
}

void ofxGenericAlertView::setTitle( string title )
{
#if TARGET_OS_IPHONE
    if ( _view )
    {
        [ _view setTitle:ofxStringToNSString( title ) ];
    }
#endif
}

void ofxGenericAlertView::setMessage( string message )
{
#if TARGET_OS_IPHONE
    if ( _view )
    {
        [ _view setMessage:ofxStringToNSString( message ) ];
    }
#endif
}

void ofxGenericAlertView::show()
{
#if TARGET_OS_IPHONE
    if ( _view )
    {
        [ _view show ];
    }
#endif
}

void ofxGenericAlertView::clickedButtonAtIndex( int buttonIndex )
{
    if ( _delegate )
    {
        _delegate.lock()->alertView_clickedButtonAtIndex( buttonIndex );
    }
}

void ofxGenericAlertView::cancelled()
{
    if ( _delegate )
    {
        _delegate.lock()->alertView_cancelled();
    }
}

#if TARGET_OS_IPHONE

@implementation ofxGenericAlertViewForwarder

-( id )initWithForwardTo:( ofPtrWeak< ofxGenericAlertView > ) forwardTo;
{
    self = [ super init ];
    if ( self )
    {
        _forwardTo = forwardTo;
    }
    return self;
}

// Called when a button is clicked. The view will be automatically dismissed after this call returns
- (void)alertView:( UIAlertView* )alertView clickedButtonAtIndex:( NSInteger )buttonIndex
{
    if ( _forwardTo )
    {
        _forwardTo.lock()->clickedButtonAtIndex( ( int )buttonIndex );
    }
}

// Called when we cancel a view (eg. the user clicks the Home button). This is not called when the user clicks the cancel button.
// If not defined in the delegate, we simulate a click in the cancel button
-( void )alertViewCancel:( UIAlertView* )alertView
{    
    if ( _forwardTo )
    {
        _forwardTo.lock()->cancelled();
    }
}

-( void )willPresentAlertView:( UIAlertView* )alertView  // before animation and showing view
{
    if ( _forwardTo )
    {
        
    }
   
}

-( void )didPresentAlertView:( UIAlertView* )alertView  // after animation
{
    if ( _forwardTo )
    {
        
    }
    
}

-( void )alertView:( UIAlertView* )alertView willDismissWithButtonIndex:( NSInteger )buttonIndex // before animation and hiding view
{
    if ( _forwardTo )
    {
        
    }
    
}

-( void )alertView:( UIAlertView* )alertView didDismissWithButtonIndex:( NSInteger )buttonIndex  // after animation
{
    if ( _forwardTo )
    {
        
    }
    
}

// Called after edits in any of the default fields added by the style
-( BOOL )alertViewShouldEnableFirstOtherButton:( UIAlertView* )alertView
{
    if ( _forwardTo )
    {
        
    }
    return YES;
}

@end
#endif