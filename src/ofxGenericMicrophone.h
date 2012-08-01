//
//  ofxGenericMicrophone.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 7/30/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"

#if TARGET_OS_IPHONE
#import <AVFoundation/AVFoundation.h>
#endif

class ofxGenericMicrophone
{
public:
    static ofPtr< ofxGenericMicrophone > create();
    
    virtual void start();
    virtual void pause();
    virtual void stop();
    
    bool isRecording();
    float getRecordedTime();
    
    float getAveragePowerForChannel( unsigned int channel );
    float getPeakPowerForChannel( unsigned int channel );
    
    float normalizePower( float value );

    virtual ~ofxGenericMicrophone();
    
protected:
    ofxGenericMicrophone();
    virtual void init( ofPtrWeak< ofxGenericMicrophone > setThis );
    ofPtrWeak< ofxGenericMicrophone > _this;
    
    const float _minimumPower;
    const float _maximumPower;
#if TARGET_OS_IPHONE
	AVAudioRecorder* _recorder;
#endif
};