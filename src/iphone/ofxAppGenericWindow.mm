//
//  ofxAppGenericWindowiPhone.mm
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 12/28/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#import "ofxAppGenericWindow.h"
#import <Foundation/Foundation.h>

#import "ofxGenericApp.h"

ofxAppGenericWindow::ofxAppGenericWindow()
: _uiWindow( nil )
{
    _uiWindow = [ [ UIWindow alloc ] initWithFrame:[ [ UIScreen mainScreen ] bounds ] ];
}

ofxAppGenericWindow::~ofxAppGenericWindow()
{
    releaseView( _uiWindow );
}

UIWindow* ofxAppGenericWindow::getUIWindow()
{
    return _uiWindow;
}

void ofxAppGenericWindow::runAppViaInfiniteLoop( ofBaseApp* appPtr ) 
{
    // HACK: for now unsafely assume we've got an ofxGenericApp, TODO: does it need to be called from the app?
    ofxGenericApp* app = ( ofxGenericApp* )appPtr;
    ofPtr< ofxAppGenericWindow > passThis( this );
    app->runViaInfiniteLoop( passThis );
}

ofRectangle ofxAppGenericWindow::getBounds()
{
    return CGRectToofRectangle( [ _uiWindow bounds ] );
}

ofPtr< ofxGenericView > ofxAppGenericWindow::getRootView()
{
    return _rootView;
}

void ofxAppGenericWindow::setRootView( ofPtr< ofxGenericView > view )
{
    _rootView = view;
    [ _uiWindow setRootViewController:_rootView->getUIViewController() ];
}
