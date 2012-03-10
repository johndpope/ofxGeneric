//
//  ofxGenericView.cpp
//
//  Created by Ian Grossberg on 12/29/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#include "ofxGenericView.h"
#include "ofxGenericUtility.h"


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
    
    willLoad();

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
    
    didLoad();
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
ofxUIGenericViewController* ofxGenericView::createUIViewController()
{
    return [ [ ofxUIGenericViewController alloc ] initWithDelegate:_this ];
}

ofxUIGenericViewController* ofxGenericView::getUIViewController()
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
        if ( true ) // TODO: only when attached to root
            add->willAppear();
        
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
        
        if ( true )
            add->didAppear();
    } else
    {
        // TODO:
    }
}

void ofxGenericView::removeChildView( ofPtr< ofxGenericView > remove )
{
    if ( remove )
    {
		if ( *( remove->_parent.lock() ) == *( _this.lock() ) )
		{
            if ( true )
                remove->willDisappear();
            
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
            
            if ( true )
                remove->didDisappear();
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

ofPtrWeak< ofxGenericView > ofxGenericView::getParent()
{
    return _parent;
}

void ofxGenericView::setAlpha( float alpha )
{
#if TARGET_OS_IPHONE
    if ( _view )
    {
        [ _view setAlpha:( CGFloat )alpha ];
    }
#endif
}

float ofxGenericView::getAlpha()
{
#if TARGET_OS_IPHONE
    if ( _view )
    {
        return ( float )_view.alpha;
    }
#endif    
    return 1.0f;
}

void ofxGenericView::setVisible( bool visible )
{    
#if TARGET_OS_IPHONE
    if ( _view )
    {
        [ _view setHidden:( BOOL )!visible ];
    }
#endif
}

bool ofxGenericView::getVisible()
{
#if TARGET_OS_IPHONE
    if ( _view )
    {
        return ( bool )!_view.isHidden;
    }
#endif
    return false;
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

-( id )initWithDelegate:( ofPtrWeak< ofxGenericView > ) delegate;
{
    self = [ super init ];
    if ( self )
    {
        _delegate = delegate;
    }
    return self;
}

-( void )dealloc
{
    [ super dealloc ];
}

#pragma mark Lifetime Events
/*
-( void )loadView
{
    if ( _delegate )
    {
        _delegate.lock()->willLoad();
    }
    [ super loadView ];
}

-( void )viewDidLoad
{
    [ super viewDidLoad ];
    if ( _delegate )
    {
        _delegate.lock()->didLoad();
    }
}

-( void )viewWillUnload
{
    [ super viewWillUnload ];
    if ( _delegate )
    {
        _delegate.lock()->willUnload();
    }
}

-( void )viewDidUnload
{
    [ super viewDidUnload ];
    if ( _delegate )
    {
        _delegate.lock()->didUnload();
    }
}

-( void )viewWillAppear:( BOOL )animated
{
    [ super viewWillAppear:animated ];
    if ( _delegate )
    {
        _delegate.lock()->willAppear();
    }
}

-( void )viewDidAppear:(BOOL)animated
{
    [ super viewDidAppear:animated ];
    if ( _delegate )
    {
        _delegate.lock()->didAppear();
    }
}

-( void )viewWillDisappear:(BOOL)animated
{
    [ super viewWillDisappear:animated ];
    if ( _delegate )
    {
        _delegate.lock()->willDisappear();
    }
}

-( void )viewDidDisappear:(BOOL)animated
{
    [ super viewDidDisappear:animated ];
    if ( _delegate )
    {
        _delegate.lock()->didDisappear();
    }
}
*/
@end
#endif


