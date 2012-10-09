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

//can't subclass because Apple doesn't expose types to buttons... WTF
#define UIButtonForwardingBSTag 535353
@interface UIButton (Forwarding)
@end

@implementation UIButton (Forwarding)

//not working, even though it should be
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesBegan:touches withEvent:event];
    
    if ( self.tag == UIButtonForwardingBSTag && self.nextResponder )
    {
        [self.nextResponder touchesBegan:touches withEvent:event];
    }
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesEnded:touches withEvent:event];
    
    if ( self.tag == UIButtonForwardingBSTag && self.nextResponder )
    {
        [self.nextResponder touchesEnded:touches withEvent:event];
    }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesMoved:touches withEvent:event];
    
    if ( self.tag == UIButtonForwardingBSTag && self.nextResponder )
    {
        [self.nextResponder touchesMoved:touches withEvent:event];
    }
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesCancelled:touches withEvent:event];
    
    if ( self.tag == UIButtonForwardingBSTag && self.nextResponder )
    {
        [self.nextResponder touchesCancelled:touches withEvent:event];
    }
}

@end

#elif TARGET_ANDROID

#include "JNIUtility.h"

jclass ofxGenericButtonView::_jniClass = NULL;
const char* ofxGenericButtonView::className = "cc/openframeworks/ofxGeneric/ButtonView";

std::vector< ofxGenericButtonView* > ofxGenericButtonView::_nativeMap;
#endif

ofPtr< ofxGenericButtonView > ofxGenericButtonView::create( ofxGenericButtonType buttonType, const ofRectangle& setFrame, ofPtrWeak< ofxGenericButtonViewTouchDelegate > touchDelegate, bool forwardsTouches )
{
    ofPtr< ofxGenericButtonView > create = ofPtr< ofxGenericButtonView >( new ofxGenericButtonView() );
    create->init( create, buttonType, setFrame, touchDelegate, forwardsTouches );
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

//stupid hack to add another param to createNativeView
static ofxGenericButtonType lastButtonType = ofxGenericButtonTypeRoundedRect;
static bool lastForwardsTouchesValue = false;
void ofxGenericButtonView::init( ofPtrWeak< ofxGenericView > setThis, ofxGenericButtonType buttonType, const ofRectangle& setFrame, ofPtrWeak< ofxGenericButtonViewTouchDelegate > touchDelegate, bool forwardsTouches )
{
    lastButtonType = buttonType;
    lastForwardsTouchesValue = forwardsTouches;
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
    UIButton* newView = [ [ UIButton buttonWithType:ofxGenericButtonTypeToiOS(lastButtonType) ] retain ];
    newView.tag = lastForwardsTouchesValue ? UIButtonForwardingBSTag : 0;
    [ newView setFrame:ofRectangleToCGRect( frame ) ];
    [ newView setTitleColor:[ UIColor blackColor ] forState:UIControlStateNormal ];
    [ newView setBackgroundColor:[ UIColor clearColor ] ];
    
    _textView = ofxGenericTextView::create( CGRectToofRectangle( [ [ newView titleLabel ] frame ] ), [ newView titleLabel ] );

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
    string imagePath = ofxGenericImage::getNativeImagePath( fileName );
    if ( ofxGFileExists( imagePath ) )
    {
#if TARGET_OS_IPHONE
        if ( [ _view isKindOfClass:[ UIButton class ] ] )
        {
            UIButton* view = ( UIButton* )_view;
            [ view setBackgroundImage:[ UIImage imageWithContentsOfFile:ofxStringToNSString( ofxGPathToDataFolder( fileName ) )  ]
                             forState:UIControlStateNormal ];
        }
#elif TARGET_ANDROID
        callJNIVoidMethod(
                _jniMethods,
                JNIMethod_setBackgroundImage
                //, fileName
                );
#endif
    } else
    {
        ofxGLogError( "Unable to find image file " + imagePath + ", cannot ofxGenericButtonView::setBackgroundImage!" );
    }
}

void ofxGenericButtonView::setDownBackgroundImage( std::string fileName )
{
    string imagePath = ofxGenericImage::getNativeImagePath( fileName );
    if ( ofxGFileExists( imagePath ) )
    {
#if TARGET_OS_IPHONE
        if ( [ _view isKindOfClass:[ UIButton class ] ] )
        {
            UIButton* view = ( UIButton* )_view;
            [ view setBackgroundImage:[ UIImage imageWithContentsOfFile:ofxStringToNSString( ofxGPathToDataFolder( fileName ) )  ]
                             forState:UIControlStateHighlighted ];
        }
#elif TARGET_ANDROID
        callJNIVoidMethod(
                          _jniMethods,
                          JNIMethod_setDownBackgroundImage
                          //, fileName
                          );
#endif
} else
{
    ofxGLogError( "Unable to find image file " + imagePath + ", cannot ofxGenericButtonView::setDownBackgroundImage!" );
}
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

void ofxGenericButtonView::setDelegate( ofPtrWeak< ofxGenericButtonViewTouchDelegate > delegate )
{
    _touchDelegate = delegate;
}

ofPtrWeak< ofxGenericButtonViewTouchDelegate > ofxGenericButtonView::getDelegate()
{
    return _touchDelegate;
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
        [ ( ( UIButton* ) _view) setTitleColor:ofColorToUIColor( setColor ) forState:UIControlStateNormal ];
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
        [((UIButton *) _view) setTitleColor:ofColorToUIColor( setColor ) forState:UIControlStateHighlighted];
    }
#endif
}

ofColor ofxGenericButtonView::getDownTextColor ()
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[UIButton class]] )
    {
        return UIColorToofColor( [(UIButton *)_view titleColorForState:UIControlStateHighlighted] );
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

void ofxGenericButtonView::setForwardsTouches( bool forwards )
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[UIButton class]] )
    {
        UIButton *v = (UIButton *) _view;
        v.tag = forwards ? UIButtonForwardingBSTag : 0;
    }
#endif
}

#if DEBUG
string ofxGenericButtonView::toString()
{
    string result( ofxGenericView::toString() );
    
    result += ofxGSPrintf( " Enabled: %s", ofxGToString( getEnabled() ) );
    
#if TARGET_OS_IPHONE
    result += ofxGSPrintf( " User Interaction Enabled: %s", ofxGToString( [ getNativeView() isUserInteractionEnabled ] ) );
#endif
    
    return result;
}
#endif

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
     //   setForwardsTouches( fromButton->getForwardsTouches() );
        setEnabled( fromButton->getEnabled() );
        setDelegate( fromButton->getDelegate() );
    }
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
