//
//  ofxGenericButtonView.cpp
//
//  Created by Ian Grossberg on 12/30/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#include "ofxGenericButtonView.h"

#include "ofxGenericUtility.h"
#include "ofxGenericTextView.h"

#include "ofxGenericImage.h"

#include "ofxGenericApp.h"

#import "ofCommon.h"

#if TARGET_OS_IPHONE
@interface ofxGenericButtonViewForwarder : NSObject 
{
@private
    ofxGenericButtonView* _delegate;
}
-( void )setDelegate:( ofxGenericButtonView* )setDelegate;

-( void )touchCancel:( id )sender withEvent:( UIEvent *)event;
-( void )touchDown:( id )sender withEvent:( UIEvent *)event;
-( void )touchDownRepeat:( id )sender withEvent:( UIEvent *)event;
-( void )touchDragEnter:( id )sender withEvent:( UIEvent *)event;
-( void )touchDragExit:( id )sender withEvent:( UIEvent *)event;
-( void )touchDragInside:( id )sender withEvent:( UIEvent *)event;
-( void )touchDragOutside:( id )sender withEvent:( UIEvent *)event;
-( void )touchUpInside:( id )sender withEvent:( UIEvent *)event;
-( void )touchUpOutside:( id )sender withEvent:( UIEvent *)event;

@end

#elif TARGET_ANDROID

#include "JNIUtility.h"

jclass ofxGenericButtonView::_jniClass = NULL;
const char* ofxGenericButtonView::className = "cc/openframeworks/ofxGeneric/ButtonView";

std::vector< ofxGenericButtonView* > ofxGenericButtonView::_nativeMap;
#endif

#if TARGET_OS_IPHONE
static Class nativeClass = Nil;
#endif

ofPtr< ofxGenericButtonView > ofxGenericButtonView::create( ofxGenericButtonType buttonType, const ofRectangle& setFrame, ofPtrWeak< ofxGenericButtonViewDelegate > touchDelegate )
{
    ofPtr< ofxGenericButtonView > create = ofPtr< ofxGenericButtonView >( new ofxGenericButtonView() );
    create->init( create, buttonType, setFrame, touchDelegate );
    return create;
}

#if TARGET_OS_IPHONE
void ofxGenericButtonView::setNativeViewClass( Class nativeViewClass )
{
    if ([nativeViewClass isSubclassOfClass:[UIButton class]])
    {
        nativeClass = nativeViewClass;
    }
}
#endif

ofxGenericButtonView::ofxGenericButtonView()
{
}

ofxGenericButtonView::~ofxGenericButtonView()
{
#if TARGET_OS_IPHONE
    [ _forwarder release ];
    _forwarder = nil;
#endif
}

//stupid hack to add another param to createNativeView
static ofxGenericButtonType lastButtonType = ofxGenericButtonTypeRoundedRect;
void ofxGenericButtonView::init( ofPtrWeak< ofxGenericView > setThis, ofxGenericButtonType buttonType, const ofRectangle& setFrame, ofPtrWeak< ofxGenericButtonViewDelegate > touchDelegate )
{
    lastButtonType = buttonType;
    ofxGenericView::init( setThis, setFrame );
    setDelegate( touchDelegate );

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
     Class labelClass = nativeClass ? nativeClass : [UIButton class];
    UIButton* newView = [ [ labelClass buttonWithType:ofxGenericButtonTypeToiOS(lastButtonType) ] retain ];
    [ newView setFrame:ofxRectangleToCGRect( frame ) ];
    [ newView setTitleColor:[ UIColor blackColor ] forState:UIControlStateNormal ];
    [ newView setBackgroundColor:[ UIColor clearColor ] ];
    
    _textView = ofxGenericTextView::create( ofxCGRectToofRectangle( [ [ newView titleLabel ] frame ] ), [ newView titleLabel ] );

    // TODO: doesn't match createUIView design
    _forwarder = [ [ ofxGenericButtonViewForwarder alloc ] init ];
    [ _forwarder setDelegate:this ];

    [ newView addTarget:_forwarder action:@selector( touchCancel:withEvent: ) forControlEvents: UIControlEventTouchCancel ];
    [ newView addTarget:_forwarder action:@selector( touchDown:withEvent: ) forControlEvents: UIControlEventTouchDown ];
    [ newView addTarget:_forwarder action:@selector( touchDownRepeat:withEvent: ) forControlEvents: UIControlEventTouchDownRepeat ];
    [ newView addTarget:_forwarder action:@selector( touchDragEnter:withEvent: ) forControlEvents: UIControlEventTouchDragEnter ];
    [ newView addTarget:_forwarder action:@selector( touchDragExit:withEvent: ) forControlEvents: UIControlEventTouchDragExit ];
    [ newView addTarget:_forwarder action:@selector( touchDragInside:withEvent: ) forControlEvents: UIControlEventTouchDragInside ];
    [ newView addTarget:_forwarder action:@selector( touchDragOutside:withEvent: ) forControlEvents: UIControlEventTouchDragOutside ];
    [ newView addTarget:_forwarder action:@selector( touchUpInside:withEvent: ) forControlEvents: UIControlEventTouchUpInside ];
    [ newView addTarget:_forwarder action:@selector( touchUpOutside:withEvent: ) forControlEvents:UIControlEventTouchUpOutside ];
    return newView;
#elif TARGET_ANDROID
    return ofxGenericView::createNativeView( frame );
#endif
}

