//
//  ofxGenericSound.cpp
//  ofxGeneric
//
//  Created by Eli Delventhal on 9/24/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericSound.h"

ofPtr< ofxGenericSound > ofxGenericSound::create( string fileName, string extension, bool loadInBackground, bool loadIntoMemory )
{
    ofPtr< ofxGenericSound > create( new ofxGenericSound() );
    create->init( create, fileName, extension, loadInBackground, loadIntoMemory );
    return create;
}

ofxGenericSound::ofxGenericSound() : _isLoadingInBackground( false )
#if TARGET_OS_IPHONE
, _player( nil ), _forwarder( nil )
#endif
{
}

void ofxGenericSound::init( ofPtrWeak< ofxGenericSound > setThis, string fileName, string extension, bool loadInBackground, bool loadIntoMemory )
{
    _this = setThis;
        
    //pull the extension off if we already have it in the fileName
    unsigned int dotIndex = fileName.find_last_of( '.' );
    if ( dotIndex != string::npos )
    {
        string foundExtension = fileName.substr( dotIndex, fileName.length() - dotIndex );
        if ( foundExtension == ("." + extension ) )
        {
            fileName = fileName.substr( 0, dotIndex );
        }
    }
    fileName = fileName + "." + extension;
    
    if ( ofxGFileExists( fileName, false ) )
    {
        if ( loadInBackground )
        {
            _isLoadingInBackground = true;
#if TARGET_OS_IPHONE
            if ( !_forwarder )
            {
                _forwarder = [[ofxGenericSoundForwarder alloc] init];
                [ _forwarder setDelegate:_this ];
            }
            
            //necessary because if we do both background sound loading and background loading into memory we aren't
            //guaranteed they will happen in the correct order
            if ( loadIntoMemory )
            {
                [ _forwarder performSelectorInBackground:@selector(loadSoundAndPreloadIntoMemory:) withObject:[ NSString stringWithCString: fileName.c_str() encoding:NSUTF8StringEncoding ] ];
            }
            else
            {
                [ _forwarder performSelectorInBackground:@selector(loadSound:) withObject:[ NSString stringWithCString: fileName.c_str() encoding:NSUTF8StringEncoding ] ];
            }
#endif
        }
        else
        {
            _isLoadingInBackground = false;
            bool success = loadSound( fileName );
            if ( success && loadIntoMemory )
            {
                preload( false );
            }
        }
    }
    else
    {
        ofLogError("Failed to load ofxGenericSound \"" + fileName + "\" - File does not exist." );
    }
}

bool ofxGenericSound::loadSound( string fileName )
{
    bool success = false;
    string loadError = "Unknown error";
#if TARGET_OS_IPHONE
    NSString *path = [ NSString stringWithCString: ofToPath(fileName, false).c_str() encoding:NSUTF8StringEncoding ];
    
    if ( path )
    {
        NSURL *url = [NSURL fileURLWithPath:path];
        if ( url )
        {
            NSError *error = nil;
            _player = [ [ AVAudioPlayer alloc ] initWithContentsOfURL:url error:&error];
            
            if ( error )
            {
                loadError = string( [[NSString stringWithFormat:@"%@",error] cStringUsingEncoding:NSUTF8StringEncoding] );
                _player = nil;
            }
            else
            {
                success = true;
            }
        }
        else
        {
            loadError = "Unable to create NSURL from path.";
        }
    }
    else
    {
        loadError = "Unable to create NSString from path.";
    }
#endif
    
    if ( !success )
    {
        ofLogError( "Failed to load ofxGenericSound \"" + fileName + "\" - " + loadError );
    }
    
    _isLoadingInBackground = false;
    
    return success;
}

ofxGenericSound::~ofxGenericSound()
{
#if TARGET_OS_IPHONE
    if ( _player )
    {
        [ _player release ];
        _player = nil;
    }
    if ( _forwarder )
    {
        [ _forwarder release ];
        _forwarder = nil;
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
void ofxGenericSound::preload( bool loadInBackground )
{
#if TARGET_OS_IPHONE
    if ( _player )
    {
        if ( loadInBackground )
        {
            if ( !_forwarder )
            {
                _forwarder = [[ofxGenericSoundForwarder alloc] init];
                [ _forwarder setDelegate:_this ];
            }
            [ _forwarder performSelectorInBackground:@selector(preloadIntoMemory) withObject:nil ];
        }
        else
        {
            [ _player prepareToPlay ];
        }
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

bool ofxGenericSound::isLoadingInBackground()
{
    return _isLoadingInBackground;
}

#if TARGET_OS_IPHONE
@implementation ofxGenericSoundForwarder

-( void ) setDelegate:( ofPtrWeak< ofxGenericSound > )setDelegate
{
    _delegate = setDelegate;
}

-( void ) loadSound:( NSString * )fileName
{
    ofPtr< ofxGenericSound > delegate = _delegate.lock();
    if ( delegate )
    {
        delegate->loadSound( string( [ fileName cStringUsingEncoding:NSUTF8StringEncoding ] ) );
    }
}

-( void ) preloadIntoMemory
{
    ofPtr< ofxGenericSound > delegate = _delegate.lock();
    if ( delegate )
    {
        delegate->preload( false );
    }
}

-( void )loadSoundAndPreloadIntoMemory:(NSString *)fileName
{
    [ self loadSound:fileName ];
    [ self preloadIntoMemory ];
}

@end
#endif