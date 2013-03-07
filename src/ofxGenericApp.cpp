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

#if TARGET_OS_IPHONE || TARGET_OS_MAC
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/sysctl.h>

#include <AVFoundation/AVFoundation.h>

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

#if TARGET_OS_IPHONE
Class ofxGenericApp::getAppDelegateClass()
{
    return [ ofxGenericAppDelegate class ];
}
#endif

void ofxGenericApp::realRun()
{
#if TARGET_OS_IPHONE
    NSString* delegateClassName = NSStringFromClass( getAppDelegateClass() );
    UIApplicationMain( nil, nil, nil, delegateClassName );
#endif
    finishedLaunching();
}

void ofxGenericApp::runViaInfiniteLoop( ofPtr< ofxAppGenericWindow > window )
{
    ofLog( ofxGenericModuleName, OF_LOG_VERBOSE, "App loop starting..." );
    // TODO: strong references
    _window = window;
    _windowSize = _window->getFrame();

    if ( true )
    {
        realRun();
    } else
    {
        try
        {
        #if TARGET_OS_IPHONE
            @try
            {
                realRun();
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
    
    if ( isDebuggerAttached() )
    {
        ofxGLogVerbose( "Debugger is attached" );
    } else 
    {
        ofxGLogVerbose( "Debugger not attached" );
    }
    
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
void ofxGenericApp::willEnterForeground()
{    
}
void ofxGenericApp::willTerminate()
{
}

void ofxGenericApp::didReceiveMemoryWarning()
{
}

string ofxGenericApp::getAppVersion()
{
#if TARGET_OS_IPHONE
    return ofxNSStringToString( [ [ [ NSBundle mainBundle ] infoDictionary ] objectForKey:@"CFBundleShortVersionString" ] );
#endif    
}

string ofxGenericApp::getBuildVersion()
{
#if TARGET_OS_IPHONE
    return ofxNSStringToString( [ [ [ NSBundle mainBundle ] infoDictionary ] objectForKey:@"CFBundleVersion" ] );
#endif
}

void ofxGenericApp::setOrientation( ofOrientation toOrientation )
{
#if TARGET_OS_IPHONE
    CGAffineTransform transform = CGAffineTransformIdentity;
    
    switch (ofInterfaceOrientationToiOS( toOrientation ) ) {
            
        case UIInterfaceOrientationLandscapeLeft:
            transform = CGAffineTransformMakeRotation( ( CGFloat )-M_PI_2 );
            break;
            
        case UIInterfaceOrientationLandscapeRight:
            transform = CGAffineTransformMakeRotation( ( CGFloat )M_PI_2 );
            break;
            
        default:
            break;
    }
    
    [ [ UIApplication sharedApplication ] setStatusBarOrientation:ofInterfaceOrientationToiOS( toOrientation ) animated:NO ];
    
    [getRootView()->getNativeView() setTransform:transform];
    [getRootView()->getNativeView() setFrame:ofRectangleToCGRect( _windowSize ) ];
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
    
    for ( unsigned int i = 0; i < _keyboardDelegates.size(); i++ )
    {
        if ( _keyboardDelegates[i] )
        {
            _keyboardDelegates[i]->keyboard_willShow( keyboardFrame );
        }
    }
}

void ofxGenericApp::keyboardWillHide()
{
    _keyboardIsVisible = false;
    if (_moveFromUnderKeyboard)
    {
        _moveFromUnderKeyboard->setFrame( _moveFromUnderKeyboardOriginalFrame );
    }
    
    for ( unsigned int i = 0; i < _keyboardDelegates.size(); i++ )
    {
        if ( _keyboardDelegates[i] )
        {
            _keyboardDelegates[i]->keyboard_willHide( );
        }
    }
}

void ofxGenericApp::setMoveFromUnderKeyboard( ofPtr< ofxGenericView > view )
{
    _moveFromUnderKeyboard = view;
}

void ofxGenericApp::addKeyboardDelegate( ofPtr< ofxGenericKeyboardDelegate > delegate )
{
#if DEBUG
    for( std::vector< ofPtr< ofxGenericKeyboardDelegate > >::const_iterator findDuplicate = _keyboardDelegates.begin(); findDuplicate != _keyboardDelegates.end(); findDuplicate ++ )
    {
        if ( delegate == *findDuplicate )
        {
            ofxGLogWarning( "Object added to keyboard delegate list twice!" );
        }
    }
#endif
    _keyboardDelegates.push_back( delegate );
}

void ofxGenericApp::removeKeyboardDelegate( ofPtr< ofxGenericKeyboardDelegate > delegate )
{
    for ( std::vector< ofPtr< ofxGenericKeyboardDelegate > >::iterator i = _keyboardDelegates.begin(); i != _keyboardDelegates.end(); i++ )
    {
        if ( *i == delegate )
        {
            _keyboardDelegates.erase( i );
            return;
        }
    }
}

void ofxGenericApp::clearKeyboardDelegates()
{
    _keyboardDelegates.clear();
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

void ofxGenericApp::setAllowOtherAppSounds( bool allow )
{
#if TARGET_OS_IPHONE
    NSString* category;
    if ( allow )
    {
        category = AVAudioSessionCategoryAmbient;
    } else
    {
        category = AVAudioSessionCategorySoloAmbient;
    }
    NSError* error;
    [ [ AVAudioSession sharedInstance ] setCategory:category error:&error ];
    if ( error )
    {
//        ofxGLogError( "Error setting Allow Other App Sounds: " + ofxNSStringToString( error.description ) );
    }
#endif
}

void ofxGenericApp::vibrate()
{
#if TARGET_OS_IPHONE
    AudioServicesPlaySystemSound( kSystemSoundID_Vibrate );
#endif
}

void ofxGenericApp::saveImageToLibrary( ofImage& image )
{
#if TARGET_OS_IPHONE
    UIImageWriteToSavedPhotosAlbum( [ UIImage imageWithData:UIImagePNGRepresentation( OFImageToUIImage( image ) ) ], nil, nil, nil );
#endif
}

#if TARGET_OS_IPHONE
#include "UIDevice-Reachability.h"
#endif

bool ofxGenericApp::hasNetworkConnection()
{
#if TARGET_OS_IPHONE
    return [ [ UIDevice currentDevice ] networkAvailable ];
#endif
    return false;
}

bool ofxGenericApp::hasInternetConnection()
{
#if TARGET_OS_IPHONE
    return [ [ UIDevice currentDevice ] hostAvailable:@"www.google.com" ];
#endif
    return false;
}

void ofxGenericApp::sendURLToOS( string url )
{
#if TARGET_OS_IPHONE
    [ [ UIApplication sharedApplication ] openURL:[ NSURL URLWithString:ofxStringToNSString( url ) ] ];
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
}

std::map< string, string > ofxGenericApp::getLaunchOptions()
{
    return _launchOptions;
}

void ofxGenericApp::setLaunchOptions( std::map< string, string > launchOptions )
{
    _launchOptions = launchOptions;
}

void ofxGenericApp::gotNotification( string type )
{
    //nuffin'
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

bool ofxGenericApp::isDebuggerAttached()
{
#if TARGET_OS_IPHONE || TARGET_OS_MAC
    int                 junk;
    int                 mib[4];
    struct kinfo_proc   info;
    size_t              size;
    
    // Initialize the flags so that, if sysctl fails for some bizarre 
    // reason, we get a predictable result.
    
    info.kp_proc.p_flag = 0;
    
    // Initialize mib, which tells sysctl the info we want, in this case
    // we're looking for information about a specific process ID.
    
    mib[0] = CTL_KERN;
    mib[1] = KERN_PROC;
    mib[2] = KERN_PROC_PID;
    mib[3] = getpid();
    
    // Call sysctl.
    
    size = sizeof(info);
    junk = sysctl(mib, sizeof(mib) / sizeof(*mib), &info, &size, NULL, 0);
    assert(junk == 0);
    
    // We're being debugged if the P_TRACED flag is set.
    
    return ( (info.kp_proc.p_flag & P_TRACED) != 0 );
#endif
}