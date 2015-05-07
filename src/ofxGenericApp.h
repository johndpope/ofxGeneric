#pragma once

#include "ofxGenericMain.h"
#include "ofxGenericConstants.h"
#include "ofxGenericUtility.h"

#if TARGET_OS_IPHONE
@class ofxGenericAppDelegate;

#include "ofxGenericAppDelegate.h"
#endif

#include "ofxAppGenericWindow.h"
#include "ofxGenericView.h"
#include "ofxGenericAlertView.h"

#include "ofxGenericTimer.h"
#include "ofBaseApp.h"

class ofxGenericException;
class ofxGenericKeyboardDelegate;

class ofxGenericApp : public ofBaseApp, public ofxGenericAlertViewDelegate, public ofxGenericTimerDelegate
{
public:
    // no longer implemented by ofxGenericApp.  Subclasses must implement to return instance of correct subclass.
    //THIS IS INCORRECT: there is no reason not to do this. Previously, LumosityApp had its own _this,
    //which is presumably the reason for this choice, but there is no reason for it to have its own _this
    //(since ofxGenericApp has a _this already) and it was casuing a crash.
    static ofPtr< ofxGenericApp > getInstance();
    
    virtual void runViaInfiniteLoop( ofPtr< ofxAppGenericWindow > window );
    
    virtual void finishedLaunching();
    virtual void didBecomeActive();
    virtual void willResignActive();
    virtual void willEnterForeground();
    virtual void didEnterBackground();
    virtual void willTerminate();
    
    static string getAppVersion();
    static string getBuildVersion();
    
    virtual void didReceiveMemoryWarning();
    
    void setOrientation( ofOrientation toOrientation );
    virtual bool shouldAutorotate( ofOrientation toOrientation );
    virtual void deviceOrientationDidChange( ofOrientation newOrientation );
    
    ofPtr< ofxAppGenericWindow > getWindow() const;
    ofPtr< ofxGenericView > getRootView() const;
    
    bool keyboardIsVisible();
    ofRectangle getKeyboardFrame();
    ofRectangle getBeginKeyboardFrame();
    ofRectangle getEndKeyboardFrame();
    virtual void keyboardWillShow( const ofRectangle& beginKeyboardFrame, const ofRectangle& endKeyboardFrame );
    virtual void keyboardWillHide();  
    void setMoveFromUnderKeyboard( ofPtr< ofxGenericView > view ); // TODO: make into a list
    void addKeyboardDelegate( ofPtr< ofxGenericKeyboardDelegate > delegate );
    void removeKeyboardDelegate( ofPtr< ofxGenericKeyboardDelegate > delegate );
    void clearKeyboardDelegates();
    
    bool getStatusBarVisible();
    void setStatusBarVisible( bool visible, bool animated );
    ofRectangle getStatusBarFrame();
    
    void setAllowOtherAppSounds( bool allow );
    
    static void vibrate();
    
    bool hasNetworkConnection();
    bool hasInternetConnection();
    
    static void sendURLToOS( string url );
    
    virtual void setup();
    
    virtual void update();
    virtual void update( float deltaTime );

#if TARGET_ANDROID
    static const char* ActivityClassName;
#endif
    
    virtual void handleUncaughtException( ofxGenericException& exception );
    virtual void handleSignal( int signal );
    virtual void showFatalErrorAndQuit( string title, string message );
    virtual void fatalErrorDismissed();
    void alertView_clickedButtonAtIndex( int buttonIndex );
    void alertView_cancelled();
    
    static string dumpViewGraph();
    
    static bool isDebuggerAttached();
    
    virtual std::map< string, string > getLaunchOptions();
    virtual void setLaunchOptions( std::map< string, string > launchOptions );
    virtual void gotNotification( string type );
    
    virtual ~ofxGenericApp();
    
protected:
    
#if TARGET_OS_IPHONE
    virtual Class getAppDelegateClass();
    ofxGenericAppDelegate* getAppDelegate();
#endif

    ofPtr< ofxAppGenericWindow > _window;
    ofRectangle _windowSize;

    virtual void createRootView();
    
    bool _keyboardIsVisible;
    ofRectangle _beginKeyboardFrame;
    ofRectangle _endKeyboardFrame;
    ofPtr< ofxGenericView > _moveFromUnderKeyboard;
    ofRectangle _moveFromUnderKeyboardOriginalFrame;
    
    bool _fatalErrorDismissed;
    
    ofxGenericApp();
    static ofPtr< ofxGenericApp > _this;
    void setofxGenericAppInstanceToThis();
        
    std::vector< ofPtr< ofxGenericKeyboardDelegate > > _keyboardDelegates;
    std::map< string, string > _launchOptions;
    
    double _lastUpdateTime;
    float _updateDeltaTime;
    
    virtual void handleFinishedLaunchingPresetup();
    ofPtr< ofxGenericTimer > _setupTimer;
};

class ofxGenericOrientationEventArgs
{
public:
    ofOrientation orientation;
};

class ofxGenericEventsClass
{
public:
    
    void disable()
    {
    }
    void enable()
    {
    }
};

extern ofxGenericEventsClass ofxGenericEvents;

template< class ListenerClass >
void ofxGRegisterOrientationEvents( ListenerClass* listener )
{
}

template< class ListenerClass >
void ofxGUnregisterOrientationEvents( ListenerClass* listener )
{
}

void ofNotifyDeviceOrientationChanged( ofOrientation orientation );

class ofxGenericKeyboardDelegate
{
public:
    virtual ~ofxGenericKeyboardDelegate() {};
    
    virtual void keyboard_willShow( const ofRectangle& startFrame, const ofRectangle& endFrame ) {};
    virtual void keyboard_willShow( const ofRectangle& keyboardFrame ) {};
    virtual void keyboard_willHide( ) {};
};