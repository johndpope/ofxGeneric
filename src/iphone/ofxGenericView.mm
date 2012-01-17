//
//  ofxGenericView.mm
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 12/29/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#import "ofxGenericView.h"
#import "ofxGenericUtility.h"

#import <UIKit/UIKit.h>

@interface ofxUIGenericViewController : UIViewController
{
@private
    NSMutableDictionary* _activeTouches;
}
@end

ofxGenericView::ofxGenericView()
: _children(), _parent( NULL )
, _view( nil ), _viewController( nil )
{
}
    
ofxGenericView::~ofxGenericView()
{
    removeFromParent();
    // TODO: make togglable?
    destroyChildViews();
    
    [ _view removeFromSuperview ];
    [ _view release ];
    _view = nil;
    [ _viewController removeFromParentViewController ];
    [ _viewController release ];
    _viewController = nil;
}

void ofxGenericView::init( const ofRectangle& setBounds )
{
    _view = createUIView( ofRectangleToCGRect( setBounds ) );
    _viewController = createUIViewController();
    [ _viewController setView:_view ];
}

UIView* ofxGenericView::createUIView( const CGRect& frame )
{
    UIView* newView = [ [ UIView alloc ] initWithFrame:frame ];
    
    [ newView setBackgroundColor:[ UIColor whiteColor ] ];
    [ newView setOpaque:YES ];
    [ newView setHidden:NO ];
    
    return newView;
}

UIViewController* ofxGenericView::createUIViewController()
{
    return [ [ ofxUIGenericViewController alloc ] init ];
}

UIView* ofxGenericView::getUIView()
{
    return _view;
}

UIViewController* ofxGenericView::getUIViewController()
{
    return _viewController;
}

ofRectangle ofxGenericView::getBounds()
{
    return CGRectToofRectangle( [ _view frame ] );
}

void ofxGenericView::setBounds( const ofRectangle& setBounds )
{
    [ _view setFrame: ofRectangleToCGRect( setBounds ) ];
}

ofColor ofxGenericView::getBackgroundColor()
{
    return UIColorToofColor( [ _view backgroundColor ] );
}

void ofxGenericView::setBackgroundColor( const ofColor& setColor )
{
    [ _view setBackgroundColor:ofColorToUIColor( setColor ) ];
}

void ofxGenericView::addChildView( ofxGenericView* add )
{
    if ( add )
    {
        if ( add->getUIView() )
        {
            _children.push_back( add );
            [ _view addSubview:add->getUIView() ];
            add->_parent = this;
        } else
        {
            // tODO:
        }
    } else
    {
        // TODO:
    }
}

void ofxGenericView::removeChildView( ofxGenericView* remove )
{
    if ( remove )
    {
        if ( remove->getUIView() )
        {
            if ( remove->_parent == this )
            {
                [ remove->getUIView() removeFromSuperview ];
                _children.remove( remove );
                remove->_parent = NULL;
            } else
            {
                // TODO:
            }
        } else
        {
            // TODO:
        }
    } else
    {
        // TODO:
    }
}

void ofxGenericView::removeFromParent()
{
    if ( _parent )
    {
        _parent->removeChildView( this );
    }
}

void ofxGenericView::destroyChildViews()
{
    while( _children.size() )
    {
        ofxGenericView* destroyView = *_children.begin();
        _children.pop_front();
        delete destroyView;
    }
}

@implementation ofxUIGenericViewController

-( id )init
{
    self = [ super init ];
    if ( self )
    {
        _activeTouches = [ [ NSMutableDictionary alloc ] init ];
    }
    return self;
}

-( void )dealloc
{
    [ _activeTouches release ];
    _activeTouches = nil;
    [ super dealloc ];
}

