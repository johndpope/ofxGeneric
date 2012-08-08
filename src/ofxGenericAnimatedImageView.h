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
#include "ofxGenericConstants.h"

class ofxGenericAnimatedImageViewDelegate;

class ofxGenericAnimatedImageView : public ofxGenericImageView, public ofxGenericTimerDelegate
{
public:
    static ofPtr< ofxGenericAnimatedImageView > create( const ofRectangle& setFrame = ofRectangle( 0, 0, 0, 0 ), const std::vector< string >& frames = std::vector< string >(), float frameRate = 20.0f, ofxGenericAnimatedImageLoopType loopType = ofxGenericAnimatedImageLoopTypeWrap, int animationDirection = 1 );
    
    virtual void setImage( string fileName );
    virtual void setImage( ofPtr< ofImage > image );
    virtual void setAtFirstImage();
    virtual void setAtLastImage();
    
    virtual void setImageFrames( const std::vector< string >& frames, bool reverse = false );
    virtual void setImageFrames( const std::vector< ofPtr< ofImage > >& frames, bool reverse = false );
    virtual void setFrameRate( float frameRate );
    virtual void start( float frameRate = -1.0f );
    virtual void setLoopMode( ofxGenericAnimatedImageLoopType mode );
    virtual void setAnimationDirection( int dir );

    virtual void setDelegate( ofPtrWeak< ofxGenericAnimatedImageViewDelegate > delegate );
    virtual void timer_fired( ofPtr< ofxGenericTimer > timer );

protected:
    ofxGenericAnimatedImageView();
    
    bool _singleFrame;
    ofxGenericAnimatedImageLoopType _loopMode;
    int _animationDirection;
    
    std::vector< ofPtr< ofImage > > _frames;
    //std::vector< string > _frameNames; //for debugging
    void showFrame( unsigned int frame );
    void clearFrames();
    float _frameRate;
    unsigned int _currentFrame;
    unsigned int _lastPlayedFrame;
    ofPtr< ofxGenericTimer > _frameTimer;    
    virtual void clearTimer();
    
    ofPtrWeak< ofxGenericAnimatedImageViewDelegate > _delegate;
};

class ofxGenericAnimatedImageViewDelegate
{
public:
    virtual ~ofxGenericAnimatedImageViewDelegate() {};
    virtual void animatedImage_animationStarted( ofPtr< ofxGenericAnimatedImageView > view ) {};
    virtual void animatedImage_animationEnded( ofPtr< ofxGenericAnimatedImageView > view ) {};
};