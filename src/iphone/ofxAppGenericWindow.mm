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
: _uiWindow( nil ), _rootView( nil )
{
    _uiWindow = [ [ UIWindow alloc ] initWithFrame:[ [ UIScreen mainScreen ] bounds ] ];
}

ofxAppGenericWindow::~ofxAppGenericWindow()
{
    if ( _rootView )
    {
        delete _rootView;
        _rootView = NULL;
    }
    [ _uiWindow removeFromSuperview ];
    [ _uiWindow release ];
    _uiWindow = nil;
}

UIWindow* ofxAppGenericWindow::getUIWindow()
{
    return _uiWindow;
}

void ofxAppGenericWindow::runAppViaInfiniteLoop( ofBaseApp* appPtr ) 
{
    // HACK: for now unsafely assume we've got an ofxGenericApp, TODO: does it need to be called from the app?
    ofxGenericApp* app = ( ofxGenericApp* )appPtr;
    app->runViaInfiniteLoop( this );
}

ofRectangle ofxAppGenericWindow::getBounds()
{
    CGRect from = [ _uiWindow bounds ];
    ofRectangle to( from.origin.x, from.origin.y, from.size.width, from.size.height );
    return to;
}

ofxGenericView* ofxAppGenericWindow::getRootView()
{
    return _rootView;
}

void ofxAppGenericWindow::setRootView( ofxGenericView* view )
{
    _rootView = view;
    [ _uiWindow setRootViewController:view->getUIViewController() ];
}
