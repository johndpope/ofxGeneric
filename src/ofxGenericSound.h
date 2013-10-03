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

#if TARGET_OS_IPHONE
@class ofxGenericSoundForwarder;
#endif

class ofxGenericSound
{
public:
    static ofPtr< ofxGenericSound > create( string fileName, string extension = "mp3", bool loadInBackground = false, bool loadIntoMemory = false );
    
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
    
    // sets the volume, range 0 - 1
    virtual void setVolume( float volume );
    
    // returns volume
    virtual float getVolume();
    
    // preloads the sound into buffers so it will play faster... we may not need this
    virtual void preload( bool loadInBackground = false );
    
    // returns the length of the sound in seconds
    virtual double getDuration();
    
    // returns how long in seconds the sound has been playing
    virtual double getCurrentTime( int playerIndex = -1 );
    
    // returns the percent complete this sound is ( getCurrentTime() / getDuration() )
    virtual float getAmountComplete( int playerIndex = -1 );
    
    // in case there was a loading issue, there will be no player so this will return false
    virtual bool loadedSuccessfully();
    
    // if this sound is loading in the background, we can't trust loadedSuccessfully() necessarily
    virtual bool isLoadingInBackground();
    
    //loads an individual sound, you shouldn't call this but must be public for forwarder to work
    virtual bool loadSound( string fileName );
    
protected:
    ofxGenericSound();
    ofPtrWeak< ofxGenericSound > _this;
    virtual void init( ofPtrWeak< ofxGenericSound > setThis, string fileName, string extension, bool loadInBackground, bool loadIntoMemory );
    
    bool _isLoadingInBackground;
    
#if TARGET_OS_IPHONE
	NSMutableArray * _players;
#elif TARGET_ANDROID
#endif
#if TARGET_OS_IPHONE
    ofxGenericSoundForwarder* _forwarder;
#elif TARGET_ANDROID
#endif
};

#if TARGET_OS_IPHONE
@interface ofxGenericSoundForwarder : NSObject
{
@private
    ofPtrWeak< ofxGenericSound > _delegate;
}
-( void )setDelegate:( ofPtrWeak< ofxGenericSound > )setDelegate;
-( void )loadSound:(NSString *)fileName;
-( void )preloadIntoMemory;
-( void )loadSoundAndPreloadIntoMemory:(NSString *)fileName;

@end
#endif