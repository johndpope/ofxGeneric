//
//  ofxGenericLaunchView.cpp
//  iOS
//
//  Created by Ian Grossberg on 3/7/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericLaunchView.h"
#include "ofxGenericApp.h"

void ofxGenericLaunchView::init( ofPtrWeak< ofxGenericView > setThis, const ofRectangle& setBounds, ofPtrWeak< ofxGenericLaunchViewDelegate > delegate )
{
    _delegate = delegate;
    ofxGenericImageView::init( setThis, setBounds );
}

void ofxGenericLaunchView::didLoad()
{
    ofxGenericImageView::didLoad();
    setFrame( ofxGenericApp::getInstance()->getWindow()->getFrame() );
    setImage( "Default.png" );
    
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