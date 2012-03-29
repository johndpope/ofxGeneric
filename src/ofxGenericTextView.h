//
//  ofxGenericTextView.h
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 12/29/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericView.h"
#include "ofxGenericConstants.h"

class ofxGenericTextView : public ofxGenericView
{
public:
    static ofPtr< ofxGenericTextView > create( const ofRectangle& setFrame = ofRectangle( 0, 0, 0, 0 ) );
    
    void setText( string newText );
    string getText();
    
    void setTextAlignment( ofxGenericTextHorizontalAlignment alignment );
    ofxGenericTextHorizontalAlignment getTextAlignment();
    
    void setTextColor ( const ofColor& setColor );
    ofColor getTextColor ();
    
    void setLineBreakMode ( ofxGenericTextLinebreakMode mode );
    ofxGenericTextLinebreakMode getLineBreakMode();
    
    void setNumberOfLines ( int n );
    int getNumberOfLines ();

#if TARGET_OS_IPHONE
    operator UILabel*();
#endif

protected:
    ofxGenericTextView() {};
    
    virtual NativeView createNativeView( const ofRectangle& frame );
};
