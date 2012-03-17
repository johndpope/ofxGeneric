//
//  ofxGenericEditTextView.cpp
//
//  Created by Ian Grossberg on 12/29/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#include "ofxGenericEditTextView.h"
#include "ofxGenericUtility.h"
#include "ofxGenericApp.h"

ofPtr< ofxGenericEditTextView > ofxGenericEditTextView::create( const ofRectangle& setFrame )
{
    ofPtr< ofxGenericEditTextView > create = ofPtr< ofxGenericEditTextView >( new ofxGenericEditTextView() );
    create->init( create, setFrame );
    return create;
}

ofxGenericEditTextView::ofxGenericEditTextView()
: _moveFromUnderKeyboardOnBeginEdit( false )
#if TARGET_OS_IPHONE
, _forwarder( nil )
#endif
{
}

ofxGenericEditTextView::~ofxGenericEditTextView()
{
#if TARGET_OS_IPHONE
    release( _forwarder );
#endif
}

NativeView ofxGenericEditTextView::createNativeView( const ofRectangle& frame )
{
#if TARGET_OS_IPHONE
    UITextField* newView = [ [ UITextField alloc ] initWithFrame:ofRectangleToCGRect( frame ) ];
    _forwarder = [ [ ofxGenericEditTextViewDelegateForwarder alloc ] initWithDelegate:this ];
    [ newView setDelegate:_forwarder ];
    [ newView setBackgroundColor:[ UIColor whiteColor ] ];
    [ newView setTextColor:[ UIColor blackColor ] ];
    return newView;
#endif
}

void ofxGenericEditTextView::setText( string newText )
{
#if TARGET_OS_IPHONE
    UITextField* textField = ( UITextField* )*this;
    if ( textField )
    {
        [ textField setText:ofxStringToNSString( newText ) ];
    }
#endif
}

string ofxGenericEditTextView::getText()
{
#if TARGET_OS_IPHONE
    UITextField* textField = ( UITextField* )*this;
    if ( textField )
    {
        return ofxNSStringToString( [ textField text ] );
    }
#endif
    return string();
}
/*
void ofxGenericEditTextView::setTextAlignment( ofxGenericTextHorizontalAlignment alignment )
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UILabel class ] ] )
    {
        UILabel* labelView = ( UILabel* )_view;
        [ labelView setTextAlignment:ofxGenericTextHorizontalAlignmentToUITextAlignment( alignment ) ];
    }
#endif
}
 */

void ofxGenericEditTextView::setPlaceholderText( string placeholderText )
{
#if TARGET_OS_IPHONE
    UITextField* textField = ( UITextField* )*this;
    if ( textField )
    {
        [ textField setPlaceholder:ofxStringToNSString( placeholderText ) ];
    }
#endif
}

string ofxGenericEditTextView::getPlaceholderText()
{
#if TARGET_OS_IPHONE
    UITextField* textField = ( UITextField* )*this;
    if ( textField )
    {
        return ofxNSStringToString( [ textField placeholder ] );
    }
#endif
    return string();    
}

bool ofxGenericEditTextView::currentlyEditing()
{
#if TARGET_OS_IPHONE
    UITextField* textField = ( UITextField* )*this;
    if ( textField )
    {
        return ( bool )textField.editing;
    }
#endif
    return false;
}

void ofxGenericEditTextView::setClearsOnBeginEditing( bool clear )
{
#if TARGET_OS_IPHONE
    UITextField* textField = ( UITextField* )*this;
    if ( textField )
    {
        [ textField setClearsOnBeginEditing:( BOOL )clear ];
    }
#endif
}

bool ofxGenericEditTextView::clearsOnBeginEditing()
{
#if TARGET_OS_IPHONE
    UITextField* textField = ( UITextField* )this;
    if ( textField )
    {
        return ( bool )textField.clearsOnBeginEditing;
    }
#endif
    return false;
}

void ofxGenericEditTextView::setMoveFromUnderKeyboardOnBeginEdit( bool move )
{
    _moveFromUnderKeyboardOnBeginEdit = move;
}

bool ofxGenericEditTextView::shouldBeginEditing()
{    
    return true;
}

void ofxGenericEditTextView::didBeginEditing()
{    
    if ( _moveFromUnderKeyboardOnBeginEdit )
    {
        ofxGenericApp::getInstance()->setMoveFromUnderKeyboard( _this.lock() );
    }
}

bool ofxGenericEditTextView::shouldEndEditing()
{    
    return true;
}

void ofxGenericEditTextView::didEndEditing()
{
    if ( _moveFromUnderKeyboardOnBeginEdit )
    {
        ofxGenericApp::getInstance()->setMoveFromUnderKeyboard( ofPtr< ofxGenericView >() );
    }
}

bool ofxGenericEditTextView::shouldChangeCharactersInRange( int from, int count, string replacement )
{    
    return true;
}

bool ofxGenericEditTextView::shouldClear()
{    
    return true;
}

bool ofxGenericEditTextView::shouldReturn()
{    
#if TARGET_OS_IPHONE
    // TODO: wrap resigning keyboard separately 
    UITextField* textField = ( UITextField* )*this;
    if ( textField )
    {
        [ textField resignFirstResponder ];
    }
#endif
    return true;
}

void ofxGenericEditTextView::setEnabled( bool enabled )
{
#if TARGET_OS_IPHONE
    UITextField* textField = ( UITextField* )*this;
    if ( textField )
    {
        [ textField setEnabled:( BOOL )enabled ];
    }
#endif
}
         
