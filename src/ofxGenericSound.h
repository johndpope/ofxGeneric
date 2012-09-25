//
//  ofxGenericSound.h
//  ofxGeneric
//
//  Don't use this directly, instead use ofxGenericSoundManager to retrive, create, and play sounds.
//
//  Created by Eli Delventhal on 9/24/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"

#if TARGET_OS_IPHONE
#import <AVFoundation/AVFoundation.h>
#endif

class ofxGenericSound
{
public:
    static ofPtr< ofxGenericSound > create( string fileName );
    
    virtual ~ofxGenericSound();
    
    // play the sound once, starting playbackDelay seconds from now
    virtual void play( float playbackDelay = 0 );
    
    // stop playing the sound, does nothing if the sound is not playing
    virtual void stop();
    
    // pause the sound, leaving it in memory and halting playback
    virtual void pause();
    
    // plays and loops. if times is negative loops until stop() is called
    virtual void loop( int times = -1, float playbackDelay = 0 );
    
    // whether or not the sound is playing
    virtual bool isPlaying();
    
    // -1 is full left, 1 is full right, 0 is both
    virtual void setPan( float pan );
    
    // sets the pitch, where 1 is normal, 0.5 is lower/slower, 2.0 is higher/faster, etc.
    virtual void setPitch( float pitch );
    
    // preloads the sound into buffers so it will play faster... we may not need this
    virtual void preload();
    
    // returns the length of the sound in seconds
    virtual double getDuration();
    
    // returns how long in seconds the sound has been playing
    virtual double getCurrentTime();
    
    // returns the percent complete this sound is ( getCurrentTime() / getDuration() )
    virtual float getAmountComplete();
    
    // in case there was a loading issue, there will be no player so this will return false
    virtual bool loadedSuccessfully();
    
protected:
    ofxGenericSound();
    ofPtrWeak< ofxGenericSound > _this;
    virtual void init( ofPtrWeak< ofxGenericSound > setThis, string fileName );
#if TARGET_OS_IPHONE
	AVAudioPlayer* _player;
#endif
};