#pragma mark Touch Events

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event 
{
	for(UITouch *touch in touches) 
    {
/*		int touchIndex = 0;
		while([[activeTouches allValues] containsObject:[NSNumber numberWithInt:touchIndex]]) {
			touchIndex++;
		}
		
		[activeTouches setObject:[NSNumber numberWithInt:touchIndex] forKey:[NSValue valueWithPointer:touch]];
		
		CGPoint touchPoint = [touch locationInView:self.view];
		
		touchPoint.x *= getWindowScale();
		touchPoint.y *= getWindowScale();
		
		iPhoneGetOFWindow()->rotateXY(touchPoint.x, touchPoint.y);
		
		if( touchIndex==0 ){
			ofNotifyMousePressed(touchPoint.x, touchPoint.y, 0);
		}
		
		ofTouchEventArgs touchArgs;
		touchArgs.x = touchPoint.x;
		touchArgs.y = touchPoint.y;
		touchArgs.id = touchIndex;
		if([touch tapCount] == 2) ofNotifyEvent(ofEvents.touchDoubleTap,touchArgs);	// send doubletap
		ofNotifyEvent(ofEvents.touchDown,touchArgs);	// but also send tap (upto app programmer to ignore this if doubletap came that frame)*/
	}
	
}

//------------------------------------------------------
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event 
{
	//	NSLog(@"touchesMoved: %i %i %i", [touches count],  [[event touchesForView:self] count], multitouchData.numTouches);
	
	for(UITouch *touch in touches) 
    {
		/*int touchIndex = [[activeTouches objectForKey:[NSValue valueWithPointer:touch]] intValue];
		//		[activeTouches setObject:[NSNumber numberWithInt:touchIndex] forKey:[NSValue valueWithPointer:touch]];
		
		CGPoint touchPoint = [touch locationInView:self];
		
		touchPoint.x*=touchScaleFactor; // this has to be done because retina still returns points in 320x240 but with high percision
		touchPoint.y*=touchScaleFactor;
		
		iPhoneGetOFWindow()->rotateXY(touchPoint.x, touchPoint.y);
		
		if( touchIndex==0 ){
			ofNotifyMouseDragged(touchPoint.x, touchPoint.y, 0);			
		}		
		ofTouchEventArgs touchArgs;
		touchArgs.numTouches = [[event touchesForView:self] count];
		touchArgs.x = touchPoint.x;
		touchArgs.y = touchPoint.y;
		touchArgs.id = touchIndex;
		ofNotifyEvent(ofEvents.touchMoved, touchArgs);*/
	}
	
}

//------------------------------------------------------
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event 
{
	//	NSLog(@"touchesEnded: %i %i %i", [touches count],  [[event touchesForView:self] count], multitouchData.numTouches);
	for(UITouch *touch in touches) 
    {
/*		int touchIndex = [[activeTouches objectForKey:[NSValue valueWithPointer:touch]] intValue];
		
		[activeTouches removeObjectForKey:[NSValue valueWithPointer:touch]];
		
		CGPoint touchPoint = [touch locationInView:self];
		
		touchPoint.x*=touchScaleFactor; // this has to be done because retina still returns points in 320x240 but with high percision
		touchPoint.y*=touchScaleFactor;
		
		iPhoneGetOFWindow()->rotateXY(touchPoint.x, touchPoint.y);
		
		if( touchIndex==0 ){
			ofNotifyMouseReleased(touchPoint.x, touchPoint.y, 0);						
		}
		
		ofTouchEventArgs touchArgs;
		touchArgs.numTouches = [[event touchesForView:self] count] - [touches count];
		touchArgs.x = touchPoint.x;
		touchArgs.y = touchPoint.y;
		touchArgs.id = touchIndex;
		ofNotifyEvent(ofEvents.touchUp, touchArgs);*/
	}
}

//------------------------------------------------------
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event 
{
	for(UITouch *touch in touches) 
    {
/*		int touchIndex = [[activeTouches objectForKey:[NSValue valueWithPointer:touch]] intValue];
		
		CGPoint touchPoint = [touch locationInView:self];
		
		touchPoint.x*=touchScaleFactor; // this has to be done because retina still returns points in 320x240 but with high percision
		touchPoint.y*=touchScaleFactor;
		
		iPhoneGetOFWindow()->rotateXY(touchPoint.x, touchPoint.y);
		
		ofTouchEventArgs touchArgs;
		touchArgs.numTouches = [[event touchesForView:self] count];
		touchArgs.x = touchPoint.x;
		touchArgs.y = touchPoint.y;
		touchArgs.id = touchIndex;
		ofNotifyEvent(ofEvents.touchCancelled, touchArgs);*/
	}
	
	[self touchesEnded:touches withEvent:event];
}

@end
