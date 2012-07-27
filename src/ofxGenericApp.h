#pragma once

#include "ofxGenericMain.h"
#include "ofxGenericConstants.h"
#include "ofxGenericUtility.h"

#if TARGET_OS_IPHONE
#include "ofxGenericAppDelegate.h"
#endif

#include "ofxAppGenericWindow.h"
#include "ofxGenericView.h"
#include "ofxGenericAlertView.h"

#include "ofEvents.h"

class ofxGenericException;
class ofxGenericKeyboardDelegate;

class ofxGenericApp : public ofBaseApp, public ofxGenericAlertViewDelegate
{
public:
    static ofPtr< ofxGenericApp > getInstance();
    
    virtual void runViaInfiniteLoop( ofPtr< ofxAppGenericWindow > window );
    
    // TODO: come up with calling scheme, friending doesn't seem to be possible :(
    virtual void finishedLaunching();
    virtual void didBecomeActive();
    virtual void willResignActive();
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
    virtual void keyboardWillShow( const ofRectangle& keyboardFrame );    
    virtual void keyboardWillHide();  
    void setMoveFromUnderKeyboard( ofPtr< ofxGenericView > view ); // TODO: make into a list
    void addKeyboardDelegate( ofPtr< ofxGenericKeyboardDelegate > delegate );
    void removeKeyboardDelegate( ofPtr< ofxGenericKeyboardDelegate > delegate );
    void clearKeyboardDelegates();
    
    bool getStatusBarVisible();
    void setStatusBarVisible( bool visible, bool animated );
    
    static void vibrate();
    
    virtual void setup();

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

    virtual ~ofxGenericApp();
protected:
    
    ofPtr< ofxAppGenericWindow > _window;

    virtual void createRootView();
    
    bool _keyboardIsVisible;
    ofRectangle _keyboardFrame;
    ofPtr< ofxGenericView > _moveFromUnderKeyboard;
    ofRectangle _moveFromUnderKeyboardOriginalFrame;
    
    bool _fatalErrorDismissed;
    
    ofxGenericApp();
    static ofPtr< ofxGenericApp > _this;
    void setofxGenericAppInstanceToThis();
    
    void realRun();
    
    std::vector< ofPtr< ofxGenericKeyboardDelegate > > _keyboardDelegates;
};

class ofxGenericOrientationEventArgs : public ofEventArgs
{
public:
    ofOrientation orientation;
};

class ofxGenericEventsClass
{
public:
    ofEvent< ofxGenericOrientationEventArgs  > orientation;
    
    void disable()
    {
        orientation.disable();
    }
    void enable()
    {
        orientation.enable();
    }
};

extern ofxGenericEventsClass ofxGenericEvents;

template< class ListenerClass >
void ofxGRegisterOrientationEvents( ListenerClass* listener )
{
    ofAddListener( ofxGenericEvents.orientation, listener, &ListenerClass::deviceOrientationChanged );
}

template< class ListenerClass >
void ofxGUnregisterOrientationEvents( ListenerClass* listener )
{
    ofRemoveListener( ofxGenericEvents.orientation, listener, &ListenerClass::deviceOrientationChanged );
}

void ofNotifyDeviceOrientationChanged( ofOrientation orientation );

class ofxGenericKeyboardDelegate
{
public:
    virtual ~ofxGenericKeyboardDelegate() {};
    
    virtual void keyboard_willShow( const ofRectangle& keyboardFrame ) {};
    virtual void keyboard_willHide( ) {};
};