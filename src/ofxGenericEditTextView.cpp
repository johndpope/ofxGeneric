//
//  ofxGenericEditTextView.cpp
//
//  Created by Ian Grossberg on 12/29/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#include "ofxGenericEditTextView.h"

#include "ofxGenericUtility.h"
#include "ofxGenericApp.h"
#include "ofxGenericImage.h"

#include "ofxGenericFont.h"

#if TARGET_OS_IPHONE
@interface ofxGenericEditTextViewForwarder : NSObject< UITextFieldDelegate, UITextViewDelegate >
{
@protected
    ofxGenericEditTextView* _delegate;
}
-( id )initWithDelegate:( ofxGenericEditTextView* )delegate;
@end
#endif

ofPtr< ofxGenericEditTextView > ofxGenericEditTextView::create( const ofRectangle& setFrame, ofPtrWeak< ofxGenericEditTextViewDelegate > delegate, bool multiline )
{
    ofPtr< ofxGenericEditTextView > create = ofPtr< ofxGenericEditTextView >( new ofxGenericEditTextView() );
    create->init( create, setFrame, delegate, multiline );
    return create;
}

ofxGenericEditTextView::ofxGenericEditTextView()
: _multiline( false ), _isEditing( false ), _moveFromUnderKeyboardOnBeginEdit( false ), _hideKeyboardOnReturn( true )
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

void ofxGenericEditTextView::init( ofPtrWeak< ofxGenericView > setThis, const ofRectangle& setFrame, ofPtrWeak< ofxGenericEditTextViewDelegate > delegate, bool multiline )
{
    _multiline = multiline;
    
    setDelegate( delegate );
    ofxGenericView::init( setThis, setFrame );
}

NativeView ofxGenericEditTextView::createNativeView( const ofRectangle& frame )
{
#if TARGET_OS_IPHONE
    UIView* newView = nil;

    _forwarder = [ [ ofxGenericEditTextViewForwarder alloc ] initWithDelegate:this ];
    if ( !_multiline )
    {
        UITextField* newTextView = [ [ UITextField alloc ] initWithFrame:ofRectangleToCGRect( frame ) ];
        [ newTextView setDelegate:_forwarder ];
        [ newTextView setBackgroundColor:[ UIColor whiteColor ] ];
        [ newTextView setTextColor:[ UIColor blackColor ] ];
        [ newTextView setAdjustsFontSizeToFitWidth:YES ];
        [ newTextView setContentVerticalAlignment:UIControlContentVerticalAlignmentCenter ];
        
        newView = newTextView;
    } else
    {
        UITextView* newTextView = [ [ UITextView alloc ] initWithFrame:ofRectangleToCGRect( frame ) ];
        [ newTextView setDelegate:_forwarder ];
        [ newTextView setBackgroundColor:[ UIColor whiteColor ] ];
        [ newTextView setTextColor:[ UIColor blackColor ] ];
        [ newTextView setEditable:YES ];
        
        newView = newTextView;
    }

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
    NSString* nsString = ofxStringToNSString( newText );
    
    if ( [ getNativeView() isKindOfClass:[ UITextField class ] ] )
    {
        UITextField* textField = ( UITextField* )getNativeView();
        [ textField setText:nsString ];
    } else if ( [ getNativeView() isKindOfClass:[ UITextView class ] ] )
    {
        UITextView* textView = ( UITextView* )getNativeView();
        [ textView setText:nsString ];
    }
#endif
}

string ofxGenericEditTextView::getText()
{
    string result;
#if TARGET_OS_IPHONE
    if ( [ getNativeView() isKindOfClass:[ UITextField class ] ] )
    {
        UITextField* textField = ( UITextField* )getNativeView();
        result = ofxNSStringToString( [ textField text ] );
    } else if ( [ getNativeView() isKindOfClass:[ UITextView class ] ] )
    {
        UITextView* textView = ( UITextView* )getNativeView();
        result = ofxNSStringToString( [ textView text ] );
    }
#endif
    return result;
}

void ofxGenericEditTextView::setTextAlignment( ofxGenericTextHorizontalAlignment alignment )
{
#if TARGET_OS_IPHONE
    if ( [ getNativeView() isKindOfClass:[ UITextField class ] ] )
    {
        UITextField* textField = ( UITextField* )getNativeView();
        [ textField setTextAlignment:ofxGenericTextHorizontalAlignmentToiOS( alignment ) ];
    } else if ( [ getNativeView() isKindOfClass:[ UITextView class ] ] )
    {
        UITextView* textView = ( UITextView* )getNativeView();
        [ textView setTextAlignment:ofxGenericTextHorizontalAlignmentToiOS( alignment ) ];
    }
#endif
}