void ofxGenericButtonView::setText( std::string newText )
{
    if ( _textView )
    {
        _textView->setText( newText );
    }
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
    if ( _textView )
    {
        return _textView->getText();
    }
    return std::string();
#elif TARGET_ANDROID
    jstring jString = ( jstring )callJNIObjectMethod( _jniMethods, JNIMethod_GetText );
    return JNIUtility::JavaStringToCString( jString );
#else
    return std::string();
#endif
}

void ofxGenericButtonView::setBackgroundImage( std::string fileName )
{
    setBackgroundImage( ofxGenericImage::create( fileName ) );
}

void ofxGenericButtonView::setBackgroundImage( ofPtr< ofxGenericImage > image )
{
    _backgroundImage = image;
    if ( !_backgroundImage )
    {
        ofxGLogError( "Unable to set image - provided ofxGenericImage is null, cannot ofxGenericButtonView::setBackgroundImage!" );
    }
    
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UIButton class ] ] )
    {
        UIImage* uiImage = nil;
        if ( _backgroundImage )
        {
            uiImage = _backgroundImage->getUIImage();
        }
        UIButton* view = ( UIButton* )_view;
        [ view setBackgroundImage: uiImage forState:UIControlStateNormal ];
    }
#elif TARGET_ANDROID
#endif
}

void ofxGenericButtonView::setDownBackgroundImage( std::string fileName )
{
    setDownBackgroundImage( ofxGenericImage::create( fileName ));
}

void ofxGenericButtonView::setDownBackgroundImage( ofPtr< ofxGenericImage > image )
{
    _downBackgroundImage = image;
    if ( !_downBackgroundImage )
    {
        ofxGLogError( "Unable to set image - provided ofxGenericImage is null, cannot ofxGenericButtonView::setDownBackgroundImage!" );
    }
    
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UIButton class ] ] )
    {
        UIImage* uiImage = nil;
        if ( _downBackgroundImage )
        {
            uiImage = _downBackgroundImage->getUIImage();
        }
        UIButton* view = ( UIButton* )_view;
        [ view setBackgroundImage: uiImage forState:UIControlStateHighlighted ];
    }
#elif TARGET_ANDROID
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

void ofxGenericButtonView::setDelegate( ofPtrWeak< ofxGenericButtonViewDelegate > delegate )
{
    _touchDelegate = delegate;
}

ofPtrWeak< ofxGenericButtonViewDelegate > ofxGenericButtonView::getDelegate()
{
    return _touchDelegate;
}

void ofxGenericButtonView::setContentHorizontalAlignment( ofxGenericContentHorizontalAlignment alignment )
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UIButton class ] ] )
    {
        UIButton* view = ( UIButton* )_view;
        view.contentHorizontalAlignment = ofxGenericContentHorizontalAlignmentToiOS(alignment);
    }
#endif
}

ofxGenericContentHorizontalAlignment ofxGenericButtonView::getContentHorizontalAlignment()
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UIButton class ] ] )
    {
        UIButton* view = ( UIButton* )_view;
        return iOSToofxGenericContentHorizontalAlignment(view.contentHorizontalAlignment);
    }
