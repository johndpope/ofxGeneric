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

ofxGenericSoundManager::ofxGenericSoundManager()
{
}

void ofxGenericSoundManager::init( ofPtr< ofxGenericSoundManager > setThis )
{
    ofxGenericSoundManager::_this = setThis;
    
    _soundMap = std::map< string, ofPtr< ofxGenericSound > >();
}

ofxGenericSoundManager::~ofxGenericSoundManager()
{
}

bool ofxGenericSoundManager::load( string sound )
{
    if ( !_soundMap[ sound ] )
    {
        ofPtr< ofxGenericSound > ofxgSound = ofxGenericSound::create( sound );
        if ( ofxgSound->loadedSuccessfully() )
        {
            _soundMap[ sound ] = ofxgSound;
            return true;
        }
        return false;
    }
    return true;
}

bool ofxGenericSoundManager::load( std::vector< string > sounds )
{
    bool success = true;
    for ( unsigned int i = 0; i < sounds.size(); i++ )
    {
        success = success && load( sounds[i] );
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
    if ( !soundExistsCheck( sound ) )
    {
        return;
    }
    
    _soundMap[ sound ]->play( playbackDelay );
}

void ofxGenericSoundManager::stop( string sound )
{
    if ( !soundExistsCheck( sound ) )
    {
        return;
    }
    
    _soundMap[ sound ]->stop();
}

void ofxGenericSoundManager::pause( string sound )
{
    if ( !soundExistsCheck( sound ) )
    {
        return;
    }
    
    _soundMap[ sound ]->pause();
}

void ofxGenericSoundManager::loop( string sound, int loopCount, float playbackDelay )
{
    if ( !soundExistsCheck( sound ) )
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