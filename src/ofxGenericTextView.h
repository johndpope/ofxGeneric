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
    void setText( string newText );
    string getText();
    void setTextAlignment( ofxGenericTextHorizontalAlignment alignment );

#if TARGET_OS_IPHONE
    operator UILabel*();
#endif

protected:
#if TARGET_OS_IPHONE
    virtual UIView* createUIView( const CGRect& frame );
#endif
};
