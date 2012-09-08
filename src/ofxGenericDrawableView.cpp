//
//  ofxDrawing.cpp
//  ofxGeneric
//
//  Created by Eli Delventhal on 4/26/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericDrawableView.h"
#include "ofUtils.h"

ofxGenericDrawableView::~ofxGenericDrawableView()
{
    
}

ofxGenericDrawableView::ofxGenericDrawableView()
{
    
}

ofPtr < ofxGenericDrawableView > ofxGenericDrawableView::create( const ofRectangle &rect)
{
    ofPtr< ofxGenericDrawableView > c = ofPtr< ofxGenericDrawableView > ( new ofxGenericDrawableView() );
    c->init( c, rect );
    return c;
}

void ofxGenericDrawableView::setLineWidth(float w)
{
    _drawCalls.push_back( ofxGenericDrawCall( w ) );    
}

void ofxGenericDrawableView::setStrokeColor( const ofColor &c )
{
    _drawCalls.push_back( ofxGenericDrawCall( c ) );
}

void ofxGenericDrawableView::drawLine( const ofPoint &p1, const ofPoint &p2 )
{
    _drawCalls.push_back( ofxGenericDrawCall( p1, p2 ) );
}

void ofxGenericDrawableView::drawLines( std::vector< ofPoint > points )
{
    for ( unsigned int i = 0; i < points.size(); i++)
    {
        _drawCalls.push_back( ofxGenericDrawCall( points[0].x, points[1].y ) );
    }
}

void ofxGenericDrawableView::drawArc( const ofPoint &p, float radius, float startAngle, float endAngle, bool clockwise )
{
    _drawCalls.push_back( ofxGenericDrawCall( p, radius, startAngle, endAngle, clockwise ) );
}

void ofxGenericDrawableView::drawArc( const ofPoint &p1, const ofPoint &p2, float radius )
{
    _drawCalls.push_back( ofxGenericDrawCall( p1, p2, radius ) );
}

void ofxGenericDrawableView::fillPath( const ofColor &c )
{
    _drawCalls.push_back( ofxGenericDrawCall( c, true ) );
}

#if TARGET_OS_IPHONE
UIView* ofxGenericDrawableView::allocNativeView( const ofRectangle& setFrame )
{
    return [ [ ofxDrawableUIView alloc ] init:CGRectMake(setFrame.x, setFrame.y, setFrame.width, setFrame.height) owner: dynamic_pointer_cast< ofxGenericDrawableView >(_this) ];
}
#elif TARGET_ANDROID
jobject ofxGenericDrawableView::allocNativeView( const ofRectangle& setFrame )
{
    return ofxGenericView::allocNativeView( setFrame );
}
#endif

void ofxGenericDrawableView::repaint()
{
#if TARGET_OS_IPHONE
    [(ofxDrawableUIView *)_view setNeedsDisplay];
#endif
}

int ofxGenericDrawableView::getDrawCallCount()
{
    return _drawCalls.size();
}

ofxGenericDrawCall ofxGenericDrawableView::getDrawCallAt(int i)
{
    return _drawCalls[i];
}

void ofxGenericDrawableView::clearDrawCalls()
{
    _drawCalls.clear();
}

//////////////////////////ofxGenericDrawCall////////////////////////

#define DrawCallTypePosition 0
#define DrawCallTypeLineWidth 1
#define DrawCallTypeColor 2
#define DrawCallTypeArc 3
#define DrawCallType2PointsArc 4
#define DrawCallTypeColorFill 5

ofxGenericDrawCall::~ofxGenericDrawCall()
{
    
}

ofxGenericDrawCall::ofxGenericDrawCall( ofPoint startPoint, ofPoint endPoint )
{
    _start = startPoint;
    _end = endPoint;
    _type = DrawCallTypePosition;
    
}

ofxGenericDrawCall::ofxGenericDrawCall( float wid )
{
    _lineWidth = wid;
    _type = DrawCallTypeLineWidth;
}

ofxGenericDrawCall::ofxGenericDrawCall( ofColor col, bool isFill )
{
    _color = col;
    _type = isFill ? DrawCallTypeColor : DrawCallTypeColorFill;
}

