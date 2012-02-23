//
//  ofxGenericView.cpp
//
//  Created by Ian Grossberg on 12/29/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#include "ofxGenericView.h"
#include "ofxGenericUtility.h"

#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>

@interface ofxUIGenericViewController : UIViewController
{
@private
    NSMutableDictionary* _activeTouches;
}
@end
#endif

#if TARGET_ANDROID
#include "JNIUtility.h"
#include "JNIRect.h"

jclass ofxGenericView::_jniClass = NULL;
const char* ofxGenericView::className = "cc/openframeworks/ofxGeneric/View";
#endif

ofxGenericView::ofxGenericView()
:
#if TARGET_OS_IPHONE
 _view( nil ), _viewController( nil ),
#elif TARGET_ANDROID
 _view( NULL ),
#endif
_children()
{
}

ofxGenericView::~ofxGenericView()
{
    removeFromParent();
    // TODO: make toggle-able?
    removeChildViews();

#if TARGET_OS_IPHONE
    releaseView( _view );
    releaseViewController( _viewController );
#elif TARGET_ANDROID
	destroyJNIReference( _view );
#endif
}

void ofxGenericView::init( ofPtrWeak< ofxGenericView > setThis, const ofRectangle& setFrame )
{
    _this = setThis;

#if TARGET_ANDROID
    registerJNIMethods();
#endif

    _view = createNativeView( setFrame );
#if TARGET_OS_IPHONE
    _viewController = createUIViewController();
    [ _viewController setView:_view ];
#elif TARGET_ANDROID
    JNIRect jniRect = ofRectangleToJNIRect( setFrame );
    callJNIVoidMethod(
    		_jniMethods,
    		JNIMethod_Init,
    		jniRect.getJNIInstance()
    		);
#endif
}

#if TARGET_ANDROID
jclass ofxGenericView::getJNIClassStatic()
{
	// TODO: handle exception
    if ( !_jniClass )
    	_jniClass = ( jclass )ofxGenericView::createJNIReferenceStatic( JNIFindClass( ofxGenericView::className ) );
    return _jniClass;
}

jclass ofxGenericView::getJNIClass()
{
	return ofxGenericView::getJNIClassStatic();
}


jobject ofxGenericView::getJNIInstance()
{
	return _view;
}

#endif

NativeView ofxGenericView::createNativeView( const ofRectangle& setFrame )
{
#if TARGET_OS_IPHONE
	UIView* newView = [ [ UIView alloc ] initWithFrame:ofRectangleToCGRect( setFrame ) ];

	// TODO: move to shared code override-able function after _view assigned? or just assign _view in here
    [ newView setBackgroundColor:[ UIColor whiteColor ] ];
    [ newView setOpaque:YES ];
    [ newView setHidden:NO ];

    return newView;

#elif TARGET_ANDROID

    // TODO: exception handling
    jobject newView = createJNIInstance( _jniMethods, JNIMethod_constructor );
    // set background white
    // set opaque
    // set not hidden
    return createJNIReference( newView );
#endif
}

NativeView ofxGenericView::getNativeView()
{
	return _view;
}

ofxGenericView::operator NativeView()
{
	return _view;
}

#if TARGET_OS_IPHONE
UIViewController* ofxGenericView::createUIViewController()
{
    return [ [ ofxUIGenericViewController alloc ] init ];
}

UIViewController* ofxGenericView::getUIViewController()
{
    return _viewController;
}
#endif

ofRectangle ofxGenericView::getFrame()
{
#if TARGET_OS_IPHONE
    return CGRectToofRectangle( [ _view frame ] );
#elif TARGET_ANDROID
    jobject jniFrame = callJNIObjectMethod( _jniMethods, JNIMethod_GetFrame );
    JNIRect jniRect( jniFrame );
    return JNIRectToofRectangle( jniRect );
#endif
}

void ofxGenericView::setFrame( const ofRectangle& setFrame )
{
#if TARGET_OS_IPHONE
    [ _view setFrame: ofRectangleToCGRect( setFrame ) ];
#elif TARGET_ANDROID
    JNIRect jniRect = ofRectangleToJNIRect( setFrame ); // TODO: temporary ref passing going to killlllll
    callJNIVoidMethod( _jniMethods, JNIMethod_SetFrame, jniRect.getJNIInstance() );
#endif
}

ofColor ofxGenericView::getBackgroundColor()
{
#if TARGET_OS_IPHONE
    return UIColorToofColor( [ _view backgroundColor ] );
#elif TARGET_ANDROID
    jint androidColor = callJNIIntMethod( _jniMethods, JNIMethod_GetBackgroundColor );
    return JNIColorToofColor( androidColor );
#endif
}