ofxGenericTextHorizontalAlignment ofxGenericEditTextView::getTextAlignment()
{
    ofxGenericTextHorizontalAlignment result = ofxGenericTextHorizontalAlignmentLeft;
#if TARGET_OS_IPHONE
    if ( [ getNativeView() isKindOfClass:[ UITextField class ] ] )
    {
        UITextField* textField = ( UITextField* )getNativeView();
        result = iOSToofxGenericTextHorizontalAlignment( textField.textAlignment );
    } else if ( [ getNativeView() isKindOfClass:[ UITextView class ] ] )
    {
        UITextView* textView = ( UITextView* )getNativeView();
        result = iOSToofxGenericTextHorizontalAlignment( textView.textAlignment );
    }
#endif
    return result;
}

void ofxGenericEditTextView::setFont( string name, float size )
{
    ofPtr< ofxGenericFont > font = ofxGenericFont::create( name, size );
    setFont( font );
}

void ofxGenericEditTextView::setFont( ofPtr< ofxGenericFont > font )
{
    if ( font )
    {
#if TARGET_OS_IPHONE
        if ( [ getNativeView() isKindOfClass:[ UITextField class ] ] )
        {
            UITextField* textField = ( UITextField* )getNativeView();
            [ textField setFont:font->getNativeFont() ];
        } else if ( [ getNativeView() isKindOfClass:[ UITextView class ] ] )
        {
            UITextView* textView = ( UITextView* )getNativeView();
            [ textView setFont:font->getNativeFont() ];
        }
#endif
    }
}

float ofxGenericEditTextView::getFontSize()
{
    float result = 0.0f;
#if TARGET_OS_IPHONE
    if ( [ getNativeView() isKindOfClass:[ UITextField class ] ] )
    {
        UITextField* textField = ( UITextField* )getNativeView();
        result = [ [ textField font ] pointSize ];
    } else if ( [ getNativeView() isKindOfClass:[ UITextView class ] ] )
    {
        UITextView* textView = ( UITextView* )getNativeView();
        result = [ [ textView font ] pointSize ];
    }
#endif
    return result;
}

string ofxGenericEditTextView::getFontName()
{
    string result;
#if TARGET_OS_IPHONE
    NSString* nsString = nil;
    if ( [ getNativeView() isKindOfClass:[ UITextField class ] ] )
    {
        UITextField* textField = ( UITextField* )getNativeView();
        nsString = [ [ textField font ] fontName ];
    } else if ( [ getNativeView() isKindOfClass:[ UITextView class ] ] )
    {
        UITextView* textView = ( UITextView* )getNativeView();
        nsString = [ [ textView font ] fontName ];
    }
    result = ofxNSStringToString( nsString );
#endif
    return result;
}

void ofxGenericEditTextView::setBackgroundImage( string imageFileName )
{
    string imagePath = ofxGenericImage::getNativeImagePath( imageFileName );
    if ( ofxGFileExists( imagePath ) )
    {
#if TARGET_OS_IPHONE
        UIImage* image = [ UIImage imageWithContentsOfFile:ofxStringToNSString( imagePath ) ];
        
        if ( [ getNativeView() isKindOfClass:[ UITextField class ] ] )
        {
            UITextField* textField = ( UITextField* )getNativeView();
            [ textField setBackground:image ];
        }
        // Unsupported
        /* else if ( [ getNativeView() isKindOfClass:[ UITextView class ] ] )
        {
            UITextView* textView = ( UITextView* )getNativeView();
        }*/
#endif
    } else
    {
        ofxGLogError( "Unable to find image file " + imagePath + ", cannot ofxGenericEditTextView::setBackgroundImage!" );
    }
}

void ofxGenericEditTextView::setPlaceholderText( string placeholderText )
{
#if TARGET_OS_IPHONE
    if ( [ getNativeView() isKindOfClass:[ UITextField class ] ] )
    {
        UITextField* textField = ( UITextField* )getNativeView();
        [ textField setPlaceholder:ofxStringToNSString( placeholderText ) ];
    }
#endif
}

string ofxGenericEditTextView::getPlaceholderText()
{
#if TARGET_OS_IPHONE
    if ( [ getNativeView() isKindOfClass:[ UITextField class ] ] )
    {
        UITextField* textField = ( UITextField* )getNativeView();
        return ofxNSStringToString( [ textField placeholder ] );
    }
#endif
    return string();    
}

bool ofxGenericEditTextView::currentlyEditing()
{
    return _isEditing;
}

void ofxGenericEditTextView::setClearsOnBeginEditing( bool clear )
{
#if TARGET_OS_IPHONE
    if ( [ getNativeView() isKindOfClass:[ UITextField class ] ] )
    {
        UITextField* textField = ( UITextField* )getNativeView();
        [ textField setClearsOnBeginEditing:( BOOL )clear ];
    }
#endif
}