#endif
    return ofxGenericContentHorizontalAlignmentLeft;
}

void ofxGenericButtonView::setTextAlignment( ofxGenericTextHorizontalAlignment alignment )
{
    if ( _textView )
    {
        _textView->setTextAlignment( alignment );
    }
}

ofxGenericTextHorizontalAlignment ofxGenericButtonView::getTextAlignment()
{
    if ( _textView )
    {
        return _textView->getTextAlignment();
    }
    return ofxGenericTextHorizontalAlignmentLeft;
}

void ofxGenericButtonView::setTextColor ( const ofColor& setColor )
{
    if ( _textView )
    {
        _textView->setTextColor( setColor );
    }
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UIButton class ] ] )
    {
        [ ( ( UIButton* ) _view) setTitleColor:ofxColorToUIColor( setColor ) forState:UIControlStateNormal ];
    }
#endif    
}

ofColor ofxGenericButtonView::getTextColor ()
{
    if ( _textView )
    {
        return _textView->getTextColor();
    }
    return ofColor::black;
}

void ofxGenericButtonView::setDownTextColor ( const ofColor& setColor )
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[UIButton class]] )
    {
        [((UIButton *) _view) setTitleColor:ofxColorToUIColor( setColor ) forState:UIControlStateHighlighted];
    }
#endif
}

ofColor ofxGenericButtonView::getDownTextColor ()
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[UIButton class]] )
    {
        return ofxUIColorToofColor( [(UIButton *)_view titleColorForState:UIControlStateHighlighted] );
    }
#endif
    return ofColor::black;
}

void ofxGenericButtonView::setLineBreakMode ( ofxGenericTextLinebreakMode mode )
{
    if ( _textView )
    {
        _textView->setLineBreakMode( mode );
    }
}

ofxGenericTextLinebreakMode ofxGenericButtonView::getLineBreakMode ()
{
    if ( _textView )
    {
        return _textView->getLineBreakMode();
    }
    return ofxGenericTextLinebreakModeClip;
}

void ofxGenericButtonView::setNumberOfLines ( int n )
{
    if ( _textView )
    {
        _textView->setNumberOfLines( n );
    }
}

int ofxGenericButtonView::getNumberOfLines ()
{
    if ( _textView )
    {
        return _textView->getNumberOfLines();
    }
    return 1;
}

float ofxGenericButtonView::getMinimumFontSize()
{
    if ( _textView )
    {
        return _textView->getMinimumFontSize();
    }
    return 10;
}

void ofxGenericButtonView::setMinimumFontSize( float s )
{
    if ( _textView )
    {
        _textView->setMinimumFontSize( s );
    }
}

void ofxGenericButtonView::setFont( string name, float size )
{
    if ( _textView )
    {
        _textView->setFont( name, size );
    }
}

float ofxGenericButtonView::getFontSize()
{
    if ( _textView )
    {
        return _textView->getFontSize();
    }
    return 12.0f;
}

string ofxGenericButtonView::getFontName()
{
    if ( _textView )
    {
        return _textView->getFontName();
    }
    return "";
}

string ofxGenericButtonView::toString()
{
    string result( ofxGenericView::toString() );
    
    result += ofxGSPrintf( " Enabled: %s", ofxGToString( getEnabled() ) );
    
    if ( !getText().empty() )
    {
        result += " " + getText();
    }
#if DEBUG
    if ( _backgroundImage && !_backgroundImage->getFilePath().empty() )
    {
        // IOSP-40 // result += " " + ofFilePath::getFileName( _backgroundImage->getFilePath() );
    }
#endif
    return result;
}

ofPtr< ofxGenericValueStore > ofxGenericButtonView::dumpViewGraphAsValueStore()
{
    ofPtr< ofxGenericValueStore > result = ofxGenericView::dumpViewGraphAsValueStore();
    
    if ( _textView )
    {
        result->write( "textChild", _textView->dumpViewGraphAsValueStore() );
    }
    
    return result;
}

