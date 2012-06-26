//
//  ofxGenericEditTextView.cpp
//
//  Created by Ian Grossberg on 12/29/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#include "ofxGenericEditTextView.h"
#include "ofxGenericUtility.h"
#include "ofxGenericApp.h"

#if TARGET_OS_IPHONE
@interface ofxGenericEditTextViewForwarder : NSObject< UITextFieldDelegate >
{
@protected
    ofxGenericEditTextView* _delegate;
}
-( id )initWithDelegate:( ofxGenericEditTextView* )delegate;
@end
#endif

ofPtr< ofxGenericEditTextView > ofxGenericEditTextView::create( const ofRectangle& setFrame, ofPtrWeak< ofxGenericEditTextViewDelegate > delegate )
{
    ofPtr< ofxGenericEditTextView > create = ofPtr< ofxGenericEditTextView >( new ofxGenericEditTextView() );
    create->init( create, setFrame, delegate );
    return create;
}

ofxGenericEditTextView::ofxGenericEditTextView()
: _moveFromUnderKeyboardOnBeginEdit( false ), _hideKeyboardOnReturn( true )
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

void ofxGenericEditTextView::init( ofPtrWeak< ofxGenericView > setThis, const ofRectangle& setFrame, ofPtrWeak< ofxGenericEditTextViewDelegate > delegate )
{
    ofxGenericView::init( setThis, setFrame );
    setDelegate( delegate );
}

NativeView ofxGenericEditTextView::createNativeView( const ofRectangle& frame )
{
#if TARGET_OS_IPHONE
    UITextField* newView = [ [ UITextField alloc ] initWithFrame:ofRectangleToCGRect( frame ) ];
    _forwarder = [ [ ofxGenericEditTextViewForwarder alloc ] initWithDelegate:this ];
    [ newView setDelegate:_forwarder ];
    [ newView setBackgroundColor:[ UIColor whiteColor ] ];
    [ newView setTextColor:[ UIColor blackColor ] ];
    [ newView setAdjustsFontSizeToFitWidth:YES ];
    return newView;
#endif
}