bool ofxGenericEditTextView::getClearsOnBeginEditing()
{
#if TARGET_OS_IPHONE
    if ( [ getNativeView() isKindOfClass:[ UITextField class ] ] )
    {
        UITextField* textField = ( UITextField* )getNativeView();
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
    [ getNativeView() becomeFirstResponder ];
#endif
}

void ofxGenericEditTextView::didBeginEditing()
{    
    _isEditing = true;
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
    _isEditing = false;
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
    [ getNativeView() resignFirstResponder ];
#endif    
}

void ofxGenericEditTextView::setEnabled( bool enabled )
{
#if TARGET_OS_IPHONE
    if ( [ getNativeView() isKindOfClass:[ UITextField class ] ] )
    {
        UITextField* textField = ( UITextField* )getNativeView();
        [ textField setEnabled:( BOOL )enabled ];
    }
#endif
}
         
bool ofxGenericEditTextView::getEnabled()
{
#if TARGET_OS_IPHONE
    if ( [ getNativeView() isKindOfClass:[ UITextField class ] ] )
    {
        UITextField* textField = ( UITextField* )getNativeView();
        return textField.enabled;
    }
#endif 
    return false;
}

void ofxGenericEditTextView::setBorderStyle( ofxGenericTextViewBorderStyle borderStyle )
{
#if TARGET_OS_IPHONE
    if ( [ getNativeView() isKindOfClass:[ UITextField class ] ] )
    {
        UITextField* textField = ( UITextField* )getNativeView();
        [ textField setBorderStyle:ofxGenericTextViewBorderStyleToiOS( borderStyle ) ];
    }
#endif    
}

void ofxGenericEditTextView::setAutoCapitalization(ofxGenericTextAutoCapitalization autoCapitalization )
{
#if TARGET_OS_IPHONE
    if ( [ getNativeView() isKindOfClass:[ UITextField class ] ] )
    {
        UITextField* textField = ( UITextField* )getNativeView();
        [ textField setAutocapitalizationType:ofxGenericTextAutoCapitalizationToiOS( autoCapitalization ) ];
    } else if ( [ getNativeView() isKindOfClass:[ UITextView class ] ] )
    {
        UITextView* textView = ( UITextView* )getNativeView();
        [ textView setAutocapitalizationType:ofxGenericTextAutoCapitalizationToiOS( autoCapitalization ) ];
    }
#endif 
}

void ofxGenericEditTextView::setAutoCorrection( bool enabled )
{
#if TARGET_OS_IPHONE
    UITextAutocorrectionType autoCorrection;
    if ( enabled )
    {
        autoCorrection = UITextAutocorrectionTypeYes;
    } else
    {
        autoCorrection = UITextAutocorrectionTypeNo;
    }

    if ( [ getNativeView() isKindOfClass:[ UITextField class ] ] )
    {
        UITextField* textField = ( UITextField* )getNativeView();
        [ textField setAutocorrectionType:autoCorrection ];
    } else if ( [ getNativeView() isKindOfClass:[ UITextView class ] ] )
    {
        UITextView* textView = ( UITextView* )getNativeView();
        [ textView setAutocorrectionType:autoCorrection ];
    }
#endif
}

bool ofxGenericEditTextView::getAutoCorrection()
{
#if TARGET_OS_IPHONE
    if ( [ getNativeView() isKindOfClass:[ UITextField class ] ] )
    {
        UITextField* textField = ( UITextField* )getNativeView();
        return textField.autocorrectionType == UITextAutocorrectionTypeYes;
    } else if ( [ getNativeView() isKindOfClass:[ UITextView class ] ] )
    {
        UITextView* textView = ( UITextView* )getNativeView();
        return textView.autocorrectionType == UITextAutocorrectionTypeYes;
    }
#endif
    return false;
}

void ofxGenericEditTextView::setSpellChecking( bool enabled )
{
#if TARGET_OS_IPHONE
    UITextSpellCheckingType spellChecking;
    if ( enabled )
    {
        spellChecking = UITextSpellCheckingTypeYes;
    } else
    {
        spellChecking = UITextSpellCheckingTypeNo;
    }
    if ( [ getNativeView() isKindOfClass:[ UITextField class ] ] )
    {
        UITextField* textField = ( UITextField* )getNativeView();
        [ textField setSpellCheckingType:spellChecking ];
    } else if ( [ getNativeView() isKindOfClass:[ UITextView class ] ] )
    {
        UITextView* textView = ( UITextView* )getNativeView();
        [ textView setSpellCheckingType:spellChecking ];
    }
#endif
}

bool ofxGenericEditTextView::getSpellChecking()
{
#if TARGET_OS_IPHONE
    if ( [ getNativeView() isKindOfClass:[ UITextField class ] ] )
    {
        UITextField* textField = ( UITextField* )getNativeView();
        return textField.spellCheckingType == UITextSpellCheckingTypeYes;
    } else if ( [ getNativeView() isKindOfClass:[ UITextView class ] ] )
    {
        UITextView* textView = ( UITextView* )getNativeView();
        return textView.spellCheckingType == UITextSpellCheckingTypeYes;
    }
#endif
    return false;
}

void ofxGenericEditTextView::setEnableReturnKeyAutomatically( bool enabled )
{
#if TARGET_OS_IPHONE
    if ( [ getNativeView() isKindOfClass:[ UITextField class ] ] )
    {
        UITextField* textField = ( UITextField* )getNativeView();
        [ textField setEnablesReturnKeyAutomatically:enabled ];
    } else if ( [ getNativeView() isKindOfClass:[ UITextView class ] ] )
    {
        UITextView* textView = ( UITextView* )getNativeView();
        [ textView setEnablesReturnKeyAutomatically:enabled ];
    }
#endif
}

bool ofxGenericEditTextView::getEnableReturnKeyAutomatically()
{
#if TARGET_OS_IPHONE
    if ( [ getNativeView() isKindOfClass:[ UITextField class ] ] )
    {
        UITextField* textField = ( UITextField* )getNativeView();
        return textField.enablesReturnKeyAutomatically == YES;
    } else if ( [ getNativeView() isKindOfClass:[ UITextView class ] ] )
    {
        UITextView* textView = ( UITextView* )getNativeView();
        return textView.enablesReturnKeyAutomatically == YES;
    }
#endif
    return false;
}

void ofxGenericEditTextView::setKeyboard( ofxGenericKeyboardType type )
{
#if TARGET_OS_IPHONE
    if ( [ getNativeView() isKindOfClass:[ UITextField class ] ] )
    {
        UITextField* textField = ( UITextField* )getNativeView();
        [ textField setKeyboardType:ofxGenericKeyboardTypeToiOS( type ) ];
    } else if ( [ getNativeView() isKindOfClass:[ UITextView class ] ] )
    {
        UITextView* textView = ( UITextView* )getNativeView();
        [ textView setKeyboardType:ofxGenericKeyboardTypeToiOS( type ) ];
    }
#endif
}

void ofxGenericEditTextView::setKeyboardReturnKey( ofxGenericKeyboardReturnKey key )
{
#if TARGET_OS_IPHONE
    if ( [ getNativeView() isKindOfClass:[ UITextField class ] ] )
    {
        UITextField* textField = ( UITextField* )getNativeView();
        [ textField setReturnKeyType:ofxGenericKeyboardReturnKeyToiOS( key ) ];
    } else if ( [ getNativeView() isKindOfClass:[ UITextView class ] ] )
    {
        UITextView* textView = ( UITextView* )getNativeView();
        [ textView setReturnKeyType:ofxGenericKeyboardReturnKeyToiOS( key ) ];
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
    if ( [ getNativeView() isKindOfClass:[ UITextField class ] ] )
    {
        UITextField* textField = ( UITextField* )getNativeView();
        [ textField setSecureTextEntry:( BOOL )secure ];
    } else if ( [ getNativeView() isKindOfClass:[ UITextView class ] ] )
    {
        UITextView* textView = ( UITextView* )getNativeView();
        [ textView setSecureTextEntry:( BOOL )secure ];
    }
#endif
}

bool ofxGenericEditTextView::getSecureText()
{
#if TARGET_OS_IPHONE
    if ( [ getNativeView() isKindOfClass:[ UITextField class ] ] )
    {
        UITextField* textField = ( UITextField* )getNativeView();
        return textField.secureTextEntry == YES;
    } else if ( [ getNativeView() isKindOfClass:[ UITextView class ] ] )
    {
        UITextView* textView = ( UITextView* )getNativeView();
        return textView.secureTextEntry == YES;
    }
#endif
    return false;   
}

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

-( BOOL )textViewShouldBeginEditing:( UITextView* )textView
{
    if ( _delegate )
    {
        return ( BOOL )_delegate->shouldBeginEditing();
    }
    return YES;
}

-( BOOL )textViewShouldEndEditing:( UITextView* )textView
{
    if ( _delegate )
    {
        return ( BOOL )_delegate->shouldEndEditing();
    }
    return YES;
}
 
-( void )textViewDidBeginEditing:( UITextView* )textView
{
    if ( _delegate )
    {
        _delegate->didBeginEditing();
    }
}

-( void )textViewDidEndEditing:( UITextView* )textView
{
    if ( _delegate )
    {
        _delegate->didEndEditing();
    }
}

/*
-( BOOL )textView:( UITextView* )textView shouldChangeTextInRange:( NSRange )range replacementText:( NSString* )text
{
    return YES;
}

-( void )textViewDidChange:( UITextView* )textView
{
}

-( void )textViewDidChangeSelection:( UITextView* )textView
{
}
*/
@end

#endif