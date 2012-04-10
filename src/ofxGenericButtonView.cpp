//
//  ofxGenericButtonView.cpp
//
//  Created by Ian Grossberg on 12/30/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#include "ofxGenericButtonView.h"
#include "ofxGenericUtility.h"



#if TARGET_OS_IPHONE
@interface ofxGenericButtonViewForwarder : NSObject 
{
@private
    ofxGenericButtonView* _delegate;
}
-( void )setDelegate:( ofxGenericButtonView* )setDelegate;

-( void )touchCancel:( id )sender;
-( void )touchDown:( id )sender;
-( void )touchDownRepeat:( id )sender;
-( void )touchDragEnter:( id )sender;
-( void )touchDragExit:( id )sender;
-( void )touchDragInside:( id )sender;
-( void )touchDragOutside:( id )sender;
-( void )touchUpInside:( id )sender;
-( void )touchUpOutside:( id )sender;

@end
#elif TARGET_ANDROID

#include "JNIUtility.h"

jclass ofxGenericButtonView::_jniClass = NULL;
const char* ofxGenericButtonView::className = "cc/openframeworks/ofxGeneric/ButtonView";

std::vector< ofxGenericButtonView* > ofxGenericButtonView::_nativeMap;
#endif

ofPtr< ofxGenericButtonView > ofxGenericButtonView::create( const ofRectangle& setFrame, ofPtrWeak< ofxGenericButtonViewTouchDelegate > touchDelegate )
{
    ofPtr< ofxGenericButtonView > create = ofPtr< ofxGenericButtonView >( new ofxGenericButtonView() );
    create->init( create, setFrame, touchDelegate );
    return create;
}

ofxGenericButtonView::ofxGenericButtonView()
{
}

ofxGenericButtonView::~ofxGenericButtonView()
{
#if TARGET_OS_IPHONE
    release( _forwarder );
#endif
}

void ofxGenericButtonView::init( ofPtrWeak< ofxGenericView > setThis, const ofRectangle& setFrame, ofPtrWeak< ofxGenericButtonViewTouchDelegate > touchDelegate )
{
    ofxGenericView::init( setThis, setFrame );
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
    _forwarder = [ [ ofxGenericButtonViewForwarder alloc ] init ];
    [ _forwarder setDelegate:this ];

    [ newView addTarget:_forwarder action:@selector( touchCancel: ) forControlEvents: UIControlEventTouchCancel ];
    [ newView addTarget:_forwarder action:@selector( touchDown: ) forControlEvents: UIControlEventTouchDown ];
    [ newView addTarget:_forwarder action:@selector( touchDownRepeat: ) forControlEvents: UIControlEventTouchDownRepeat ];
    [ newView addTarget:_forwarder action:@selector( touchDragEnter: ) forControlEvents: UIControlEventTouchDragEnter ];
    [ newView addTarget:_forwarder action:@selector( touchDragExit: ) forControlEvents: UIControlEventTouchDragExit ];
    [ newView addTarget:_forwarder action:@selector( touchDragInside: ) forControlEvents: UIControlEventTouchDragInside ];
    [ newView addTarget:_forwarder action:@selector( touchDragOutside: ) forControlEvents: UIControlEventTouchDragOutside ];
    [ newView addTarget:_forwarder action:@selector( touchUpInside: ) forControlEvents: UIControlEventTouchUpInside ];
    [ newView addTarget:_forwarder action:@selector( touchUpOutside: ) forControlEvents:UIControlEventTouchUpOutside ];
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

void ofxGenericButtonView::setEnabled( bool enabled )
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UIButton class ] ] )
    {
        UIButton* view = ( UIButton* )_view;
        [ view setEnabled:( BOOL )enabled ];
    }
#endif
}

bool ofxGenericButtonView::getEnabled()
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UIButton class ] ] )
    {
        UIButton* view = ( UIButton* )_view;
        return ( bool )view.enabled;
    }
#endif 
    return false;
}

#if DEBUG
string ofxGenericButtonView::toString()
{
    string result( ofxGenericView::toString() );
    
    char buffer[ 1024 ];
    snprintf( buffer, 1024, "Enabled: %d", getEnabled() );
    result += buffer;
    
#if TARGET_OS_IPHONE
    snprintf( buffer, 1024, " User Interaction Enabled %d", [ getNativeView() isUserInteractionEnabled ] );
    result += buffer;
#else
#endif
    
    return result;
}
#endif


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
        ( _touchDelegate.lock() )->button_ ## eventName( dynamic_pointer_cast< ofxGenericButtonView >( _this.lock() ) ); \
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
@implementation ofxGenericButtonViewForwarder

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
