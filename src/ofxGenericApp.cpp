//
//  ofxGenericApp.cpp
//
//  Created by Ian Grossberg on 12/29/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#include "ofxGenericApp.h"

#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>

#import "ofxGenericAppDelegate.h"
#endif

#include "ofxGenericException.h"

#if TARGET_ANDROID
#include "JNIObject.h"
#endif

#include <math.h>
#include <time.h>

#if TARGET_ANDROID
const char* ofxGenericApp::ActivityClassName = "cc/openframeworks/ofxGeneric/Activity";
#endif

#ifdef OF_USING_POCO
ofxGenericEventsClass ofxGenericEvents;
#endif

ofxGenericApp::ofxGenericApp()
: _keyboardIsVisible( false )
{
}

ofxGenericApp::~ofxGenericApp()
{
}

ofPtr< ofxGenericApp > ofxGenericApp::_this;

ofPtr< ofxGenericApp > ofxGenericApp::getInstance()
{
    if ( !ofxGenericApp::_this )
    {
        ( new ofxGenericApp() )->setofxGenericAppInstanceToThis();
    }
    return ofxGenericApp::_this;
}

void ofxGenericApp::setofxGenericAppInstanceToThis()
{
    if ( !ofxGenericApp::_this )
    {
        ofxGenericApp::_this = ofPtr< ofxGenericApp >( this );
    } else
    {
        throw ofxGenericExceptionSubclassedSingletonInitializedTwice( "ofxGenericApp" );        
    }
}

void ofxGenericApp::runViaInfiniteLoop( ofPtr< ofxAppGenericWindow > window )
{
    try
    {
        ofLog( ofxGenericModuleName, OF_LOG_VERBOSE, "App loop starting..." );
        // TODO: strong references
        _window = window;

    #if TARGET_OS_IPHONE
        @try
        {
            NSString* delegateClassName = NSStringFromClass( [ ofxGenericAppDelegate class ] );
            UIApplicationMain( nil, nil, nil, delegateClassName );
            finishedLaunching();
        } @catch( NSException* exception )
        {
            ofxGenericException uncaught( exception );
            handleUncaughtException( uncaught );
        }
        
    #elif TARGET_ANDROID
    /*    JNIMethod setWindow(
                JNIFindClass(ofxGenericApp::ActivityClassName ),
                true,
                "setWindow",
                JNIEncodeMethodSignature( 1, JNIType_void, JNIType_object, ofxGenericView::className ), //"(Lcc/openframeworks/ofxGeneric/View;)V",
                true
                );

        JNICallStaticVoidMethod( setWindow.getClass(), setWindow.getID(), _window->getNativeWindow() );
    */
        
    #endif
        
    } catch( ofxGenericException& uncaughtofxGeneric )
    {
        handleUncaughtException( uncaughtofxGeneric );
    } catch( std::exception& uncaughtStd )
    {
        ofxGenericException uncaughtStdofxGeneric( uncaughtStd );
        handleUncaughtException( uncaughtStdofxGeneric );        
    }
#if !defined (DEBUG)
    catch( ... )
    {
        ofxGenericException uncaughtUnknown( "Unknown exception" );
        handleUncaughtException( uncaughtUnknown );
    }
#endif
}

// TODO: come up with calling scheme, friending doesn't seem to be possible :(
void ofxGenericApp::finishedLaunching()
{
    createRootView();
    
#if TARGET_OS_IPHONE
    ofSetDataPathRoot( ofxNSStringToString( [ NSString stringWithFormat:@"%@/", [ [ NSBundle mainBundle ] resourcePath ] ] ) );
    
    ofSetDocumentsPathRoot( ofxNSStringToString( [ NSString stringWithFormat:@"%@/", [ NSSearchPathForDirectoriesInDomains( NSDocumentDirectory, NSUserDomainMask, YES ) objectAtIndex:0 ] ] ) );
#elif TARGET_ANDROID

#endif
    
    /*
     iPhoneSetOrientation(OFXIPHONE_ORIENTATION_PORTRAIT);


     // call testApp::setup()
     ofRegisterTouchEvents((ofxiPhoneApp*)ofGetAppPtr());
     */

	ofNotifySetup();
	ofNotifyUpdate();

     /*
     // Listen to did rotate event
*/
}

void ofxGenericApp::createRootView()
{
    ofPtr< ofxGenericView > rootView = ofxGenericView::create( _window->getFrame() );
#if TARGET_OS_IPHONE
    [ rootView->getUIViewController() setWantsFullScreenLayout:YES ];
#endif
    _window->setRootView( rootView );
}

void ofxGenericApp::didBecomeActive()
{
}
void ofxGenericApp::willResignActive()
{
}
void ofxGenericApp::willTerminate()
{
}

void ofxGenericApp::didReceiveMemoryWarning()
{
}

void ofxGenericApp::setOrientation( ofOrientation toOrientation )
{
#if TARGET_OS_IPHONE
    CGAffineTransform transform = CGAffineTransformIdentity;
    
    switch (ofInterfaceOrientationToiOS( toOrientation ) ) {
            
        case UIInterfaceOrientationLandscapeLeft:
            transform = CGAffineTransformMakeRotation(-M_PI_2);
            break;
            
        case UIInterfaceOrientationLandscapeRight:
            transform = CGAffineTransformMakeRotation(M_PI_2);
            break;
            
        default:
            break;
    }
    
    [ [ UIApplication sharedApplication ] setStatusBarOrientation:ofInterfaceOrientationToiOS( toOrientation ) animated:NO ];
    
    [getRootView()->getNativeView() setTransform:transform];
    [getRootView()->getNativeView() setFrame:CGRectMake(0, 0, 320, 480)];
    [getRootView()->getNativeView() setNeedsLayout];
#endif
}

