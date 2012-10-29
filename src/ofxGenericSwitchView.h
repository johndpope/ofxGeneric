//
//  ofxGenericSwitchView.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 10/28/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericView.h"

#if TARGET_OS_IPHONE
@class ofxGenericSwitchViewForwarder;
#endif
class ofxGenericSwitchViewDelegate;

class ofxGenericSwitchView : public ofxGenericView
{
public:
    static ofPtr< ofxGenericSwitchView > create( const ofRectangle& setFrame = ofRectangle(), ofPtrWeak< ofxGenericSwitchViewDelegate > delegate = ofPtr< ofxGenericSwitchViewDelegate >() );
    
    virtual ~ofxGenericSwitchView();
    
    void setDelegate( ofPtrWeak< ofxGenericSwitchViewDelegate > delegate );
    ofPtrWeak< ofxGenericSwitchViewDelegate > getDelegate();
    
    void setSwitched( bool value, bool animate = false );
    bool isSwitched();
    virtual void valueChanged();
    
    void setTintColor( const ofColor& color );
    ofColor getTintColor();
    
protected:
    ofxGenericSwitchView();
    NativeView createNativeView( const ofRectangle& frame );

    virtual void init( ofPtrWeak< ofxGenericSwitchView > setThis, const ofRectangle& setFrame, ofPtrWeak< ofxGenericSwitchViewDelegate > delegate );
#if TARGET_OS_IPHONE
    ofxGenericSwitchViewForwarder* _forwarder;
#endif
    ofPtrWeak< ofxGenericSwitchViewDelegate > _delegate;
};

class ofxGenericSwitchViewDelegate
{
public:
    virtual ~ofxGenericSwitchViewDelegate() {};
    
    virtual void switch_valueChanged( ofPtr< ofxGenericSwitchView > view ) {};
};