void ofxGenericView::setBackgroundColor( const ofColor& setColor )
{
#if TARGET_OS_IPHONE
    [ _view setBackgroundColor:ofColorToUIColor( setColor ) ];
#elif TARGET_ANDROID
    callJNIVoidMethod(
    		_jniMethods,
    		JNIMethod_SetBackgroundColor,
    		( int )setColor.r, ( int )setColor.g, ( int )setColor.b, ( int )setColor.a
    		);
#endif
}

void ofxGenericView::addChildView( ofPtr< ofxGenericView > add )
{
    if ( add )
    {
        _children.push_back( add );
        NativeView nativeView = add->getNativeView();
        if ( nativeView )
        {
#if TARGET_OS_IPHONE
          	[ _view addSubview:nativeView ];
#elif TARGET_ANDROID
          	callJNIVoidMethod(
          			_jniMethods,
          			JNIMethod_AddChildView,
          			nativeView
          			);
#endif
        } else
        {
            // TODO:
        }
        add->_parent = _this;
    } else
    {
        // TODO:
    }
}

void ofxGenericView::removeChildView( ofPtr< ofxGenericView > remove )
{
    if ( remove )
    {
		if ( remove->_parent == _this )
		{
            NativeView nativeView = remove->getNativeView();
			if ( nativeView )
			{
#if TARGET_OS_IPHONE
				[ nativeView removeFromSuperview ];
#elif TARGET_ANDROID
				callJNIVoidMethod(
						_jniMethods,
						JNIMethod_RemoveChildView,
						nativeView
						);
#endif
			} else
			{
				// TODO:
			}
			_children.remove( remove );
			remove->_parent = ofPtrWeak< ofxGenericView >();
        } else
        {
            // TODO:
        }
    } else
    {
        // TODO:
    }
}

ofPtr< ofxGenericView > ofxGenericView::getChildViewofPtr( ofxGenericView* forView )
{
    for(
        std::list< ofPtr< ofxGenericView > >::iterator trav = _children.begin();
        trav != _children.end();
        trav ++
        )
    {
        if ( &**trav == forView )
            return *trav;
    }
    return ofPtr< ofxGenericView >();
}

void ofxGenericView::removeFromParent()
{
    if ( _parent )
    {
        ofPtr< ofxGenericView > parentPtr = _parent.lock();
        ofPtr< ofxGenericView > thisPtr = parentPtr->getChildViewofPtr( this );
        parentPtr->removeChildView( thisPtr );
    }
}

void ofxGenericView::removeChildViews()
{
    _children.clear();
}

#if TARGET_ANDROID
void ofxGenericView::registerJNIMethods()
{
    registerJNIMethodID(
    		_jniMethods,
    		false,
    		JNIMethod_constructor,
			"<init>",
			JNIEncodeMethodSignature( 0, JNIType_void )
    		);

    registerJNIMethodID(
    		_jniMethods,
    		false,
    		JNIMethod_Init,
			"Init",
			JNIEncodeMethodSignature( 1, JNIType_void, JNIType_object, JNIRect::className )
    		);

    registerJNIMethodID(
    		_jniMethods,
    		false,
    		JNIMethod_GetFrame,
    		"getFrame",
    		JNIEncodeMethodSignature( 0, JNIType_object, JNIRect::className )
    		);

    registerJNIMethodID(
    		_jniMethods,
    		false,
    		JNIMethod_SetFrame,
    		"setFrame",
    		JNIEncodeMethodSignature( 1, JNIType_void, JNIType_object, JNIRect::className )
    		);

    registerJNIMethodID(
    		_jniMethods,
    		false,
    		JNIMethod_GetBackgroundColor,
    		"getBackgroundColor",
    		JNIEncodeMethodSignature( 0, JNIType_int )
    		);

    registerJNIMethodID(
    		_jniMethods,
    		false,
    		JNIMethod_SetBackgroundColor,
    		"setBackgroundColor",
    		JNIEncodeMethodSignature( 4, JNIType_void, JNIType_int, JNIType_int, JNIType_int, JNIType_int )
    		);

    registerJNIMethodID(
    		_jniMethods,
    		false,
    		JNIMethod_AddChildView,
    		"addChildView",
    		JNIEncodeMethodSignature( 1, JNIType_void, JNIType_object, ofxGenericView::className )
    		);

    registerJNIMethodID(
    		_jniMethods,
    		false,
    		JNIMethod_RemoveChildView,
    		"removeChildView",
    		JNIEncodeMethodSignature( 1, JNIType_void, JNIType_object, ofxGenericView::className )
    		);
}
#endif

#if TARGET_OS_IPHONE
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
#endif


