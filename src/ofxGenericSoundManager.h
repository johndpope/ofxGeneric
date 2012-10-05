//
//  ofxGenericSoundManager.h
//  ofxGeneric
//
//  Created by Eli Delventhal on 9/24/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"
#include <map>

class ofxGenericSound;

class ofxGenericSoundManager
{
public:
    static ofxGenericSoundManager& getInstance();
    
    virtual bool load( string sound );
    virtual bool load( std::vector< string > sounds );
    virtual bool preloadIntoMemory( string sound );
    virtual bool preloadIntoMemory( std::vector< string > sounds );
    virtual void unload( string sound );
    virtual void unload( std::vector< string > sounds );
    virtual void unloadAll();
    
    virtual void play( string sound, float playbackDelay = 0 );
    virtual void stop( string sound );
    virtual void pause( string sound );
    virtual void loop( string sound, int loopCount = -1, float playbackDelay = 0 );
    virtual void setPan( string sound, float pan );
    virtual void setPitch( string sound, float pitch );
    
    virtual bool isPlaying( string sound );
    virtual double getDuration( string sound );
    virtual double getCurrentTime( string sound );
    virtual float getAmountComplete( string sound );
    
    virtual ~ofxGenericSoundManager();
protected:
    
    ofxGenericSoundManager();
    static ofPtr< ofxGenericSoundManager > _this;
    virtual void init( ofPtr< ofxGenericSoundManager > setThis );
    
    virtual bool soundExistsCheck( string sound );
    
    std::map< string, ofPtr< ofxGenericSound > > _soundMap;
};