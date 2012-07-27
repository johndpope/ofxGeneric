//
//  ofxGenericAnimatedImageView.cpp
//  ofxGeneric
//
//  Created by Ian Grossberg on 7/27/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericAnimatedImageView.h"

ofPtr< ofxGenericAnimatedImageView > ofxGenericAnimatedImageView::create( const ofRectangle& setFrame )
{
    ofPtr< ofxGenericAnimatedImageView > create( new ofxGenericAnimatedImageView() );
    create->init( create, setFrame );
    return create;
}

ofxGenericAnimatedImageView::ofxGenericAnimatedImageView()
: _singleFrame( true ), _frameRate( 1.0f )
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

void ofxGenericAnimatedImageView::setImageFrames( const std::vector< string >& frames )
{
    std::vector< ofPtr< ofImage > > imageFrames;
    for( unsigned int travImageFileNames = 0; travImageFileNames < frames.size(); travImageFileNames ++ )
    {
        ofPtr< ofImage > imageFrame( new ofImage( frames[ travImageFileNames ] ) );
        imageFrames.push_back( imageFrame );
    }
    setImageFrames( imageFrames );
}

void ofxGenericAnimatedImageView::setImageFrames( const std::vector< ofPtr< ofImage > >& frames )
{
    clearFrames();
    
    _singleFrame = false;
    _frames = frames;
    _currentFrame = 0;
    showFrame( _currentFrame );

    setFrameRate( _frameRate );
}

void ofxGenericAnimatedImageView::setFrameRate( float frameRate )
{
    _frameRate = frameRate;
    clearTimer();
    _frameTimer = ofxGenericTimer::create( frameRate, true, dynamic_pointer_cast< ofxGenericTimerDelegate >( _this ) );
}

void ofxGenericAnimatedImageView::timer_fired( ofPtr< ofxGenericTimer > timer )
{
    if ( timer == _frameTimer )
    {
        _currentFrame ++;
        if ( _currentFrame > _frames.size() )
        {
            _currentFrame = 0;
        }
        showFrame( _currentFrame );
    }
}

void ofxGenericAnimatedImageView::showFrame( unsigned int frame )
{
    if ( frame >= _frames.size() )
    {
        frame = frame % ( _frames.size() );
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
