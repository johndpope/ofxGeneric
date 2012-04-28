//
//  ofxDrawing.h
//  ofxGeneric
//
//  Created by Eli Delventhal on 4/26/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericView.h"

#if TARGET_OS_IPHONE
#include <UIKit/UIKit.h>
#endif

class ofxGenericDrawCall;

class ofxGenericDrawableView : public ofxGenericView
{
public:
    virtual ~ofxGenericDrawableView();
    static ofPtr < ofxGenericDrawableView > create( const ofRectangle &rect = ofRectangle(0, 0, 0, 0) );
    
    virtual void setLineWidth(float w);
    virtual void setStrokeColor( const ofColor &c );
    virtual void drawLine( const ofPoint &p1, const ofPoint &p2 );
    virtual void drawLines( std::vector< ofPoint > points );
    
    //clears all pending draw calls and updates the drawing context
    virtual void repaint();
    
    virtual int getDrawCallCount();
    virtual ofxGenericDrawCall getDrawCallAt(int i);
    virtual void clearDrawCalls();
    
protected:
    ofxGenericDrawableView();

    std::vector< ofxGenericDrawCall > _drawCalls;
    
#if TARGET_OS_IPHONE
    virtual UIView* allocNativeView( const ofRectangle& frame );
#elif TARGET_ANDROID
    virtual jobject allocNativeView( const ofRectangle& frame );
#endif
};

class ofxGenericDrawCall
{
public:
    ofxGenericDrawCall( ofPoint start, ofPoint end );
    ofxGenericDrawCall( float lineWidth );
    ofxGenericDrawCall( ofColor color );
    ~ofxGenericDrawCall();
    bool changesLineWidth();
    bool changesColor();
    bool changesPosition();
    ofPoint getStart();
    ofPoint getEnd();
    float getLineWidth();
    ofColor getColor();
    
protected:
    ofPoint start;
    ofPoint end;
    float lineWidth;
    ofColor color;
    bool newLineWidth;
    bool newColor;
    bool newPosition;
};

#if TARGET_OS_IPHONE
@interface ofxDrawableUIView : UIView
{
    ofPtrWeak< ofxGenericDrawableView > drawOwner;
}

- (id) init:(CGRect)frame owner:(ofPtrWeak< ofxGenericDrawableView >)own;

@end

#endif