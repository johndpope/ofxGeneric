//
//  ofxGenericButtonView.cpp
//
//  Created by Ian Grossberg on 12/30/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#include "ofxGenericButtonView.h"
#include "ofxGenericUtility.h"

#if TARGET_ANDROID
#include "JNIUtility.h"

jclass ofxGenericButtonView::_jniClass = NULL;
const char* ofxGenericButtonView::className = "cc/openframeworks/ofxGeneric/ButtonView";

std::vector< ofxGenericButtonView* > ofxGenericButtonView::_nativeMap;
#endif

ofxGenericButtonView::ofxGenericButtonView()
{
}

ofxGenericButtonView::~ofxGenericButtonView()
{
#if TARGET_OS_IPHONE
    release( _eventHandler );
#endif
}

void ofxGenericButtonView::init( ofPtrWeak< ofxGenericView > setThis, const ofRectangle& setBounds, ofPtrWeak< ofxGenericButtonViewTouchDelegate > touchDelegate )
{
    ofxGenericView::init( setThis, setBounds );
    _touchDelegate = touchDelegate;

#if TARGET_ANDROID
    int androidViewID = _nativeMap.size();
    _nativeMap.push_back( this );
    callJNIVoidMethod( _jniMethods, JNIMethod_setJNITouchID, androidViewID );
#endif
}

NativeView ofxGenericButtonView::createNativeView( const ofRectangle& frame )
{
#if TARGET_OS_IPHONE
    // TODO: configurable type
    UIButton* newView = [ [ UIButton buttonWithType:UIButtonTypeRoundedRect ] retain ];
    [ newView setFrame:ofRectangleToCGRect( frame ) ];
    [ newView setTitleColor:[ UIColor blackColor ] forState:UIControlStateNormal ];
    [ newView setBackgroundColor:[ UIColor clearColor ] ];

    // TODO: doesn't match createUIView design
    _eventHandler = [ [ UIButtonDelegateForwarder alloc ] init ];
    [ _eventHandler setDelegate:this ];

    [ newView addTarget:_eventHandler action:@selector( touchCancel: ) forControlEvents: UIControlEventTouchCancel ];
    [ newView addTarget:_eventHandler action:@selector( touchDown: ) forControlEvents: UIControlEventTouchDown ];
    [ newView addTarget:_eventHandler action:@selector( touchDownRepeat: ) forControlEvents: UIControlEventTouchDownRepeat ];
    [ newView addTarget:_eventHandler action:@selector( touchDragEnter: ) forControlEvents: UIControlEventTouchDragEnter ];
    [ newView addTarget:_eventHandler action:@selector( touchDragExit: ) forControlEvents: UIControlEventTouchDragExit ];
    [ newView addTarget:_eventHandler action:@selector( touchDragInside: ) forControlEvents: UIControlEventTouchDragInside ];
    [ newView addTarget:_eventHandler action:@selector( touchDragOutside: ) forControlEvents: UIControlEventTouchDragOutside ];
    [ newView addTarget:_eventHandler action:@selector( touchUpInside: ) forControlEvents: UIControlEventTouchUpInside ];
    [ newView addTarget:_eventHandler action:@selector( touchUpOutside: ) forControlEvents:UIControlEventTouchUpOutside ];
    return newView;
#elif TARGET_ANDROID
    return ofxGenericView::createNativeView( frame );
#endif
}

void ofxGenericButtonView::setText( std::string newText )
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UIButton class ] ] )
    {
        UIButton* view = ( UIButton* )_view;
        [ view setTitle:ofxStringToNSString( newText ) forState:UIControlStateNormal ];
    }
#elif TARGET_ANDROID
    jstring newTextJNI = JNIUtility::CStringToJavaString( newText );
    callJNIVoidMethod( _jniMethods, JNIMethod_SetText, newTextJNI );
#endif
}

std::string ofxGenericButtonView::getText()
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UIButton class ] ] )
    {
        UIButton* view = ( UIButton* )_view;
        return ofxNSStringToString( [ view currentTitle ] );
    }
    return string();
#elif TARGET_ANDROID
    jstring jString = ( jstring )callJNIObjectMethod( _jniMethods, JNIMethod_GetText );
    return JNIUtility::JavaStringToCString( jString );
#else
    return std::string();
#endif
}

void ofxGenericButtonView::setBackgroundImage( std::string fileName )
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UIButton class ] ] )
    {
        UIButton* view = ( UIButton* )_view;
        [ view setBackgroundImage:[ UIImage imageWithContentsOfFile:pathToBundle( ofxStringToNSString( fileName ) )  ]
                         forState:UIControlStateNormal ];
    }