ofPtr< ofxGenericValueStore > ofxGenericButtonView::toValueStore()
{
    ofPtr< ofxGenericValueStore > result = ofxGenericView::toValueStore();
    if ( result )
    {
        result->write( "enabled", getEnabled() );
        if ( !getText().empty() )
        {
            result->write( "text", getText() );
        }
#if DEBUG
        if ( _backgroundImage && !_backgroundImage->getFilePath().empty() )
        {
            result->write( "background image", _backgroundImage->getFilePath() );
        }
#endif
        if ( ofxGenericApp::getInstance() && ofxGenericApp::getInstance()->getRootView() )
        {
            result->write( "textFrame", ofxGToString( getTextArea() ) );
            result->write( "textFrame (Absolute)", ofxGToString( ofxGenericApp::getInstance()->getRootView()->convertFrom( getTextArea(), _this.lock() ) ) );
        }
    }

    return result;
}

void ofxGenericButtonView::copyProperties( ofPtr< ofxGenericView > from )
{
    ofxGenericView::copyProperties( from );
    
    ofPtr< ofxGenericButtonView > fromButton = dynamic_pointer_cast< ofxGenericButtonView >( from );
    if ( fromButton )
    {
        setText( fromButton->getText() );
     //   setBackgroundImage( fromButton->getBackgroundImage() );
     //   setDownBackgroundImage( fromButton->getBackgroundImage() );
        setTextAlignment( fromButton->getTextAlignment() );
        setTextColor( fromButton->getTextColor() );
        setDownTextColor( fromButton->getDownTextColor() );
        setLineBreakMode( fromButton->getLineBreakMode() );
        setNumberOfLines( fromButton->getNumberOfLines() );
        setMinimumFontSize( fromButton->getMinimumFontSize() );
        setFont( fromButton->getFontName(), fromButton->getFontSize() );
        setEnabled( fromButton->getEnabled() );
        setDelegate( fromButton->getDelegate() );
    }
}

ofRectangle ofxGenericButtonView::getTextArea()
{
    ofRectangle result;
    
    if ( _textView )
    {
        result = _textView->getFrame();
    }
    
    return result;
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
void ofxGenericButtonView::eventName( ofPoint position ) \
{ \
    if ( _touchDelegate ) \
    { \
        ( _touchDelegate.lock() )->button_ ## eventName( dynamic_pointer_cast< ofxGenericButtonView >( _this.lock() ) ); \
        ( _touchDelegate.lock() )->button_ ## eventName( dynamic_pointer_cast< ofxGenericButtonView >( _this.lock() ), position ); \
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

- ( ofPoint ) getTouchPos:( UIEvent * )event
{
    //check the class because of a strange Crashlytics bug where iOS 7 seems to send a UIButton instead of an event
    if ( [event isKindOfClass: [UIEvent class]] && [ event.allTouches count ] > 0 )
    {
        UITouch *touch = [ event.allTouches anyObject ];
        CGPoint point = [ touch locationInView:touch.view ];
        return ofPoint( point.x, point.y );
    }
    return ofPoint( -1, -1 );
}

-( void )touchCancel:( id )sender withEvent:( UIEvent * )event
{
    _delegate->touchCancel( [ self getTouchPos:event ] );
}

-( void )touchDown:( id )sender withEvent:( UIEvent * )event
{
    _delegate->touchDown( [ self getTouchPos:event ] );
}

-( void )touchDownRepeat:( id )sender withEvent:( UIEvent * )event
{
    _delegate->touchDownRepeat( [ self getTouchPos:event ] );
}

-( void )touchDragEnter:( id )sender withEvent:( UIEvent * )event
{
    _delegate->touchDragEnter( [ self getTouchPos:event ] );
}

-( void )touchDragExit:( id )sender withEvent:( UIEvent * )event
{
    _delegate->touchDragExit( [ self getTouchPos:event ] );
}

-( void )touchDragInside:( id )sender withEvent:( UIEvent * )event
{
    _delegate->touchDragInside( [ self getTouchPos:event ] );
}

-( void )touchDragOutside:( id )sender withEvent:( UIEvent * )event
{
    _delegate->touchDragOutside( [ self getTouchPos:event ] );
}

-( void )touchUpInside:( id )sender withEvent:( UIEvent * )event
{
    _delegate->touchUpInside( [ self getTouchPos:event ] );
}

-( void )touchUpOutside:( id )sender withEvent:( UIEvent * )event
{
    _delegate->touchUpOutside( [ self getTouchPos:event ] );
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