ofxGenericDrawCall::ofxGenericDrawCall( ofPoint center, float radius, float startAngle, float endAngle, bool clockwise )
{
    _start = center;
    _radius = radius;
    _startAngle = startAngle;
    _endAngle = endAngle;
    _clockwise = clockwise;
    _type = DrawCallTypeArc;
}

ofxGenericDrawCall::ofxGenericDrawCall( ofPoint start, ofPoint end, float radius )
{
    _start = start;
    _end = end;
    _radius = radius;
    _type = DrawCallType2PointsArc;
}

bool ofxGenericDrawCall::changesLineWidth()
{
    return _type == DrawCallTypeLineWidth;
}

bool ofxGenericDrawCall::changesColor()
{
    return _type == DrawCallTypeColor;
}

bool ofxGenericDrawCall::changesPosition()
{
    return _type == DrawCallTypePosition;
}

bool ofxGenericDrawCall::isArc()
{
    return _type == DrawCallTypeArc;
}

bool ofxGenericDrawCall::is2PointsArc()
{
    return _type == DrawCallType2PointsArc;
}

bool ofxGenericDrawCall::isFill()
{
    return _type == DrawCallTypeColorFill;
}

ofPoint ofxGenericDrawCall::getStart()
{
    return _start;
}

ofPoint ofxGenericDrawCall::getEnd()
{
    return _end;
}

float ofxGenericDrawCall::getLineWidth()
{
    return _lineWidth;
}

ofColor ofxGenericDrawCall::getColor()
{
    return _color;
}

float ofxGenericDrawCall::getRadius()
{
    return _radius;
}

float ofxGenericDrawCall::getStartAngle()
{
    return _startAngle;
}

float ofxGenericDrawCall::getEndAngle()
{
    return _endAngle;
}

bool ofxGenericDrawCall::isClockwise()
{
    return _clockwise;
}

////////////////////////////ofxDrawableUIView////////////////////////////

#if TARGET_OS_IPHONE
@implementation ofxDrawableUIView

- (id) init:(CGRect)frame owner:(ofPtrWeak< ofxGenericDrawableView >)own
{
    if (self = [super initWithFrame:frame])
    {
        drawOwner = own;
        return self;
    }
    return nil;
}

- (void) drawRect:(CGRect)rect
{
    //TODO use CGContextAddLines if we need more speed
    ofPtr< ofxGenericDrawableView > own = drawOwner.lock();
    if (own->getDrawCallCount() > 0)
    {
        CGContextRef context = UIGraphicsGetCurrentContext();
        CGContextBeginPath(context);
        
        ofPoint lastPoint = ofPoint(-1,-1);
        
        for (int i = 0; i < own->getDrawCallCount(); i++)
        {
            ofxGenericDrawCall call = own->getDrawCallAt(i);
            
            if (call.changesLineWidth())
            {
                CGContextSetLineWidth(context, call.getLineWidth());
            }
            
            if (call.changesColor())
            {
                ofColor c = call.getColor();
                CGContextSetStrokeColorWithColor(context, [UIColor colorWithRed:c.r green:c.g blue:c.b alpha:c.a].CGColor);
            }
            
            if (call.changesPosition())
            {
                if (call.getStart() != lastPoint)
                {
                    CGContextMoveToPoint(context, call.getStart().x, call.getStart().y);
                }
                
                CGContextAddLineToPoint(context, call.getEnd().x, call.getEnd().y);
                lastPoint = call.getEnd();
            }
            
            if (call.isArc())
            {
                CGContextAddArc(context, call.getStart().x, call.getStart().y, call.getRadius(), call.getStartAngle(), call.getEndAngle(), call.isClockwise());
                lastPoint = ofPoint( -9999, -9999 );
            }
            
            if (call.is2PointsArc())
            {
                CGContextAddArcToPoint(context, call.getStart().x, call.getStart().y, call.getEnd().x, call.getEnd().y, call.getRadius());
                lastPoint = ofPoint( -9999, -9999 );
            }
            
            if (call.isFill())
            {
                ofColor c = call.getColor();
                CGContextSetFillColorWithColor(context, [UIColor colorWithRed:c.r green:c.g blue:c.b alpha:c.a].CGColor );
                CGContextFillPath(context);
            }
        }
        
        CGContextDrawPath(context, kCGPathStroke);
    }
    own->clearDrawCalls();
}

@end
#endif