//
//  ofxGenericSoundManager.cpp
//  ofxGeneric
//
//  Created by Eli Delventhal on 9/24/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericSoundManager.h"
#include "ofxGenericSound.h"

ofPtr< ofxGenericSoundManager > ofxGenericSoundManager::_this;

ofxGenericSoundManager& ofxGenericSoundManager::getInstance()
{
    if ( !ofxGenericSoundManager::_this )
    {
        ofPtr< ofxGenericSoundManager > instance = ofPtr< ofxGenericSoundManager >( new ofxGenericSoundManager() );
        instance->init( instance );
    }
    return *ofxGenericSoundManager::_this;
}

ofxGenericSoundManager::ofxGenericSoundManager() : _soundsAreEnabled( true )
{
}

void ofxGenericSoundManager::init( ofPtr< ofxGenericSoundManager > setThis )
{
    if ( _this )
    {
        ofLogWarning( "ofxGenericSoundManager already has _this set and init is being called! Are you calling ofxGenericSoundManager::getInstance() before calling you subclass's getInstance()?" );
    }
    ofxGenericSoundManager::_this = setThis;
    
    _soundMap = std::map< string, ofPtr< ofxGenericSound > >();
}

ofxGenericSoundManager::~ofxGenericSoundManager()
{
}

bool ofxGenericSoundManager::load( string sound, bool loadInBackground, bool loadIntoMemory )
{
    if ( !_soundMap[ sound ] || ( !_soundMap[ sound ]->loadedSuccessfully() && !_soundMap[ sound ]->isLoadingInBackground() ) )
    {
        ofPtr< ofxGenericSound > ofxgSound = ofxGenericSound::create( sound, "mp3", loadInBackground, loadIntoMemory );
        if ( ofxgSound->loadedSuccessfully() || loadInBackground )
        {
            _soundMap[ sound ] = ofxgSound;
            return true;
        }
        return false;
    }
    return true;
}

bool ofxGenericSoundManager::load( std::vector< string > sounds, bool loadInBackground, bool loadIntoMemory )
{
    bool success = true;
    for ( unsigned int i = 0; i < sounds.size(); i++ )
    {
        success = success && load( sounds[i], loadInBackground, loadIntoMemory );
    }
    return success;
}

bool ofxGenericSoundManager::preloadIntoMemory( string sound, bool loadInBackground )
{
    if ( _soundMap[ sound ] )
    {
        _soundMap[ sound ]->preload( loadInBackground );
    }
    return false;
}

bool ofxGenericSoundManager::preloadIntoMemory( std::vector< string > sounds, bool loadInBackground )
{
    bool success = true;
    for ( unsigned int i = 0; i < sounds.size(); i++ )
    {
        success = success && preloadIntoMemory( sounds[i], loadInBackground );
    }
    return success;
}

void ofxGenericSoundManager::unload( string sound )
{
    if ( _soundMap[ sound ] )
    {
        _soundMap[ sound ]->stop();
        _soundMap[ sound ] = ofPtr< ofxGenericSound >();
    }
}

void ofxGenericSoundManager::unload( std::vector< string > sounds )
{
    for ( unsigned int i = 0; i < sounds.size(); i++ )
    {
        unload( sounds[i] );
    }
}

void ofxGenericSoundManager::unloadAll()
{
    _soundMap = std::map< string, ofPtr< ofxGenericSound > >();
}

void ofxGenericSoundManager::play( string sound, float playbackDelay )
{
    if ( !soundExistsCheck( sound ) || !_soundsAreEnabled )
    {
        return;
    }
    ofxGLogVerbose( "Playing Sound " + sound );
    
    _soundMap[ sound ]->play( playbackDelay );
}

void ofxGenericSoundManager::stop( string sound )
{
    if ( !soundExistsCheck( sound ) )
    {
        return;
    }
    ofxGLogVerbose( "Stopping Sound " + sound );
    
    _soundMap[ sound ]->stop();
}

void ofxGenericSoundManager::pause( string sound )
{
    if ( !soundExistsCheck( sound ) )
    {
        return;
    }
    ofxGLogVerbose( "Pausing Sound " + sound );
    
    _soundMap[ sound ]->pause();
}

void ofxGenericSoundManager::loop( string sound, int loopCount, float playbackDelay )
{
    if ( !soundExistsCheck( sound ) || !_soundsAreEnabled )
    {
        return;
    }
    
    _soundMap[ sound ]->loop( loopCount, playbackDelay );
}

void ofxGenericSoundManager::setPan( string sound, float pan )
{
    if ( !soundExistsCheck( sound ) )
    {
        return;
    }
    
    _soundMap[ sound ]->setPan( pan );
}

void ofxGenericSoundManager::setPitch( string sound, float pitch )
{
    if ( !soundExistsCheck( sound ) )
    {
        return;
    }
    
    _soundMap[ sound ]->setPitch( pitch );
}

float ofxGenericSoundManager::getVolume( string sound )
{
    if ( !soundExistsCheck( sound ) )
    {
        return 0.0f;
    }
    
    return _soundMap[ sound ]->getVolume();
}

void ofxGenericSoundManager::setVolume( string sound, float volume )
{
    if ( !soundExistsCheck( sound ) )
    {
        return;
    }
    
    _soundMap[ sound ]->setVolume( volume );
}

bool ofxGenericSoundManager::isPlaying( string sound )
{
    if ( !soundExistsCheck( sound ) )
    {
        return false;
    }
    
    return _soundMap[ sound ]->isPlaying();
}

double ofxGenericSoundManager::getDuration( string sound )
{
    if ( !soundExistsCheck( sound ) )
    {
        return 0.0;
    }
    
    return _soundMap[ sound ]->getDuration();
}

double ofxGenericSoundManager::getCurrentTime( string sound )
{
    if ( !soundExistsCheck( sound ) )
    {
        return 0.0;
    }
    
    return _soundMap[ sound ]->getCurrentTime();
}

float ofxGenericSoundManager::getAmountComplete( string sound )
{
    if ( !soundExistsCheck( sound ) )
    {
        return 0.0f;
    }
    
    return _soundMap[ sound ]->getAmountComplete();
}

bool ofxGenericSoundManager::soundExistsCheck( string sound )
{
    if ( !_soundMap[ sound ] )
    {
        ofLogWarning( "ofxGenericSoundManager: There is no sound loaded named \"" + sound + "\"" );
        return false;
    }
    return true;
}

void ofxGenericSoundManager::setSoundsAreEnabled( bool enabled )
{
    _soundsAreEnabled = enabled;
}

bool ofxGenericSoundManager::soundsAreEnabled()
{
    return _soundsAreEnabled;
}