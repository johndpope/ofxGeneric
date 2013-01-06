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
, _players( nil ), _forwarder( nil )
#endif
{
}

void ofxGenericSound::init( ofPtrWeak< ofxGenericSound > setThis, string fileName, string extension, bool loadInBackground, bool loadIntoMemory )
{
    _this = setThis;
#if TARGET_OS_IPHONE
    if ( !_players )
    {
        _players = [[NSMutableArray alloc] initWithCapacity:3];
    }
#endif
        
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
    NSString *path = ofxStringToNSString( ofToPath(fileName, false) );
    
    if ( path )
    {
        NSURL *url = [ NSURL fileURLWithPath:path ];
        if ( url )
        {
            NSData* data = [ NSData dataWithContentsOfURL:url ];
            if ( data )
            {
                NSError* error = nil;
                AVAudioPlayer* player = [ [ [ AVAudioPlayer alloc ] initWithData:data error:&error ] autorelease ];
                
                if ( error )
                {
                    loadError = ofxNSStringToString( [ NSString stringWithFormat:@"%@", error ] );
                    player = nil;
                }
                else
                {
                    success = true;
                    [ _players addObject:player ];
                }
            } else
            {
                loadError = "Unable to create NSData from path.";
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
    if ( _players )
    {
        [ _players release ];
        _players = nil;
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
    if ( [ _players count ] > 0 )
    {
        AVAudioPlayer *player = nil;
        
        for ( unsigned int i = 0; i < [ _players count ]; i++ )
        {
            AVAudioPlayer *thisPlayer = [ _players objectAtIndex:i ];
            if ( !thisPlayer.playing )
            {
                player = thisPlayer;
            }
        }
        
        //if this sound is already playing, then add another audio player at the end.
        //performance-wise this should be negligible because we share sound data.
        if ( !player )
        {
            AVAudioPlayer *oldPlayer = [ _players objectAtIndex:0 ];
            
            //store off all of the old player's values and assign them to this one too
            float oldPan;
            float oldPitch;
            BOOL oldEnablePitch;
            if ( [ oldPlayer respondsToSelector:@selector( setPan: ) ] )
            {
                oldPan = oldPlayer.pan;
            }
            if ( [ oldPlayer respondsToSelector:@selector( setRate: ) ] )
            {
                oldPitch = oldPlayer.rate;
                oldEnablePitch = oldPlayer.enableRate;
            }
            
            NSError *error = nil;
            player = [ [ [ AVAudioPlayer alloc ] initWithData:oldPlayer.data error:&error ] autorelease ];
            if ( error )
            {
                ofLogError( "Error playing sound and trying to create new AVAudioPlayer: " + string( [[NSString stringWithFormat:@"%@",error] cStringUsingEncoding:NSUTF8StringEncoding] ) );
                return;
            }
            else if ( !player )
            {
                ofLogError( "Error playing sound and trying to create new AVAudioPlayer: player is nil after alloc call." );
                return;
            }
            
            if ( [ oldPlayer respondsToSelector:@selector( setPan: ) ] )
            {
                player.pan = oldPan;
            }
            if ( [ oldPlayer respondsToSelector:@selector( setRate: ) ] )
            {
                player.enableRate = oldEnablePitch;
                player.rate = oldPitch;
            }
            
            [ _players addObject:player ];
        }
        
        //set the loops to be only one because we're not looping
        player.numberOfLoops = 0;
        if ( playbackDelay <= 0 )
        {
            [ player play ];
        }
        else
        {
            [ player playAtTime: player.deviceCurrentTime +  playbackDelay ];
        }
    }
#endif
}

// stop playing the sound, does nothing if the sound is not playing
void ofxGenericSound::stop()
{
#if TARGET_OS_IPHONE
    if ( [ _players count ] > 0 )
    {
        for ( unsigned int i = 0; i < [ _players count ]; i++ )
        {
            [ [ _players objectAtIndex:i ] stop ];
        }
    }
#endif
}

// pause the sound, leaving it in memory and halting playback
void ofxGenericSound::pause()
{
#if TARGET_OS_IPHONE
    if ( [ _players count ] > 0 )
    {
        for ( unsigned int i = 0; i < [ _players count ]; i++ )
        {
            [ [ _players objectAtIndex:i ] pause ];
        }
    }
#endif
}

// plays and loops. if times is negative loops until stop() is called
void ofxGenericSound::loop( int times, float playbackDelay )
{
#if TARGET_OS_IPHONE
    if ( [ _players count ] > 0 )
    {
        play( playbackDelay );
        ( ( AVAudioPlayer * )[ _players objectAtIndex: [ _players count ] - 1 ] ).numberOfLoops = times;
    }
#endif
}

// whether or not the sound is playing
bool ofxGenericSound::isPlaying()
{
#if TARGET_OS_IPHONE
    if ( [ _players count ] > 0 )
    {
        for ( unsigned int i = 0; i < [ _players count ]; i++ )
        {
            if ( ( ( AVAudioPlayer * )[ _players objectAtIndex: i ] ).playing )
            {
                return true;
            }
        }
    }
#endif
    return false;
}

// -1 is full left, 1 is full right, 0 is both
void ofxGenericSound::setPan( float pan )
{
#if TARGET_OS_IPHONE
    if ( [ _players count ] > 0 )
    {
        for ( unsigned int i = 0; i < [ _players count ]; i++ )
        {
            ( ( AVAudioPlayer * )[ _players objectAtIndex: i ] ).pan = pan;
        }
    }
#endif
}

// sets the pitch, where 1 is normal, 0.5 is lower/slower, 2.0 is higher/faster, etc.
void ofxGenericSound::setPitch( float pitch )
{
#if TARGET_OS_IPHONE
    if ( [ _players count ] > 0 )
    {
        for ( unsigned int i = 0; i < [ _players count ]; i++ )
        {
            AVAudioPlayer *player = [ _players objectAtIndex: i ];
            player.enableRate = pitch != 1.0f ? YES : NO;
            player.rate = pitch;
        }
    }
#endif
}

// preloads the sound into buffers so it will play faster... we may not need this
void ofxGenericSound::preload( bool loadInBackground )
{
#if TARGET_OS_IPHONE
    if ( [ _players count ] > 0 )
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
            for ( unsigned int i = 0; i < [ _players count ]; i++ )
            {
                AVAudioPlayer *player = [ _players objectAtIndex: i ];
                [ player prepareToPlay ];
            }
        }
    }
#endif
}

// returns the length of the sound in seconds
double ofxGenericSound::getDuration()
{
#if TARGET_OS_IPHONE
    if ( [ _players count ] > 0 )
    {
        return ( ( AVAudioPlayer * )[ _players objectAtIndex: 0 ] ).duration;
    }
#endif
    return 0.0;
}

// returns how long in seconds the sound has been playing
double ofxGenericSound::getCurrentTime( int playerIndex)
{
#if TARGET_OS_IPHONE
    if ( [ _players count ] > 0 )
    {
        if ( playerIndex < 0 )
        {
            playerIndex = [ _players count ] - 1;
        }
        return ( ( AVAudioPlayer * )[ _players objectAtIndex: playerIndex ] ).currentTime;
    }
#endif
    return 0.0;
}

// returns the percent complete this sound is ( getCurrentTime() / getDuration() )
float ofxGenericSound::getAmountComplete( int playerIndex )
{
    if ( getDuration() <= 0.0 )
    {
        return 0.0f;
    }
    return (float) ( getCurrentTime( playerIndex ) / getDuration() );
}

bool ofxGenericSound::loadedSuccessfully()
{
#if TARGET_OS_IPHONE
    if ( [ _players count ] > 0 )
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
    NSAutoreleasePool *pool = [ [ NSAutoreleasePool alloc ] init ];
    
    ofPtr< ofxGenericSound > delegate = _delegate.lock();
    if ( delegate )
    {
        delegate->loadSound( string( [ fileName cStringUsingEncoding:NSUTF8StringEncoding ] ) );
    }
    
    [ pool release ];
}

-( void ) preloadIntoMemory
{
    NSAutoreleasePool *pool = [ [ NSAutoreleasePool alloc ] init ];
    
    ofPtr< ofxGenericSound > delegate = _delegate.lock();
    if ( delegate )
    {
        delegate->preload( false );
    }
    
    [ pool release ];
}

-( void )loadSoundAndPreloadIntoMemory:(NSString *)fileName
{
    NSAutoreleasePool *pool = [ [ NSAutoreleasePool alloc ] init ];
    
    [ self loadSound:fileName ];
    [ self preloadIntoMemory ];
    
    [ pool release ];
}

@end
#endif