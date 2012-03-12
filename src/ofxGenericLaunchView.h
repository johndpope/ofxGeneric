//
//  ofxGenericLaunchView.h
//  iOS
//
//  Created by Ian Grossberg on 3/7/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericImageView.h"

class ofxGenericLaunchViewDelegate;

class ofxGenericLaunchView : public ofxGenericImageView
{
public:
    static ofPtr< ofxGenericLaunchView > create( const ofRectangle& setFrame = ofRectangle( 0, 0, 0, 0 ), ofPtrWeak< ofxGenericLaunchViewDelegate > delegate = ofPtrWeak< ofxGenericLaunchViewDelegate >() );
    virtual void didLoad();
    virtual void didAppear();
    
protected:
    virtual void init( ofPtrWeak< ofxGenericView > setThis, const ofRectangle& setBounds, ofPtrWeak< ofxGenericLaunchViewDelegate > delegate );
    
    ofPtrWeak< ofxGenericLaunchViewDelegate > _delegate;
};

class ofxGenericLaunchViewDelegate
{
public:
    virtual ~ofxGenericLaunchViewDelegate() {};
    virtual void launchView_didLoad() {};
    virtual void launchView_didAppear() {};
};