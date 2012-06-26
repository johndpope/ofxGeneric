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
    
    bool getStatusBarVisible();
    void setStatusBarVisible( bool visible, bool animated );
    
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