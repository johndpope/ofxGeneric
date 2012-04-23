//
//  ofxGenericSliderView.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 4/22/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericView.h"

class ofxGenericSliderViewDelegate;
#if TARGET_OS_IPHONE
@class ofxGenericSliderViewForwarder;
#endif

class ofxGenericSliderView : public ofxGenericView
{
public:
    static ofPtr< ofxGenericSliderView > create( const ofRectangle& setFrame = ofRectangle( 0, 0, 0, 0 ), ofPtrWeak< ofxGenericSliderViewDelegate > delegate = ofPtrWeak< ofxGenericSliderViewDelegate >() );
    
    float getValue();
    void setValue( float value, bool animated );
    float getMinimum();
    float getMaximum();
    void setRange( float minimum, float maximum );

#if TARGET_OS_IPHONE
    operator UISlider*();
#endif
    
    virtual void valueChanged();
    virtual void touchCancel();
    virtual void touchDown();
    virtual void touchDownRepeat();
    virtual void touchDragEnter();
    virtual void touchDragExit();
    virtual void touchDragInside();
    virtual void touchDragOutside();
    virtual void touchUpInside();
    virtual void touchUpOutside();
    
protected:
    ofxGenericSliderView();
    virtual void init( ofPtrWeak< ofxGenericSliderView > setThis, const ofRectangle& setFrame, ofPtrWeak< ofxGenericSliderViewDelegate > delegate );
    virtual NativeView createNativeView( const ofRectangle& frame );
    
    ofxGenericSliderViewForwarder* _forwarder;
    ofPtrWeak< ofxGenericSliderViewDelegate > _delegate;
};

class ofxGenericSliderViewDelegate
{
public:  
    virtual ~ofxGenericSliderViewDelegate() {};
    
    virtual void slider_valueChanged( ofPtr< ofxGenericSliderView > slider ) {};
    virtual void slider_touchCancel( ofPtr< ofxGenericSliderView > slider ) {};
    virtual void slider_touchDown( ofPtr< ofxGenericSliderView > slider ) {};
    virtual void slider_touchDownRepeat( ofPtr< ofxGenericSliderView > slider ) {};
    virtual void slider_touchDragEnter( ofPtr< ofxGenericSliderView > slider ) {};
    virtual void slider_touchDragExit( ofPtr< ofxGenericSliderView > slider ) {};
    virtual void slider_touchDragInside( ofPtr< ofxGenericSliderView > slider ) {};
    virtual void slider_touchDragOutside( ofPtr< ofxGenericSliderView > slider ) {};
    virtual void slider_touchUpInside( ofPtr< ofxGenericSliderView > slider ) {};
    virtual void slider_touchUpOutside( ofPtr< ofxGenericSliderView > slider ) {};
};