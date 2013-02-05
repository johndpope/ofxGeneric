//
//  ofxGenericLaunchView.cpp
//  iOS
//
//  Created by Ian Grossberg on 3/7/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericLaunchView.h"
#include "ofxGenericApp.h"
#include "ofxGenericPlatform.h"

#if TARGET_OS_IPHONE
#include <UIKit/UIKit.h>
#endif

ofPtr< ofxGenericLaunchView > ofxGenericLaunchView::create( const ofRectangle& setFrame, ofPtrWeak< ofxGenericLaunchViewDelegate > delegate )
{
    ofPtr< ofxGenericLaunchView > create = ofPtr< ofxGenericLaunchView >( new ofxGenericLaunchView() );
    create->init( create, setFrame, delegate );
    return create;
}

void ofxGenericLaunchView::init( ofPtrWeak< ofxGenericLaunchView > setThis, const ofRectangle& setBounds, ofPtrWeak< ofxGenericLaunchViewDelegate > delegate )
{
    _delegate = delegate;
    ofxGenericImageView::init( setThis, setBounds );
}

void ofxGenericLaunchView::didLoad()
{
    ofxGenericImageView::didLoad();
    setFrame( ofxGenericApp::getInstance()->getWindow()->getFrame() );
    
    if ( ofxGenericPlatform::isTablet() )
    {
        ofOrientation orientation = ofxGenericPlatform::orientation();
        if ( orientation == OF_ORIENTATION_90_LEFT || orientation == OF_ORIENTATION_90_RIGHT )
        {
#if TARGET_OS_IPHONE
            setImage( "Default-Landscape~ipad.png" );
#endif
        } else
        {
#if TARGET_OS_IPHONE
            setImage( "Default-Portrait~ipad.png" );
#endif
        }
    } else 
    {
        setImage( "Default.png" );
    }
    
    if ( _delegate )
    {
        _delegate.lock()->launchView_didLoad();
    }
}

void ofxGenericLaunchView::didAppear()
{
    ofxGenericImageView::didAppear();
    if ( _delegate )
    {
        _delegate.lock()->launchView_didAppear();
    }
}