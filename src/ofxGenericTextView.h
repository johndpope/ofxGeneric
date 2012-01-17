//
//  ofxGenericTextView.h
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 12/29/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericView.h"

class ofxGenericTextView : public ofxGenericView
{
public:
    void setText( string newText );
    string getText();
    
protected:
    virtual UIView* createUIView( const CGRect& frame );
};