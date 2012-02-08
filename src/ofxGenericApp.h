#pragma once

#include "ofMain.h"

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
    
    ofPtr< ofxAppGenericWindow > getWindow();
    ofPtr< ofxGenericView > getRootView();
    
protected:    
    ofPtr< ofxAppGenericWindow > _window;

    virtual void createRootView();
};
