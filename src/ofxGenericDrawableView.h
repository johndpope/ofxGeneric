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

class ofxGenericValueStore;
//class ofxGenericDrawCall;

class ofxGenericDrawableView : public ofxGenericView
{
public:
    virtual ~ofxGenericDrawableView();
    static ofPtr < ofxGenericDrawableView > create( const ofRectangle &rect = ofRectangle(0, 0, 0, 0) );
    
    //Finish drawing an individual path. If this is not the last entry, then a stroked no-fill path is assumed to be the last entry.
    virtual void endPath( bool hasStroke = true, bool hasFill = false );
    
    virtual void setLineWidth(float w);
    virtual void setStrokeColor( const ofColor &c );
    virtual void setFillColor( const ofColor &c );
    virtual void drawLineToPoint( const ofPoint &p );
    virtual void drawLine( const ofPoint &p1, const ofPoint &p2 );
    virtual void drawLines( std::vector< ofPoint > points );
    virtual void drawArc( const ofPoint &center, float radius, float startAngle, float endAngle, bool clockwise );
    virtual void drawArc( const ofPoint &p1, const ofPoint &p2, float radius );
    
    //clears all pending draw calls and updates the drawing context
    virtual void repaint();
    
    virtual int getDrawCallCount();
    virtual ofPtr< ofxGenericValueStore > getDrawCallAt(int i);
    virtual void clearDrawCalls();
    
protected:
    ofxGenericDrawableView();

    std::vector< ofPtr< ofxGenericValueStore > > _drawCalls;
    
#if TARGET_OS_IPHONE
    virtual UIView* allocNativeView( const ofRectangle& frame );
#elif TARGET_ANDROID
    virtual jobject allocNativeView( const ofRectangle& frame );
#endif
};

/*class ofxGenericDrawCall
{
public: 
    ofxGenericDrawCall( bool hasStroke = true, bool hasFill = false );
    ofxGenericDrawCall( ofPoint next );
    ofxGenericDrawCall( ofPoint start, ofPoint end );
    ofxGenericDrawCall( float lineWidth );
    ofxGenericDrawCall( ofColor color, bool isFill = false );
    ofxGenericDrawCall( ofPoint center, float radius, float startAngle, float endAngle, bool clockwise );
    ofxGenericDrawCall( ofPoint start, ofPoint end, float radius );
    ~ofxGenericDrawCall();
    
    int getType();
    
    ofPoint getStart();
    ofPoint getEnd();
    float getLineWidth();
    ofColor getColor();
    float getRadius();
    float getStartAngle();
    float getEndAngle();
    bool isClockwise();
    bool hasStroke();
    bool hasFill();
    
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
    bool _hasStroke;
    bool _hasFill;
};*/

#if TARGET_OS_IPHONE
@interface ofxDrawableUIView : UIView
{
    ofPtrWeak< ofxGenericDrawableView > drawOwner;
}

- (id) init:(CGRect)frame owner:(ofPtrWeak< ofxGenericDrawableView >)own;

@end

#endif