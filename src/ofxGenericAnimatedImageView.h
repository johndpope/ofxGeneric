//
//  ofxGenericAnimatedImageView.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 7/27/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericImageView.h"
#include "ofxGenericTimer.h"

class ofxGenericAnimatedImageView : public ofxGenericImageView, public ofxGenericTimerDelegate
{
public:
    static ofPtr< ofxGenericAnimatedImageView > create( const ofRectangle& setFrame = ofRectangle( 0, 0, 0, 0 ) );
    
    virtual void setImage( string fileName );
    virtual void setImage( ofPtr< ofImage > image );
    
    virtual void setImageFrames( const std::vector< string >& frames );
    virtual void setImageFrames( const std::vector< ofPtr< ofImage > >& frames );
    virtual void setFrameRate( float frameRate );

    virtual void timer_fired( ofPtr< ofxGenericTimer > timer );

protected:
    ofxGenericAnimatedImageView();
    
    bool _singleFrame;
    
    std::vector< ofPtr< ofImage > > _frames;
    void showFrame( unsigned int frame );
    void clearFrames();
    float _frameRate;
    unsigned int _currentFrame;
    ofPtr< ofxGenericTimer > _frameTimer;    
    virtual void clearTimer();
};