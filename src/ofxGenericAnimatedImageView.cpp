//
//  ofxGenericAnimatedImageView.cpp
//  ofxGeneric
//
//  Created by Ian Grossberg on 7/27/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericAnimatedImageView.h"

ofPtr< ofxGenericAnimatedImageView > ofxGenericAnimatedImageView::create( const ofRectangle& setFrame, const std::vector< string >& frames, float frameRate, ofxGenericAnimatedImageLoopType loopType, int animationDirection )
{
    ofPtr< ofxGenericAnimatedImageView > create( new ofxGenericAnimatedImageView() );
    create->init( create, setFrame );
    create->setImageFrames( frames );
    create->setFrameRate( frameRate );
    create->setLoopMode( loopType );
    create->setAnimationDirection( animationDirection );
    return create;
}

ofxGenericAnimatedImageView::ofxGenericAnimatedImageView()
: _singleFrame( true ), _frameRate( 1.0f ), _animationDirection( 1 ), _loopMode( ofxGenericAnimatedImageLoopTypeWrap )
{
}

void ofxGenericAnimatedImageView::setImage( string fileName )
{
    clearFrames();
    ofxGenericImageView::setImage( fileName );
}

void ofxGenericAnimatedImageView::setImage( ofPtr< ofImage > image )
{
    clearFrames();
    ofxGenericImageView::setImage( image );
}

void ofxGenericAnimatedImageView::setImageFrames( const std::vector< string >& frames, bool reverse )
{
    std::vector< ofPtr< ofImage > > imageFrames;
    for( unsigned int travImageFileNames = 0; travImageFileNames < frames.size(); travImageFileNames ++ )
    {
        ofPtr< ofImage > imageFrame( new ofImage( frames[ travImageFileNames ] ) );
        imageFrames.push_back( imageFrame );
    }
    setImageFrames( imageFrames, reverse );
}

void ofxGenericAnimatedImageView::setImageFrames( const std::vector< ofPtr< ofImage > >& frames, bool reverse )
{
    clearFrames();
    
    _singleFrame = false;
    _frames = frames;
    if ( reverse )
    {
        _currentFrame = frames.size() - 1;
        _animationDirection = -1;
    }
    else
    {
        _currentFrame = 0;
        _animationDirection = 1;
    }
    
    showFrame( _currentFrame );
}

void ofxGenericAnimatedImageView::setFrameRate( float frameRate )
{
    if ( frameRate >= 0 )
    {
        _frameRate = frameRate;
    }
}

void ofxGenericAnimatedImageView::start( float frameRate )
{
    if ( frameRate >= 0 )
    {
        _frameRate = frameRate;
    }
    clearTimer();
    _frameTimer = ofxGenericTimer::create( 1.0f / _frameRate, true, dynamic_pointer_cast< ofxGenericTimerDelegate >( _this ) );
}

void ofxGenericAnimatedImageView::setLoopMode( ofxGenericAnimatedImageLoopType mode )
{
    _loopMode = mode;
}

void ofxGenericAnimatedImageView::timer_fired( ofPtr< ofxGenericTimer > timer )
{
    if ( timer == _frameTimer )
    {
        int newFrame = _currentFrame + _animationDirection;
        
        if ( _currentFrame >= _frames.size() )
        {
            if ( _delegate )
            {
                _delegate.lock()->animatedImage_animationEnded( dynamic_pointer_cast< ofxGenericAnimatedImageView >( _this ) );
            }
            
            if ( _loopMode == ofxGenericAnimatedImageLoopTypeWrap )
            {
                _currentFrame = 0;
            }
            else if ( _loopMode == ofxGenericAnimatedImageLoopTypePingPong )
            {
                _animationDirection = -1;
                _currentFrame = _frames.size() - 1;
            }
            else if ( _loopMode == ofxGenericAnimatedImageLoopTypeClamp )
            {
                clearTimer();
            }
            else if ( _loopMode == ofxGenericAnimatedImageLoopTypeOnce )
            {
                _currentFrame = 0;
                clearTimer();
            }
        }
        else if ( newFrame < 0 )
        {
            if ( _loopMode == ofxGenericAnimatedImageLoopTypePingPong )
            {
                _animationDirection = 1;
            }
            else if ( _loopMode == ofxGenericAnimatedImageLoopTypeClamp )
            {
                clearTimer();
            }
            _currentFrame = 0;
        }
        
        _currentFrame = newFrame;
        showFrame( _currentFrame );
    }
}

void ofxGenericAnimatedImageView::showFrame( unsigned int frame )
{
    if ( _frames.size() <= 0 )
    {
        return;
    }
    
    if ( frame >= _frames.size() )
    {
        frame = frame % ( _frames.size() );
    }
    
    if ( frame == 0 && _delegate )
    {
        _delegate.lock()->animatedImage_animationStarted( dynamic_pointer_cast< ofxGenericAnimatedImageView >( _this ) );
    }
    
    ofxGenericImageView::setImage( _frames[ frame ] );
}

void ofxGenericAnimatedImageView::clearFrames()
{
    _singleFrame = true;
    _frames.clear();
    clearTimer();
}

void ofxGenericAnimatedImageView::clearTimer()
{
    if ( _frameTimer )
    {
        _frameTimer->stop();
        _frameTimer = ofPtr< ofxGenericTimer >();
    }  
}

void ofxGenericAnimatedImageView::setDelegate( ofPtrWeak< ofxGenericAnimatedImageViewDelegate > delegate )
{
    _delegate = delegate;
}

void ofxGenericAnimatedImageView::setAnimationDirection( int dir )
{
    _animationDirection = dir < 0 ? -1 : 1;
}