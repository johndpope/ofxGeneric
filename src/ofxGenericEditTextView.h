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

class ofxGenericEditTextView : public ofxGenericView
{
public:
    static ofPtr< ofxGenericEditTextView > create( const ofRectangle& setFrame = ofRectangle( 0, 0, 0, 0 ) );
    virtual ~ofxGenericEditTextView();
    
    void setText( string newText );
    string getText();
//    void setTextAlignment( ofxGenericTextHorizontalAlignment alignment );
    
    void setPlaceholderText( string placeholderText );
    string getPlaceholderText();
    
    void setMoveFromUnderKeyboardOnBeginEdit( bool move );
    
#if TARGET_OS_IPHONE
    operator UITextField*();
#endif
    
    bool currentlyEditing();
    
    void setClearsOnBeginEditing( bool clear );
    bool clearsOnBeginEditing();
    
    virtual bool shouldBeginEditing();
    virtual void didBeginEditing();
    virtual bool shouldEndEditing();
    virtual void didEndEditing();
    virtual bool shouldChangeCharactersInRange( int from, int count, string replacement );
    virtual bool shouldClear();
    virtual bool shouldReturn();
    
    void setEnabled( bool enabled );
    bool getEnabled();
    
    void setAutoCapitalization( ofxGenericTextAutoCapitalization autoCapitalization );
    
    void setAutoCorrection( bool enabled );
    bool getAutoCorrection();
    
    void setSpellChecking( bool enabled );
    bool getSpellChecking();
    
    void setEnableReturnKeyAutomatically( bool enabled );
    bool getEnableReturnKeyAutomatically();

    void setKeyboard( ofxGenericKeyboardType type );
    void setKeyboardReturnKey( ofxGenericKeyboardReturnKey key );
    
    void setSecureText( bool secure );
    bool getSecureText();
    
    
protected:
    ofxGenericEditTextView();
    virtual NativeView createNativeView( const ofRectangle& frame );
#if TARGET_OS_IPHONE
    ofxGenericEditTextViewForwarder* _forwarder;
#endif
    
    bool _moveFromUnderKeyboardOnBeginEdit;
};
