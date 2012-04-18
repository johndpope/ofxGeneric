#pragma once

#include "ofxGenericMain.h"
#include "ofxGenericConstants.h"
#include "ofxGenericUtility.h"

#if TARGET_OS_IPHONE
#include "ofxGenericAppDelegate.h"
#endif

#include "ofxAppGenericWindow.h"
#include "ofxGenericView.h"

class ofxGenericApp : public ofBaseApp
{
    singletonInheretableHeader( ofxGenericApp );
    
public:
    virtual void runViaInfiniteLoop( ofPtr< ofxAppGenericWindow > window );
    
    // TODO: come up with calling scheme, friending doesn't seem to be possible :(
    virtual void finishedLaunching();
    virtual void didBecomeActive();
    virtual void willResignActive();
    virtual void willTerminate();
    
    virtual void didReceiveMemoryWarning();
    
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

#if TARGET_ANDROID
    static const char* ActivityClassName;
#endif
    
    static string dumpViewGraph();

protected:
    ofPtr< ofxAppGenericWindow > _window;

    virtual void createRootView();
    
    bool _keyboardIsVisible;
    ofRectangle _keyboardFrame;
    ofPtr< ofxGenericView > _moveFromUnderKeyboard;
    ofRectangle _moveFromUnderKeyboardOriginalFrame;
};
