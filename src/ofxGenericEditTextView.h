//
//  ofxGenericEditTextView.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 3/6/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericView.h"
#include "ofxGenericConstants.h"

#if TARGET_OS_IPHONE
@class ofxGenericEditTextViewForwarder;
#endif

class ofxGenericEditTextViewDelegate;

class ofxGenericEditTextView : public ofxGenericView
{
public:
    static ofPtr< ofxGenericEditTextView > create( const ofRectangle& setFrame = ofRectangle( 0, 0, 0, 0 ), ofPtrWeak< ofxGenericEditTextViewDelegate > delegate = ofPtrWeak< ofxGenericEditTextViewDelegate >() );
    virtual ~ofxGenericEditTextView();
    
    void setDelegate( ofPtrWeak< ofxGenericEditTextViewDelegate > delegate );
    
    void setText( string newText );
    string getText();
    void setTextAlignment( ofxGenericTextHorizontalAlignment alignment );
    ofxGenericTextHorizontalAlignment getTextAlignment();
    
    void setBackgroundImage( string imageFileName );
    
    void setPlaceholderText( string placeholderText );
    string getPlaceholderText();
    
    void setMoveFromUnderKeyboardOnBeginEdit( bool move );
    
#if TARGET_OS_IPHONE
    operator UITextField*();
#endif
    
    
    void setClearsOnBeginEditing( bool clear );
    bool getClearsOnBeginEditing();
    
    virtual void setFocusOn();
    virtual bool shouldBeginEditing();
    virtual void didBeginEditing();
    virtual bool shouldEndEditing();
    virtual void didEndEditing();
    virtual bool shouldChangeCharactersInRange( int from, int count, string replacement );
    bool currentlyEditing();
    virtual bool shouldClear();
    virtual bool shouldReturn();
    
    void setEnabled( bool enabled );
    bool getEnabled();
    
    void setBorderStyle( ofxGenericTextViewBorderStyle borderStyle );
    
    void setAutoCapitalization( ofxGenericTextAutoCapitalization autoCapitalization );
    
    void setAutoCorrection( bool enabled );
    bool getAutoCorrection();
    
    void setSpellChecking( bool enabled );
    bool getSpellChecking();
    
    void setEnableReturnKeyAutomatically( bool enabled );
    bool getEnableReturnKeyAutomatically();

    void setKeyboard( ofxGenericKeyboardType type );
    void setKeyboardReturnKey( ofxGenericKeyboardReturnKey key );
    
    void setHideKeyboardOnReturn( bool enabled );
    bool getHideKeyboardOnReturn();

    void setSecureText( bool secure );
    bool getSecureText();
    

    // TODO: find better place
    virtual void hideKeyboard();

protected:
    ofxGenericEditTextView();
        virtual void init( ofPtrWeak< ofxGenericView > setThis, const ofRectangle& setFrame = ofRectangle( 0, 0, 0, 0 ), ofPtrWeak< ofxGenericEditTextViewDelegate > delegate = ofPtrWeak< ofxGenericEditTextViewDelegate >() );
    virtual NativeView createNativeView( const ofRectangle& frame );
#if TARGET_OS_IPHONE
    ofxGenericEditTextViewForwarder* _forwarder;
#endif
    
    ofPtrWeak< ofxGenericEditTextViewDelegate > _delegate;
    
    bool _moveFromUnderKeyboardOnBeginEdit;
    bool _hideKeyboardOnReturn;
};

class ofxGenericEditTextViewDelegate
{
public:
    virtual ~ofxGenericEditTextViewDelegate() {};
    
    virtual bool editText_shouldBeginEditing( ofPtr< ofxGenericEditTextView > view ) { return true; };
    virtual void editText_didBeginEditing( ofPtr< ofxGenericEditTextView > view ) {};
    virtual bool editText_shouldEndEditing( ofPtr< ofxGenericEditTextView > view ) { return true; };
    virtual void editText_didEndEditing( ofPtr< ofxGenericEditTextView > view ) {};
    virtual bool editText_shouldChangeCharactersInRange(  ofPtr< ofxGenericEditTextView > view, int from, int count, string replacement ) { return true; };
    virtual bool editText_shouldClear( ofPtr< ofxGenericEditTextView > view ) { return true; }
    virtual bool editText_shouldReturn( ofPtr< ofxGenericEditTextView > view ) { return true; }
};
