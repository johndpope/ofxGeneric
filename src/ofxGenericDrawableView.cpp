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
/*#if TARGET_OS_IPHONE
    CGContextSetLineWidth(UIGraphicsGetCurrentContext(), w);
#endif*/
    
}

void ofxGenericDrawableView::setStrokeColor( const ofColor &c )
{
    _drawCalls.push_back( ofxGenericDrawCall( c ) );
/*#if TARGET_OS_IPHONE
    CGContextSetStrokeColorWithColor(UIGraphicsGetCurrentContext(), [UIColor colorWithRed:c.r green:c.g blue:c.b alpha:c.a].CGColor);
#endif    */
}

void ofxGenericDrawableView::drawLine( const ofPoint &p1, const ofPoint &p2 )
{
    _drawCalls.push_back( ofxGenericDrawCall( p1, p2 ) );
/*#if TARGET_OS_IPHONE
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextBeginPath(context);
    CGContextMoveToPoint(context, p1.x, p1.y);
    CGContextAddLineToPoint(context, p2.x, p2.y);
    CGContextStrokePath(context);
#endif    */
}

void ofxGenericDrawableView::drawLines( std::vector< ofPoint > points )
{
    for (int i = 0; i < points.size(); i++)
    {
        _drawCalls.push_back( ofxGenericDrawCall( points[0].x, points[1].y ) );
    }
    
/*#if TARGET_OS_IPHONE
    if (points.size() > 0)
    {
        CGContextRef context = UIGraphicsGetCurrentContext();
        CGContextBeginPath(context);
        CGContextMoveToPoint(context, points[0].x, points[1].y);
        
        for (int i = 0; i < (int) points.size(); i++)
        {
            CGContextAddLineToPoint(context, points[i].x, points[i].y);
        }
        
        CGContextStrokePath(context);
    }
#endif  */
    
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

ofxGenericDrawCall::~ofxGenericDrawCall()
{
    
}

ofxGenericDrawCall::ofxGenericDrawCall( ofPoint startPoint, ofPoint endPoint )
{
    start = startPoint;
    end = endPoint;
    newPosition = true;
    newColor = false;
    newLineWidth = false;
}

ofxGenericDrawCall::ofxGenericDrawCall( float wid )
{
    lineWidth = wid;
    newPosition = false;
    newColor = false;
    newLineWidth = true;
}

ofxGenericDrawCall::ofxGenericDrawCall( ofColor col )
{
    color = col;
    newPosition = false;
    newColor = true;
    newLineWidth = false;
}

bool ofxGenericDrawCall::changesLineWidth()
{
    return newLineWidth;
}

bool ofxGenericDrawCall::changesColor()
{
    return newColor;
}

bool ofxGenericDrawCall::changesPosition()
{
    return newPosition;
}

ofPoint ofxGenericDrawCall::getStart()
{
    return start;
}

ofPoint ofxGenericDrawCall::getEnd()
{
    return end;
}

float ofxGenericDrawCall::getLineWidth()
{
    return lineWidth;
}

ofColor ofxGenericDrawCall::getColor()
{
    return color;
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
        }
        
        CGContextDrawPath(context, kCGPathStroke);
    }
    own->clearDrawCalls();
}

@end
#endif