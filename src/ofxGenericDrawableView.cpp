//
//  ofxDrawing.cpp
//  ofxGeneric
//
//  Created by Eli Delventhal on 4/26/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericDrawableView.h"
#include "ofUtils.h"
#include "ofxGenericValueStore.h"

#define DrawCallTypePosition 0
#define DrawCallTypeLineWidth 1
#define DrawCallTypeLineColor 2
#define DrawCallTypeArc 3
#define DrawCallType2PointsArc 4
#define DrawCallTypeFillColor 5
#define DrawCallTypeFinish 6
#define DrawCallTypePoint 7

#define DrawDataType "type"
#define DrawDataHasStroke "hasStroke"
#define DrawDataHasFill "hasFill"
#define DrawDataColorR "colorR"
#define DrawDataColorG "colorG"
#define DrawDataColorB "colorB"
#define DrawDataColorA "colorA"
#define DrawDataPointX "pointX"
#define DrawDataPointY "pointY"
#define DrawDataPointEndX "pointEndX"
#define DrawDataPointEndY "pointEndY"
#define DrawDataRadius "radius"
#define DrawDataAngleStart "angleStart"
#define DrawDataAngleEnd "angleEnd"
#define DrawDataClockwise "clockwise"
#define DrawDataLineWidth "lineWidth"

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

void ofxGenericDrawableView::endPath( bool hasStroke, bool hasFill )
{
    //_drawCalls.push_back( ofxGenericDrawCall( hasStroke, hasFill ) );
    ofPtr< ofxGenericValueStore > drawData = ofxGenericValueStore::create( false );
    drawData->write( DrawDataType, DrawCallTypeFinish );
    drawData->write( DrawDataHasStroke, hasStroke );
    drawData->write( DrawDataHasFill, hasFill );
    _drawCalls.push_back( drawData );
}

void ofxGenericDrawableView::setLineWidth( float w )
{
    //_drawCalls.push_back( ofxGenericDrawCall( w ) );
    ofPtr< ofxGenericValueStore > drawData = ofxGenericValueStore::create( false );
    drawData->write( DrawDataType, DrawCallTypeLineWidth );
    drawData->write( DrawDataLineWidth, w );
    _drawCalls.push_back( drawData );
}

void ofxGenericDrawableView::setStrokeColor( const ofColor &c )
{
    //_drawCalls.push_back( ofxGenericDrawCall( c ) );
    ofPtr< ofxGenericValueStore > drawData = ofxGenericValueStore::create( false );
    drawData->write( DrawDataType, DrawCallTypeLineColor );
    drawData->write( DrawDataColorR, c.r );
    drawData->write( DrawDataColorG, c.g );
    drawData->write( DrawDataColorB, c.b );
    drawData->write( DrawDataColorA, c.a );
    _drawCalls.push_back( drawData );
}

void ofxGenericDrawableView::setFillColor( const ofColor &c )
{
    //_drawCalls.push_back( ofxGenericDrawCall( c, true ) );
    ofPtr< ofxGenericValueStore > drawData = ofxGenericValueStore::create( false );
    drawData->write( DrawDataType, DrawCallTypeFillColor );
    drawData->write( DrawDataColorR, c.r );
    drawData->write( DrawDataColorG, c.g );
    drawData->write( DrawDataColorB, c.b );
    drawData->write( DrawDataColorA, c.a );
    _drawCalls.push_back( drawData );
}

void ofxGenericDrawableView::drawLineToPoint( const ofPoint &p )
{
    //_drawCalls.push_back( ofxGenericDrawCall( p ) );
    ofPtr< ofxGenericValueStore > drawData = ofxGenericValueStore::create( false );
    drawData->write( DrawDataType, DrawCallTypePoint );
    drawData->write( DrawDataPointX, p.x );
    drawData->write( DrawDataPointY, p.y );
    _drawCalls.push_back( drawData );
}

