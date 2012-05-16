//
//  ofxGenericView.cpp
//
//  Created by Ian Grossberg on 12/29/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#include "ofxGenericView.h"
#include "ofxGenericUtility.h"
#include "ofxGenericApp.h"

#if TARGET_OS_IPHONE

@interface ofxGenericViewAnimationForwarder : NSObject
{
    ofPtrWeak< ofxGenericViewDelegate > _delegate;
}
- ( id )initWithDelegate:( ofPtrWeak< ofxGenericViewDelegate > )delegate;
- (void)animationWillStart:( NSString* )animationID finished:( NSNumber* )finished context:( void* )context;
- (void)animationDidStop:( NSString* )animationID finished:( NSNumber* )finished context:( void* )context;

@end

@interface ofxGenericGestureForwarder : NSObject
{
    ofPtrWeak< ofxGenericViewDelegate > _delegate;
    int _type;
}
- ( id )initWithDelegate:( ofPtrWeak< ofxGenericViewDelegate > )delegate type:(int)type;
- (void)gesturePerformed:( UIGestureRecognizer* )recognizer;

@end

#elif TARGET_ANDROID
#include "JNIUtility.h"
#include "JNIRect.h"

jclass ofxGenericView::_jniClass = NULL;
const char* ofxGenericView::className = "cc/openframeworks/ofxGeneric/View";
#endif

ofPtr< ofxGenericView > ofxGenericView::create( const ofRectangle& setFrame, NativeView nativeView )
{
    ofPtr< ofxGenericView > create = ofPtr< ofxGenericView >( new ofxGenericView() );
    create->init( create, setFrame, nativeView );
    return create;
}

ofxGenericView::ofxGenericView()
:
#if TARGET_OS_IPHONE
 _view( nil ), _viewController( nil ),
#elif TARGET_ANDROID
 _view( NULL ), 
#endif
_isAttachedToRoot( false ), _children()
{
}

ofxGenericView::~ofxGenericView()
{
    removeFromParent();

#if TARGET_OS_IPHONE
    releaseView( _view );
    releaseViewController( _viewController );
    [gestureForwarders release];
    gestureForwarders = nil;
#elif TARGET_ANDROID
	destroyJNIReference( _view );
#endif
}