bool ofxGenericApp::shouldAutorotate( ofOrientation toOrientation )
{
    return true;
}

void ofxGenericApp::deviceOrientationDidChange( ofOrientation newOrientation )
{
    if ( shouldAutorotate( newOrientation ) )
    {
        ofNotifyDeviceOrientationChanged( newOrientation );
    }
}


ofPtr< ofxAppGenericWindow > ofxGenericApp::getWindow() const
{
    return _window;
}

ofPtr< ofxGenericView > ofxGenericApp::getRootView() const
{
    if ( _window )
    {
        return _window->getRootView();
    }
    return ofPtr< ofxGenericView >();
}

bool ofxGenericApp::keyboardIsVisible()
{
    return _keyboardIsVisible;
}

ofRectangle ofxGenericApp::getKeyboardFrame()
{
    return _keyboardFrame;
}

void ofxGenericApp::keyboardWillShow( const ofRectangle& keyboardFrame )
{    
    _keyboardFrame = keyboardFrame;
    _keyboardIsVisible = true;
    if ( _moveFromUnderKeyboard )
    {
        _moveFromUnderKeyboardOriginalFrame = _moveFromUnderKeyboard->getFrame();
        ofRectangle worldLocation = _window->convertTo( _moveFromUnderKeyboardOriginalFrame, _moveFromUnderKeyboard->getParent().lock() );
#if TARGET_OS_IPHONE
        if ( worldLocation.intersects( _keyboardFrame ) )
        {
            // assume the keyboard came from the bottom
            
            // TODO: animate
            worldLocation = ofRectangle( worldLocation.x, keyboardFrame.y - worldLocation.height, worldLocation.width, worldLocation.height );
            _moveFromUnderKeyboard->setFrame( _window->convertFrom( worldLocation, _moveFromUnderKeyboard->getParent().lock() ) );
        }
#endif
    }
}

void ofxGenericApp::keyboardWillHide()
{
    _keyboardIsVisible = false;
    if (_moveFromUnderKeyboard)
    {
        _moveFromUnderKeyboard->setFrame( _moveFromUnderKeyboardOriginalFrame );
    }
}

void ofxGenericApp::setMoveFromUnderKeyboard( ofPtr< ofxGenericView > view )
{
    _moveFromUnderKeyboard = view;
}

bool ofxGenericApp::getStatusBarVisible()
{
#if TARGET_OS_IPHONE
    return ( bool )![ UIApplication sharedApplication ].statusBarHidden;
#endif
}

void ofxGenericApp::setStatusBarVisible( bool visible, bool animated )
{
#if TARGET_OS_IPHONE
    UIStatusBarAnimation animation;
    if ( animated )
    {
        animation = UIStatusBarAnimationSlide;       
    } else 
    {
        animation = UIStatusBarAnimationNone;
    }
    [ [ UIApplication sharedApplication ] setStatusBarHidden:( BOOL )!visible withAnimation:animation ];
#endif
}

void ofxGenericApp::handleUncaughtException( ofxGenericException& exception )
{
}

void ofxGenericApp::handleSignal( int signal )
{
}

void ofxGenericApp::showFatalErrorAndQuit( string title, string message )
{
    ofPtr< ofxGenericAlertView > fatalErrorAlert = ofxGenericAlertView::create( title, message, dynamic_pointer_cast< ofxGenericAlertViewDelegate >( _this ) );
    fatalErrorAlert->show();
    
#if TARGET_OS_IPHONE
    // Keep app alive while error popup is displayed
    CFRunLoopRef runLoop = CFRunLoopGetCurrent();
	CFArrayRef allModes = CFRunLoopCopyAllModes(runLoop);
    
    _fatalErrorDismissed = false;
    while ( !_fatalErrorDismissed )
	{
		for ( NSString* mode in ( NSArray* )allModes)
		{
			CFRunLoopRunInMode( ( CFStringRef )mode, 0.001, false );
		}
	}
	
	CFRelease( allModes );
#endif
}

void ofxGenericApp::fatalErrorDismissed()
{
    _fatalErrorDismissed = true;
}

void ofxGenericApp::alertView_clickedButtonAtIndex( int buttonIndex )
{
    fatalErrorDismissed();
}

void ofxGenericApp::alertView_cancelled()
{
    fatalErrorDismissed();
}

string ofxGenericApp::dumpViewGraph()
{
#if defined(DEBUG) || defined(TEST)
    if ( ofxGenericApp::getInstance()->getWindow() )
    {
        return ofxGenericApp::getInstance()->getWindow()->dumpViewGraph();
    }
#endif
    return string();
}

void ofxGenericApp::setup()
{
    ofBaseApp::setup();
    srand( time( NULL ) );
}

void ofNotifyDeviceOrientationChanged( ofOrientation orientation )
{
//    ofBaseApp * ofAppPtr = ofGetAppPtr();
    static ofxGenericOrientationEventArgs orientationEventArgs;
    
//	if(ofAppPtr){
//		ofAppPtr->dragEvent(info);
//	}
	
#ifdef OF_USING_POCO
    orientationEventArgs.orientation = orientation;
    ofNotifyEvent( ofxGenericEvents.orientation, orientationEventArgs );
#endif
}