//
//  ofxGenericImageView.h
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 1/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericView.h"

class ofxGenericImageView : public ofxGenericView
{
public:
    void setImage( string fileName );
    
#if TARGET_OS_IPHONE
    operator UIImageView*();
#endif
    
protected:
#if TARGET_OS_IPHONE
    virtual UIView* createUIView( const CGRect& frame );
#endif
};
