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

void ofxGenericLaunchView::init( ofPtrWeak< ofxGenericView > setThis, const ofRectangle& setBounds, ofPtrWeak< ofxGenericLaunchViewDelegate > delegate )
{
    _delegate = delegate;
    ofxGenericImageView::init( setThis, setBounds );
}

void ofxGenericLaunchView::didLoad()
{
    ofxGenericImageView::didLoad();
    setFrame( ofxGenericApp::getInstance()->getWindow()->getFrame() );
    
#if TARGET_OS_IPHONE
#ifdef UI_USER_INTERFACE_IDIOM
    if ( UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad )
    {
        ofOrientation orientation = ofxGenericPlatform::getInstance()->orientation();
        if ( orientation == OF_ORIENTATION_90_LEFT || orientation == OF_ORIENTATION_90_RIGHT )
        {
            setImage( "Default-Landscape~ipad.png" );        
        } else 
        {
            setImage( "Default-Portrait~ipad.png" );        
        }
    } else 
    {
        setImage( "Default.png" );
    }
#else
    setImage( "Default.png" );
#endif
#else
    setImage( "Default.png" );
#endif
    
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