void ofxGenericDrawableView::drawLine( const ofPoint &p1, const ofPoint &p2 )
{
    //_drawCalls.push_back( ofxGenericDrawCall( p1, p2 ) );
    ofPtr< ofxGenericValueStore > drawData = ofxGenericValueStore::create( false );
    drawData->write( DrawDataType, DrawCallTypePosition );
    drawData->write( DrawDataPointX, p1.x );
    drawData->write( DrawDataPointY, p1.y );
    drawData->write( DrawDataPointEndX, p2.x );
    drawData->write( DrawDataPointEndY, p2.y );
    _drawCalls.push_back( drawData );
}

void ofxGenericDrawableView::drawLines( std::vector< ofPoint > points )
{
    for ( unsigned int i = 0; i < points.size(); i++)
    {
        //_drawCalls.push_back( ofxGenericDrawCall( points[i] ) );
        ofPtr< ofxGenericValueStore > drawData = ofxGenericValueStore::create( false );
        drawData->write( DrawDataType, DrawCallTypePoint );
        drawData->write( DrawDataPointX, points[i].x );
        drawData->write( DrawDataPointY, points[i].y );
        _drawCalls.push_back( drawData );
    }
}

void ofxGenericDrawableView::drawArc( const ofPoint &p, float radius, float startAngle, float endAngle, bool clockwise )
{
    //_drawCalls.push_back( ofxGenericDrawCall( p, radius, startAngle, endAngle, clockwise ) );
    ofPtr< ofxGenericValueStore > drawData = ofxGenericValueStore::create( false );
    drawData->write( DrawDataType, DrawCallTypeArc );
    drawData->write( DrawDataPointX, p.x );
    drawData->write( DrawDataPointY, p.y );
    drawData->write( DrawDataRadius, radius );
    drawData->write( DrawDataAngleStart, startAngle );
    drawData->write( DrawDataAngleEnd, endAngle );
    drawData->write( DrawDataClockwise, clockwise );
    _drawCalls.push_back( drawData );
}