#elif TARGET_ANDROID
    callJNIVoidMethod(
    		_jniMethods,
    		JNIMethod_setBackgroundImage
    		//, fileName
    		);
#endif
}

#if TARGET_OS_IPHONE
ofxGenericUIViewCastOperator( ofxGenericButtonView, UIButton );
#elif TARGET_ANDROID

jclass ofxGenericButtonView::getJNIClassStatic()
{
	// TODO: handle exception
    if ( !_jniClass )
    	_jniClass = ( jclass )ofxGenericButtonView::createJNIReferenceStatic( JNIFindClass( ofxGenericButtonView::className ) );
    return _jniClass;
}

jclass ofxGenericButtonView::getJNIClass()
{
	return ofxGenericButtonView::getJNIClassStatic();
}

void ofxGenericButtonView::registerJNIMethods()
{
	ofxGenericView::registerJNIMethods();
	registerJNIMethodID(
			_jniMethods,
			false,
			JNIMethod_SetText,
			"setText",
			JNIEncodeMethodSignature( 1, JNIType_void, JNIType_object, "java/lang/String" )
			);
	registerJNIMethodID(
			_jniMethods,
			false,
			JNIMethod_GetText,
			"getText",
			JNIEncodeMethodSignature( 0, JNIType_object, "java/lang/String" )
			);
	registerJNIMethodID(
			_jniMethods,
			false,
			JNIMethod_setBackgroundImage,
			"setBackgroundImage",
			JNIEncodeMethodSignature( 0, JNIType_void )
//			JNIEncodeMethodSignature( 1, JNIType_void, JNIType_object, "java/lang/String" )
			);

    registerJNIMethodID(
    		_jniMethods,
    		false,
    		JNIMethod_setJNITouchID,
    		"setJNITouchID",
    		JNIEncodeMethodSignature( 1, JNIType_void, JNIType_int )
    		);

    registerJNIMethodID(
    		_jniMethods,
    		false,
    		JNIMethod_getJNITouchID,
    		"getJNITouchID",
    		JNIEncodeMethodSignature( 0, JNIType_int )
    		);
}
#endif

#define touchEventMethod( eventName ) \
void ofxGenericButtonView::eventName() \
{ \
    if ( _touchDelegate ) \
    { \
        ( _touchDelegate.lock() )->button_ ## eventName(); \
    } \
}

touchEventMethod( touchCancel );
touchEventMethod( touchDown );
touchEventMethod( touchDownRepeat );
touchEventMethod( touchDragEnter );
touchEventMethod( touchDragExit );
touchEventMethod( touchDragInside );
touchEventMethod( touchDragOutside );
touchEventMethod( touchUpInside );
touchEventMethod( touchUpOutside );

#if TARGET_OS_IPHONE
@implementation UIButtonDelegateForwarder

-( void )setDelegate:(ofxGenericButtonView *)setDelegate
{
    _delegate = setDelegate;
}

-( void )touchCancel:( id )sender
{
    _delegate->touchCancel();
}

-( void )touchDown:( id )sender
{
    _delegate->touchDown();
}

-( void )touchDownRepeat:( id )sender
{
    _delegate->touchDownRepeat();
}

-( void )touchDragEnter:( id )sender
{
    _delegate->touchDragEnter();
}

-( void )touchDragExit:( id )sender
{
    _delegate->touchDragExit();
}

-( void )touchDragInside:( id )sender
{
    _delegate->touchDragInside();
}

-( void )touchDragOutside:( id )sender
{
    _delegate->touchDragOutside();
}

-( void )touchUpInside:( id )sender
{
    _delegate->touchUpInside();
}

-( void )touchUpOutside:( id )sender
{
    _delegate->touchUpOutside();
}

@end
#elif TARGET_ANDROID

void ofxGenericButtonView::handleOnClick( int nativeID )
{
	if ( nativeID < _nativeMap.size() )
	{
		ofxGenericButtonView* buttonView = _nativeMap[ nativeID ];
		if ( buttonView )
		{
			buttonView->touchUpInside();
		} else
		{
			// TODO:
		}
	}
}

extern "C"
{
	void Java_cc_openframeworks_ofxGeneric_ButtonView_handleOnClickNative( JNIEnv* env, jobject thiz, jobject clickedView, jint id )
	{
		ofxGenericButtonView::handleOnClick( id );
	}
}

#endif
