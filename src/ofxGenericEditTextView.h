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
@class ofxGenericEditTextViewDelegateForwarder;
#endif

class ofxGenericEditTextView : public ofxGenericView
{
public:
    ofxGenericEditTextView();
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
    
protected:
    virtual NativeView createNativeView( const ofRectangle& frame );
#if TARGET_OS_IPHONE
    ofxGenericEditTextViewDelegateForwarder* _forwarder;
#endif
    
    bool _moveFromUnderKeyboardOnBeginEdit;
};

#if TARGET_OS_IPHONE
@interface ofxGenericEditTextViewDelegateForwarder : NSObject< UITextFieldDelegate >
{
@protected
    ofxGenericEditTextView* _delegate;
}
-( id )initWithDelegate:( ofxGenericEditTextView* )delegate;
@end
#endif