void ofxGenericView::init( ofPtrWeak< ofxGenericView > setThis, const ofRectangle& setFrameValue, NativeView nativeView )
{
    _this = setThis;

#if TARGET_ANDROID
    registerJNIMethods();
#endif
    
    willLoad();

    if ( nativeView )
    {
        _view = nativeView;
#if TARGET_OS_IPHONE
        [ _view retain ];
#endif
        setFrame( setFrameValue );
    } else 
    {
        _view = createNativeView( setFrameValue );
    }
    
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
    
#if TARGET_OS_IPHONE
    gestureForwarders = [[NSMutableArray array] retain];
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

#if TARGET_OS_IPHONE
UIView* ofxGenericView::allocNativeView( const ofRectangle& setFrame )
{
    return [ [ UIView alloc ] initWithFrame:ofRectangleToCGRect( setFrame ) ];
}
#elif TARGET_ANDROID
jobject ofxGenericView::allocNativeView( const ofRectangle& frame )
{
    return createJNIInstance( _jniMethods, JNIMethod_constructor );
}
#endif

NativeView ofxGenericView::createNativeView( const ofRectangle& setFrame )
{
#if TARGET_OS_IPHONE
	UIView* newView = allocNativeView( setFrame );

	// TODO: move to shared code override-able function after _view assigned? or just assign _view in here
    [ newView setBackgroundColor:[ UIColor whiteColor ] ];
    [ newView setOpaque:YES ];
    [ newView setHidden:NO ];
    [ newView setClipsToBounds:YES ];

    return newView;

#elif TARGET_ANDROID
    // TODO: exception handling
    jobject newView = allocNativeView( setFrame );
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
    addChildViewTo( _this.lock(), add );
}

void ofxGenericView::addChildView( ofPtr< ofxGenericView > add, ofPtr< ofxGenericView > before )
{
    addChildViewTo( _this.lock(), add, before );
}

void ofxGenericView::addChildViewTo( ofPtr< ofxGenericView > parent, ofPtr< ofxGenericView > add )
{
    if ( add )
    {
        if ( _isAttachedToRoot )
        {
            add->willAppear();
        }
        
        parent->_children.push_back( add );
        NativeView nativeView = add->getNativeView();
        if ( nativeView )
        {
#if TARGET_OS_IPHONE
          	[ parent->_view addSubview:nativeView ];
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
        add->_parent = parent;
        
        if ( _isAttachedToRoot )
        {
            add->setIsAttachedToRoot( true );
            add->didAppear();
        }
    } else
    {
        // TODO:
    }
}

void ofxGenericView::addChildViewTo( ofPtr< ofxGenericView > parent, ofPtr< ofxGenericView > add, ofPtr< ofxGenericView > before )
{
    if ( add )
    {
        if ( before && before->getParent().lock() == parent )
        {
            if ( _isAttachedToRoot )
            {
                add->willAppear();
            }

            std::list< ofPtr< ofxGenericView > >::iterator findIndex = parent->_children.begin();
            while ( findIndex != parent->_children.end() && ( *findIndex ) != before )
            {
                findIndex ++;
            }
            // TODO: extra error check
            parent->_children.insert( findIndex, add );
            
            NativeView addNativeView = add->getNativeView();
            NativeView beforeNativeView = before->getNativeView();
            if ( addNativeView )
            {
#if TARGET_OS_IPHONE
                [ parent->_view insertSubview:addNativeView belowSubview:beforeNativeView ];
#elif TARGET_ANDROID
#endif
            } else
            {
                // TODO:
            }
            add->_parent = parent;
            
            if ( _isAttachedToRoot )
            {
                add->setIsAttachedToRoot( true );
                add->didAppear();
            }
        } else 
        {
            addChildViewTo( parent, add );
        }
    } else
    {
        // TODO:
    }    
}

void ofxGenericView::removeChildView( ofPtr< ofxGenericView > remove )
{
    removeChildViewFrom( _this.lock(), remove );
}

void ofxGenericView::removeChildViewFrom( ofPtr< ofxGenericView > parent, ofPtr< ofxGenericView > remove )
{
    if ( remove && parent && remove->_parent.lock() == parent )
    {
		if ( *( remove->_parent.lock() ) == *( parent ) )
		{
            if ( _isAttachedToRoot )
            {
                remove->willDisappear();
            }
            
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
			parent->_children.remove( remove );
			remove->_parent = ofPtrWeak< ofxGenericView >();
            
            if ( _isAttachedToRoot )
            {
                remove->setIsAttachedToRoot( false );
                remove->didDisappear();
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

void ofxGenericView::setIsAttachedToRoot( bool attached )
{
    _isAttachedToRoot = attached;
}

void ofxGenericView::removeFromParent()
{
    if ( _parent )
    {
        ofPtr< ofxGenericView > parentPtr = _parent.lock();
        parentPtr->removeChildView( _this.lock() );
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

void ofxGenericView::setClipSubviews( bool clip )
{
#if TARGET_OS_IPHONE
    if ( _view )
    {
        [ _view setClipsToBounds:( BOOL )clip ];
    }
#endif
}

bool ofxGenericView::getClipSubviews()
{
#if TARGET_OS_IPHONE
    if ( _view )
    {
        return ( bool )_view.clipsToBounds;
    }
#endif    
    return false;
}

void ofxGenericView::setAutoresizingMask( int autoresizing )
{
#if TARGET_OS_IPHONE
    if ( _view )
    {
        [ _view setAutoresizingMask:ofxGenericViewAutoresizingToiOS( ( ofxGenericViewAutoresizing )autoresizing ) ];
    }
#endif
}

void ofxGenericView::setNextResponder( ofPtrWeak< ofxGenericView > view )
{
    _nextResponder = view;
}

ofPtr< ofxGenericView > ofxGenericView::getNextResponder()
{
    return _nextResponder.lock();
}

void ofxGenericView::willLoad()
{
    if ( _viewDelegate )
    {
        _viewDelegate.lock()->willLoad( _this.lock() );
    }
};

void ofxGenericView::didLoad()
{
    if ( _viewDelegate )
    {
        _viewDelegate.lock()->didLoad( _this.lock() );
    }
};

void ofxGenericView::willAppear()
{
    if ( _viewDelegate )
    {
        _viewDelegate.lock()->willAppear( _this.lock() );
    }
    
    for( std::list< ofPtr< ofxGenericView > >::iterator travChildren = _children.begin(); travChildren != _children.end(); travChildren ++ )
    {
        ( *travChildren )->willAppear();
    }
};

void ofxGenericView::didAppear()
{
    setIsAttachedToRoot( true );
    
    if ( _viewDelegate )
    {
        _viewDelegate.lock()->didAppear( _this.lock() );
    }
    
    for( std::list< ofPtr< ofxGenericView > >::iterator travChildren = _children.begin(); travChildren != _children.end(); travChildren ++ )
    {
        ( *travChildren )->didAppear();
    }
};

void ofxGenericView::willDisappear()
{
    if ( _viewDelegate )
    {
        _viewDelegate.lock()->willDisappear( _this.lock() );
    }

    for( std::list< ofPtr< ofxGenericView > >::iterator travChildren = _children.begin(); travChildren != _children.end(); travChildren ++ )
    {
        ( *travChildren )->willDisappear();
    }
};

void ofxGenericView::didDisappear()
{
    setIsAttachedToRoot( false );
    
    if ( _viewDelegate )
    {
        _viewDelegate.lock()->didDisappear( _this.lock() );
    }

    for( std::list< ofPtr< ofxGenericView > >::iterator travChildren = _children.begin(); travChildren != _children.end(); travChildren ++ )
    {
        ( *travChildren )->didDisappear();
    }
};

void ofxGenericView::setViewDelegate( ofPtrWeak< ofxGenericViewDelegate > delegate )
{
    _viewDelegate = delegate;
}

void ofxGenericView::beginAnimation( string animationId, ofPtr< ofxGenericViewDelegate > delegate )
{
#if TARGET_OS_IPHONE
    [ UIView beginAnimations:ofxStringToNSString( animationId ) context:nil ];
    if ( delegate )
    {
        ofxGenericViewAnimationForwarder* forwarder = [ [ ofxGenericViewAnimationForwarder alloc ] initWithDelegate:delegate ];
        [ UIView setAnimationDelegate:forwarder ];
        [ UIView setAnimationWillStartSelector:@selector( animationWillStart:finished:context: ) ];
        [ UIView setAnimationDidStopSelector:@selector( animationDidStop:finished:context: ) ];
        [ forwarder release ];
    }
#elif TARGET_ANDROID
#endif
}

void ofxGenericView::commitAnimation()
{
#if TARGET_OS_IPHONE
    [ UIView commitAnimations ];
#elif TARGET_ANDROID
#endif
}

void ofxGenericView::setAnimationDuration( double seconds )
{
#if TARGET_OS_IPHONE
    [ UIView setAnimationDuration:( NSTimeInterval )seconds ];
#elif TARGET_ANDROID
#endif
}

void ofxGenericView::setAnimationDelay( double seconds )
{
#if TARGET_OS_IPHONE
    [ UIView setAnimationDelay:( NSTimeInterval )seconds ];
#elif TARGET_ANDROID
#endif
}

void ofxGenericView::setAnimationCurve( ofxGenericViewAnimationCurve curve )
{
#if TARGET_OS_IPHONE
    [ UIView setAnimationCurve:ofxGenericViewAnimationCurveToiOS( curve ) ];
#elif TARGET_ANDROID
#endif    
}

void ofxGenericView::setAnimationTransition( ofxGenericViewAnimationTransition transition, ofPtr< ofxGenericView > forView )
{
#if TARGET_OS_IPHONE
    [ UIView setAnimationTransition:ofxGenericViewAnimationTransitionToiOS( transition ) forView:forView->getNativeView() cache:NO ];
#elif TARGET_ANDROID
#endif    
}

void ofxGenericView::replaceViewWithView( ofPtr< ofxGenericView > replace, ofPtr< ofxGenericView > with )
{
    if ( replace && replace->getParent() && with )
    {
        ofPtr< ofxGenericView > parent = replace->getParent().lock();
        // TODO: match autosizing?
        with->setFrame( replace->getFrame() );
        parent->addChildView( with, replace );
        parent->removeChildView( replace );
    }
}

void ofxGenericView::addGestureRecognizerSwipe( ofxGenericGestureTypeSwipe type, ofPtrWeak< ofxGenericViewDelegate > delegate )
{
#if TARGET_OS_IPHONE
    ofxGenericGestureForwarder *forwarder = [[[ofxGenericGestureForwarder alloc] initWithDelegate:delegate type:(int)type] autorelease];
    [gestureForwarders addObject:forwarder];
    UISwipeGestureRecognizer *swipeRecognizer = [[[UISwipeGestureRecognizer alloc] initWithTarget:forwarder action:@selector(gesturePerformed:)] autorelease];
	[swipeRecognizer setDirection: ofxGenericGestureTypeSwipeToiOS( type ) ];
	[ _view addGestureRecognizer:swipeRecognizer];
#elif TARGET_ANDROID
#endif
}

string ofxGenericView::dumpViewGraph( int depth )
{
#if defined(DEBUG) || defined(TEST)
    string result;
    string tab;
    for( int trav = 0 ; trav < depth; trav ++ )
        tab += " ";
    tab += " ";
    
    result += tab + toString() + "\n";

    if ( _children.size() )
    {
        result += tab + "\\\n";

        for( std::list< ofPtr< ofxGenericView > >::const_iterator travChildren = _children.begin(); travChildren != _children.end(); travChildren++ )
        {
            result += ( *travChildren )->dumpViewGraph( depth + 1 );
        }
    }
    
    return result;
#else
    return string();
#endif
}

string ofxGenericView::toString()
{
#if defined(DEBUG) || defined(TEST)
    const std::type_info& info = typeid( *this );
    
    string result( info.name() );    
#if TARGET_OS_IPHONE
    string nativeClassName = ofxNSStringToString( NSStringFromClass ( [ getNativeView() class ] ) ); 
    result += " native: " + nativeClassName;
#else
#endif
    result += " frame: " + getFrame().toString();
    return result;
    
#else
    return string();
#endif
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

-( BOOL )shouldAutorotateToInterfaceOrientation:( UIInterfaceOrientation )interfaceOrientation
{
    if ( _delegate )
    {
    }
    return ofxGenericApp::getInstance()->shouldAutorotate( iOSToofOrientation( interfaceOrientation ) );
}

@end

@implementation ofxGenericViewAnimationForwarder

-( id )initWithDelegate:( ofPtrWeak< ofxGenericViewDelegate > )delegate
{
    self = [ super init ];
    if ( self )
    {
        _delegate = delegate;
    }
    return self;    
}

- (void)animationWillStart:( NSString* )animationID finished:( NSNumber* )finished context:( void* )context
{
    if ( _delegate )
    {
        _delegate.lock()->animationWillStart( ofxNSStringToString( animationID ) );
    }
}

- (void)animationDidStop:( NSString* )animationID finished:( NSNumber* )finished context:( void* )context
{
    if ( _delegate )
    {
        _delegate.lock()->animationDidStop( ofxNSStringToString( animationID ) );
    }
}

@end

@implementation ofxGenericGestureForwarder

-( id )initWithDelegate:( ofPtrWeak< ofxGenericViewDelegate > )delegate type:(int)type
{
    self = [ super init ];
    if ( self )
    {
        _delegate = delegate;
        _type = type;
    }
    return self;    
}

- (void)gesturePerformed:( UIGestureRecognizer* )recognizer
{
    if ( _delegate )
    {
        if ( [recognizer isKindOfClass:[UISwipeGestureRecognizer class]] )
        {
            _delegate.lock()->gesturePerformedSwipe( (ofxGenericGestureTypeSwipe) _type );
        }
    }
}

@end
#endif


