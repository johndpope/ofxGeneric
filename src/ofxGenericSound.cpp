//
//  ofxGenericSound.cpp
//  ofxGeneric
//
//  Created by Eli Delventhal on 9/24/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericSound.h"

ofPtr< ofxGenericSound > ofxGenericSound::create( string fileName )
{
    ofPtr< ofxGenericSound > create( new ofxGenericSound() );
    create->init( create, fileName );
    return create;
}

ofxGenericSound::ofxGenericSound()
{
}

void ofxGenericSound::init( ofPtrWeak< ofxGenericSound > setThis, string fileName )
{
    _this = setThis;
    
#if TARGET_OS_IPHONE
    NSString *path = [ NSString stringWithCString:fileName.c_str() encoding:NSUTF8StringEncoding ];
    NSURL *url = [NSURL fileURLWithPath:[[NSBundle mainBundle] pathForResource:path ofType:@"mp3"]];
    NSError *error = nil;
    _player = [ [ AVAudioPlayer alloc ] initWithContentsOfURL:url error:&error];
    
    if ( error )
    {
        ofLogError("Failed to load ofxGenericSound: " + string( [[NSString stringWithFormat:@"%@",error] cStringUsingEncoding:NSUTF8StringEncoding] ) );
        _player = nil;
    }
#endif
}

ofxGenericSound::~ofxGenericSound()
{
#if TARGET_OS_IPHONE
    if ( _player )
    {
        [ _player release ];
        _player = nil;
    }
#endif
}

// play the sound once, starting playbackDelay seconds from now
void ofxGenericSound::play( float playbackDelay )
{
#if TARGET_OS_IPHONE
    if ( _player )
    {
        _player.numberOfLoops = 0;
        if ( playbackDelay <= 0 )
        {
            [ _player play ];
        }
        else
        {
            [ _player playAtTime: _player.deviceCurrentTime +  playbackDelay ];
        }
    }
#endif
}

// stop playing the sound, does nothing if the sound is not playing
void ofxGenericSound::stop()
{
#if TARGET_OS_IPHONE
    if ( _player )
    {
        [ _player stop ];
    }
#endif
}

// pause the sound, leaving it in memory and halting playback
void ofxGenericSound::pause()
{
#if TARGET_OS_IPHONE
    if ( _player )
    {
        [ _player pause ];
    }
#endif
}

// plays and loops. if times is negative loops until stop() is called
void ofxGenericSound::loop( int times, float playbackDelay )
{
#if TARGET_OS_IPHONE
    if ( _player )
    {
        _player.numberOfLoops = times;
        if ( playbackDelay <= 0 )
        {
            [ _player play ];
        }
        else
        {
            [ _player playAtTime: _player.deviceCurrentTime +  playbackDelay ];
        }
    }
#endif
}

// whether or not the sound is playing
bool ofxGenericSound::isPlaying()
{
#if TARGET_OS_IPHONE
    if ( _player )
    {
        return _player.playing == YES ? true : false;
    }
#endif
    return false;
}

// -1 is full left, 1 is full right, 0 is both
void ofxGenericSound::setPan( float pan )
{
#if TARGET_OS_IPHONE
    if ( _player )
    {
        _player.pan = pan;
    }
#endif
}

// sets the pitch, where 1 is normal, 0.5 is lower/slower, 2.0 is higher/faster, etc.
void ofxGenericSound::setPitch( float pitch )
{
#if TARGET_OS_IPHONE
    if ( _player )
    {
        _player.enableRate = pitch != 1.0f ? YES : NO;
        _player.rate = pitch;
    }
#endif
}

// preloads the sound into buffers so it will play faster... we may not need this
void ofxGenericSound::preload()
{
#if TARGET_OS_IPHONE
    if ( _player )
    {
        [ _player prepareToPlay ];
    }
#endif
}

// returns the length of the sound in seconds
double ofxGenericSound::getDuration()
{
#if TARGET_OS_IPHONE
    if ( _player )
    {
        return _player.duration;
    }
#endif
    return 0.0;
}

// returns how long in seconds the sound has been playing
double ofxGenericSound::getCurrentTime()
{
#if TARGET_OS_IPHONE
    if ( _player )
    {
        return _player.currentTime;
    }
#endif
    return 0.0;
}

// returns the percent complete this sound is ( getCurrentTime() / getDuration() )
float ofxGenericSound::getAmountComplete()
{
    if ( getDuration() <= 0.0 )
    {
        return 0.0f;
    }
    return (float) ( getCurrentTime() / getDuration() );
}

bool ofxGenericSound::loadedSuccessfully()
{
#if TARGET_OS_IPHONE
    if ( _player )
    {
        return true;
    }
#endif
    return false;
}