void ofxGenericDrawableView::drawArc( const ofPoint &p1, const ofPoint &p2, float radius )
{
    //_drawCalls.push_back( ofxGenericDrawCall( p1, p2, radius ) );
    ofPtr< ofxGenericValueStore > drawData = ofxGenericValueStore::create( false );
    drawData->write( DrawDataType, DrawCallType2PointsArc );
    drawData->write( DrawDataPointX, p1.x );
    drawData->write( DrawDataPointY, p1.y );
    drawData->write( DrawDataPointEndX, p2.x );
    drawData->write( DrawDataPointEndY, p2.y );
    drawData->write( DrawDataRadius, radius );
    _drawCalls.push_back( drawData );
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

ofPtr< ofxGenericValueStore > ofxGenericDrawableView::getDrawCallAt(int i)
{
    return _drawCalls[i];
}

void ofxGenericDrawableView::clearDrawCalls()
{
    _drawCalls.clear();
}

//////////////////////////ofxGenericDrawCall////////////////////////
//replaced these with ofxGenericValueStores so that this can be more easily expanded and is less bloated

/*ofxGenericDrawCall::~ofxGenericDrawCall()
{
    
}

ofxGenericDrawCall::ofxGenericDrawCall( bool hasStroke, bool hasFill )
{
    _type = DrawCallTypeFinish;
    _hasStroke = hasStroke;
    _hasFill = hasFill;
}

ofxGenericDrawCall::ofxGenericDrawCall( ofPoint next )
{
    _type = DrawCallTypePoint;
    _start = next;
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
    _type = isFill ? DrawCallTypeColorFill : DrawCallTypeColor;
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

int ofxGenericDrawCall::getType()
{
    return _type;
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

bool ofxGenericDrawCall::hasStroke()
{
    return _hasStroke;
}

bool ofxGenericDrawCall::hasFill()
{
    return _hasFill;
}*/

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
        
        //insert a finish at the end of we don't already have one
        if ( own->getDrawCallAt( own->getDrawCallCount() - 1 )->read( DrawDataType, -1 ) != DrawCallTypeFinish )
        {
            own->endPath();
        }
        
        for (int i = 0; i < own->getDrawCallCount(); i++)
        {
            ofPtr< ofxGenericValueStore > call = own->getDrawCallAt(i);
            int type = call->read( DrawDataType, -1 );
            
            if ( type == DrawCallTypeLineWidth )
            {
                CGContextSetLineWidth(context, call->read( DrawDataLineWidth, 0.0f ) );
            }
            else if ( type == DrawCallTypeLineColor )
            {
                ofColor color = ofColor
                (
                    call->read( DrawDataColorR, 0 ),
                    call->read( DrawDataColorG, 0 ),
                    call->read( DrawDataColorB, 0 ),
                    call->read( DrawDataColorA, 0 )
                );
                CGContextSetStrokeColorWithColor(context, ofxColorToUIColor( color ).CGColor );
            }
            else if ( type == DrawCallTypeFillColor )
            {
                ofColor color = ofColor
                (
                    call->read( DrawDataColorR, 0 ),
                    call->read( DrawDataColorG, 0 ),
                    call->read( DrawDataColorB, 0 ),
                    call->read( DrawDataColorA, 0 )
                 );
                CGContextSetFillColorWithColor(context, ofxColorToUIColor( color ).CGColor );
            }
            else if ( type == DrawCallTypePosition )
            {
                if ( CGContextIsPathEmpty( context ) )
                {
                    CGContextMoveToPoint(context, call->read( DrawDataPointX, 0.0f ), call->read( DrawDataPointY, 0.0f ) );
                }
                else
                {
                    CGPoint p = CGContextGetPathCurrentPoint(context);
                    if ( call->read( DrawDataPointX, 0.0f ) != p.x && call->read( DrawDataPointY, 0.0f ) != p.y )
                    {
                        CGContextMoveToPoint(context, call->read( DrawDataPointX, 0.0f ), call->read( DrawDataPointY, 0.0f ) );
                    }
                }
                
                CGContextAddLineToPoint(context, call->read( DrawDataPointEndX, 0.0f ), call->read( DrawDataPointEndY, 0.0f ) );
            }
            else if ( type == DrawCallTypePoint )
            {
                CGContextAddLineToPoint(context, call->read( DrawDataPointX, 0.0f ), call->read( DrawDataPointY, 0.0f ) );
            }
            else if ( type == DrawCallTypeArc )
            {
                CGContextAddArc(context,
                                call->read( DrawDataPointX, 0.0f ),
                                call->read( DrawDataPointY, 0.0f ),
                                call->read( DrawDataRadius, 0.0f ),
                                call->read( DrawDataAngleStart, 0.0f ),
                                call->read( DrawDataAngleEnd, 0.0f ),
                                call->read( DrawDataClockwise, false ) );
            }
            else if ( type == DrawCallType2PointsArc )
            {
                CGContextAddArcToPoint(context,
                                call->read( DrawDataPointX, 0.0f ),
                                call->read( DrawDataPointY, 0.0f ),
                                call->read( DrawDataPointEndX, 0.0f ),
                                call->read( DrawDataPointEndY, 0.0f ),
                                call->read( DrawDataRadius, 0.0f ) );
            }
            else if ( type == DrawCallTypeFinish )
            {
                CGPathDrawingMode mode = kCGPathStroke;
                
                if ( call->read( DrawDataHasFill, false ) && call->read( DrawDataHasStroke, false ) )
                {
                    mode = kCGPathFillStroke;
                    CGContextClosePath( context );
                }
                else if ( call->read( DrawDataHasStroke, false ) )
                {
                    mode = kCGPathStroke;
                }
                else if ( call->read( DrawDataHasFill, false ) )
                {
                    mode = kCGPathFill;
                    CGContextClosePath( context );
                }
                
                CGContextDrawPath( context, mode );
                
                //start the next path if we are not done
                if ( i < own->getDrawCallCount() - 1 )
                {
                    CGContextBeginPath( context );
                }
            }
        }
    }
    //own->clearDrawCalls();
}

@end
#endif