bool ofxGenericEditTextView::getEnabled()
{
#if TARGET_OS_IPHONE
    UITextField* textField = ( UITextField* )*this;
    if ( textField )
    {
        return textField.enabled;
    }
#endif 
    return false;
}

void ofxGenericEditTextView::setAutoCapitalization(ofxGenericTextAutoCapitalization autoCapitalization )
{
#if TARGET_OS_IPHONE
    UITextField* textField = ( UITextField* )*this;
    if ( textField )
    {
        [ textField setAutocapitalizationType:ofxGenericTextAutoCapitalizationToiOS( autoCapitalization ) ];
    }
#endif 
}

void ofxGenericEditTextView::setAutoCorrection( bool enabled )
{
#if TARGET_OS_IPHONE
    UITextField* textField = ( UITextField* )*this;
    if ( textField )
    {
        UITextAutocorrectionType autoCorrection;
        if ( enabled )
        {
            autoCorrection = UITextAutocorrectionTypeYes;
        } else 
        {
            autoCorrection = UITextAutocorrectionTypeNo;
        }
        [ textField setAutocorrectionType:autoCorrection ];            
    }
#endif 
}

bool ofxGenericEditTextView::getAutoCorrection()
{
#if TARGET_OS_IPHONE
    UITextField* textField = ( UITextField* )*this;
    if ( textField )
    {
        return textField.autocorrectionType == UITextAutocorrectionTypeYes;
    }
#endif
    return false;
}

void ofxGenericEditTextView::setSpellChecking( bool enabled )
{
#if TARGET_OS_IPHONE
    UITextField* textField = ( UITextField* )*this;
    if ( textField )
    {
        if ( enabled )
        {
            [ textField setSpellCheckingType:UITextSpellCheckingTypeYes ];
        } else
        {
            [ textField setSpellCheckingType:UITextSpellCheckingTypeNo ];
        }
    }
#endif 
}

bool ofxGenericEditTextView::getSpellChecking()
{
#if TARGET_OS_IPHONE
    UITextField* textField = ( UITextField* )*this;
    if ( textField )
    {
        return textField.spellCheckingType == UITextSpellCheckingTypeYes;
    }
#endif 
    return false;
}

void ofxGenericEditTextView::setEnableReturnKeyAutomatically( bool enabled )
{
#if TARGET_OS_IPHONE
    UITextField* textField = ( UITextField* )*this;
    if ( textField )
    {
        [ textField setEnablesReturnKeyAutomatically:enabled ];
    }
#endif 
}

bool ofxGenericEditTextView::getEnableReturnKeyAutomatically()
{
#if TARGET_OS_IPHONE
    UITextField* textField = ( UITextField* )*this;
    if ( textField )
    {
        return textField.enablesReturnKeyAutomatically;
    }
#endif 
    return false;
}

void ofxGenericEditTextView::setKeyboard( ofxGenericKeyboardType type )
{
#if TARGET_OS_IPHONE
    UITextField* textField = ( UITextField* )*this;
    if ( textField )
    {
        [ textField setKeyboardType:ofxGenericKeyboardTypeToiOS( type ) ];
    }
#endif 
}

void ofxGenericEditTextView::setKeyboardReturnKey( ofxGenericKeyboardReturnKey key )
{
#if TARGET_OS_IPHONE
    UITextField* textField = ( UITextField* )*this;
    if ( textField )
    {
        [ textField setReturnKeyType:ofxGenericKeyboardReturnKeyToiOS( key ) ];
    }
#endif 
}

     

#if TARGET_OS_IPHONE
ofxGenericUIViewCastOperator( ofxGenericEditTextView, UITextField );
#endif

#if TARGET_OS_IPHONE

@implementation ofxGenericEditTextViewDelegateForwarder

-( id )initWithDelegate:( ofxGenericEditTextView* )delegate
{
    self = [ super init ];
    if ( self )
    {
        _delegate = delegate;
    }
    return self;
}

-( void )dealloc
{
    [ super dealloc ];
    _delegate = NULL;
}

- (BOOL)textFieldShouldBeginEditing:(UITextField *)textField        // return NO to disallow editing.
{
    if ( _delegate )
    {
        return ( BOOL )_delegate->shouldBeginEditing();
    }
    return YES;
}

- (void)textFieldDidBeginEditing:(UITextField *)textField           // became first responder
{    
    if ( _delegate )
    {
        _delegate->didBeginEditing();
    }
}

- (BOOL)textFieldShouldEndEditing:(UITextField *)textField          // return YES to allow editing to stop and to resign first responder status. NO to disallow the editing session to end
{
    if ( _delegate )
    {
        return ( BOOL )_delegate->shouldEndEditing();
    }
    return YES;
}

- (void)textFieldDidEndEditing:(UITextField *)textField             // may be called if forced even if shouldEndEditing returns NO (e.g. view removed from window) or endEditing:YES called 
{    
    if ( _delegate )
    {
        _delegate->didEndEditing();
    }
}

- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string   // return NO to not change text
{    
    if ( _delegate )
    {
        return ( BOOL )_delegate->shouldChangeCharactersInRange( range.location, range.length, ofxNSStringToString( string ) );
    }
    return YES;
}

 
- (BOOL)textFieldShouldClear:(UITextField *)textField               // called when clear button pressed. return NO to ignore (no notifications)
{    
    if ( _delegate )
    {
        return ( BOOL )_delegate->shouldClear();
    }
    return YES;
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField              // called when 'return' key pressed. return NO to ignore.
{    
    if ( _delegate )
    {
        return ( BOOL )_delegate->shouldReturn();
    }
    return YES;
}


@end

#endif