void ofxGenericEditTextView::setDelegate( ofPtrWeak< ofxGenericEditTextViewDelegate > delegate )
{
    _delegate = delegate;
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

void ofxGenericEditTextView::setTextAlignment( ofxGenericTextHorizontalAlignment alignment )
{
#if TARGET_OS_IPHONE
    UITextField* textField = ( UITextField* )*this;
    if ( textField )
    {
        [ textField setTextAlignment:ofxGenericTextHorizontalAlignmentToiOS( alignment ) ];
    }
#endif
}

ofxGenericTextHorizontalAlignment ofxGenericEditTextView::getTextAlignment()
{
#if TARGET_OS_IPHONE
    UITextField* textField = ( UITextField* )*this;
    if ( textField )
    {
        return iOSToofxGenericTextHorizontalAlignment( textField.textAlignment );
    }
#endif
    return ofxGenericTextHorizontalAlignmentLeft;
}

void ofxGenericEditTextView::setBackgroundImage( string imageFileName )
{
#if TARGET_OS_IPHONE
    UITextField* textField = ( UITextField* )*this;
    if ( textField )
    {
        [ textField setBackground:[ UIImage imageWithContentsOfFile:ofxStringToNSString( ofxGPathToDataFolder( imageFileName ) ) ] ];
    }    
#endif
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

bool ofxGenericEditTextView::getClearsOnBeginEditing()
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
    if ( _delegate )
    {
        return _delegate.lock()->editText_shouldBeginEditing( dynamic_pointer_cast< ofxGenericEditTextView >( _this ) );
    }
    return true;
}

void ofxGenericEditTextView::setFocusOn()
{
#if TARGET_OS_IPHONE
    UITextField* view = *this;
    if ( view )
    {
        [ view becomeFirstResponder ];
    }
#endif
}

void ofxGenericEditTextView::didBeginEditing()
{    
    if ( _moveFromUnderKeyboardOnBeginEdit )
    {
        ofxGenericApp::getInstance()->setMoveFromUnderKeyboard( _this.lock() );
    }
    if ( _delegate )
    {
        _delegate.lock()->editText_didBeginEditing( dynamic_pointer_cast< ofxGenericEditTextView >( _this ) );
    }
}

bool ofxGenericEditTextView::shouldEndEditing()
{    
    if ( _delegate )
    {
        return _delegate.lock()->editText_shouldEndEditing( dynamic_pointer_cast< ofxGenericEditTextView >( _this ) );
    }    return true;
}

void ofxGenericEditTextView::didEndEditing()
{
    if ( _moveFromUnderKeyboardOnBeginEdit )
    {
        ofxGenericApp::getInstance()->setMoveFromUnderKeyboard( ofPtr< ofxGenericView >() );
    }
    if ( _delegate )
    {
        _delegate.lock()->editText_didEndEditing( dynamic_pointer_cast< ofxGenericEditTextView >( _this ) );
    }
}

bool ofxGenericEditTextView::shouldChangeCharactersInRange( int from, int count, string replacement )
{    
    if ( _delegate )
    {
        return _delegate.lock()->editText_shouldChangeCharactersInRange( dynamic_pointer_cast< ofxGenericEditTextView >( _this ), from, count, replacement );
    }
    return true;
}

bool ofxGenericEditTextView::shouldClear()
{
    if ( _delegate )
    {
        return _delegate.lock()->editText_shouldClear( dynamic_pointer_cast< ofxGenericEditTextView >( _this ) );
    }
    return true;
}

bool ofxGenericEditTextView::shouldReturn()
{  
    //next responder transfer should happen here only
#if TARGET_OS_IPHONE
    if ( _nextResponder )
    {
        [ _nextResponder.lock()->getNativeView() becomeFirstResponder ];
    }
    else
    {
        [ _this.lock()->getNativeView() resignFirstResponder ];
    }
#endif
    
    bool shouldReturn = true;
    if ( _delegate )
    {
        shouldReturn = _delegate.lock()->editText_shouldReturn( dynamic_pointer_cast< ofxGenericEditTextView >( _this ) );
    }
    if ( shouldReturn )
    {
        if ( _hideKeyboardOnReturn )
        {
            hideKeyboard();
        }
    }
    return shouldReturn;
}

void ofxGenericEditTextView::hideKeyboard()
{
#if TARGET_OS_IPHONE
    // TODO: wrap resigning keyboard separately 
    UITextField* textField = ( UITextField* )*this;
    if ( textField )
    {
        [ textField resignFirstResponder ];
    }
#endif    
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

void ofxGenericEditTextView::setBorderStyle( ofxGenericTextViewBorderStyle borderStyle )
{
#if TARGET_OS_IPHONE
    UITextField* textField = ( UITextField* )*this;
    if ( textField )
    {
        [ textField setBorderStyle:ofxGenericTextViewBorderStyleToiOS( borderStyle ) ];
    }
#endif    
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

void ofxGenericEditTextView::setHideKeyboardOnReturn( bool enabled )
{
    _hideKeyboardOnReturn = enabled;
}

bool ofxGenericEditTextView::getHideKeyboardOnReturn()
{
    return _hideKeyboardOnReturn;
}
     
void ofxGenericEditTextView::setSecureText( bool secure )
{
#if TARGET_OS_IPHONE
    UITextField* textField = ( UITextField* )*this;
    if ( textField )
    {
        [ textField setSecureTextEntry:( BOOL )secure ];
    }
#endif 
}

bool ofxGenericEditTextView::getSecureText()
{
#if TARGET_OS_IPHONE
    UITextField* textField = ( UITextField* )*this;
    if ( textField )
    {
        return ( bool )textField.secureTextEntry;
    }
#endif 
    return false;   
}


#if TARGET_OS_IPHONE
ofxGenericUIViewCastOperator( ofxGenericEditTextView, UITextField );
#endif

#if TARGET_OS_IPHONE

@implementation ofxGenericEditTextViewForwarder

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
    _delegate = NULL;
    [ super dealloc ];
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