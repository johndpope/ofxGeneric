//
//  ofxGenericScrollView.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 3/14/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericView.h"

class ofxGenericScrollView : public ofxGenericView
{
public:
    static ofPtr< ofxGenericScrollView > create( const ofRectangle& setFrame = ofRectangle( 0, 0, 0, 0 ) );
    
    void setContentSize( const ofPoint& contentSize );
    void setContentOffset( const ofPoint& contentOffset, bool animated );
    
#if TARGET_OS_IPHONE
    operator UIScrollView*();
#endif
    
protected:
    ofxGenericScrollView();
    virtual NativeView createNativeView( const ofRectangle& frame );
};