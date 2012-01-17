#pragma once

#include "ofMain.h"

#include "ofxGenericUtility.h"

#include "ofxGenericAppDelegate.h"
#include "ofxAppGenericWindow.h"
#include "ofxGenericView.h"

class ofxGenericApp : public ofBaseApp
{
    singletonInheretableHeader( ofxGenericApp );
    
public:
    void runViaInfiniteLoop( ofxAppGenericWindow* window );
    
    // TODO: come up with calling scheme, friending doesn't seem to be possible :(
    virtual void finishedLaunching();
    virtual void didBecomeActive();
    virtual void willResignActive();
    virtual void willTerminate();
    
    virtual void didReceiveMemoryWarning();
    
    virtual void deviceOrientationDidChange( ofOrientation newOrientation );
    
    ofxAppGenericWindow* getWindow();
    ofxGenericView* getRootView();
    
protected:    
    ofxAppGenericWindow* _window;
};