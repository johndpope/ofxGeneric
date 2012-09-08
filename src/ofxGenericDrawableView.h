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
    virtual void drawArc( const ofPoint &center, float radius, float startAngle, float endAngle, bool clockwise );
    virtual void drawArc( const ofPoint &p1, const ofPoint &p2, float radius );
    virtual void fillPath( const ofColor &c );
    
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
    ofxGenericDrawCall( ofColor color, bool isFill = false );
    ofxGenericDrawCall( ofPoint center, float radius, float startAngle, float endAngle, bool clockwise );
    ofxGenericDrawCall( ofPoint start, ofPoint end, float radius );
    ~ofxGenericDrawCall();
    
    bool changesLineWidth();
    bool changesColor();
    bool changesPosition();
    bool isArc();
    bool is2PointsArc();
    bool isFill();
    
    ofPoint getStart();
    ofPoint getEnd();
    float getLineWidth();
    ofColor getColor();
    float getRadius();
    float getStartAngle();
    float getEndAngle();
    bool isClockwise();
    
protected:
    int _type;
    ofPoint _start;
    ofPoint _end;
    ofColor _color;
    float _lineWidth;
    float _radius;
    float _startAngle;
    float _endAngle;
    bool _clockwise;
};

#if TARGET_OS_IPHONE
@interface ofxDrawableUIView : UIView
{
    ofPtrWeak< ofxGenericDrawableView > drawOwner;
}

- (id) init:(CGRect)frame owner:(ofPtrWeak< ofxGenericDrawableView >)own;

@end

#endif