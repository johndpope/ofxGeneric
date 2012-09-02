//
//  ofxGenericTimer.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 5/8/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"

#if TARGET_OS_IPHONE
@class ofxGenericTimerForwarder;
#endif
class ofxGenericTimerDelegate;

class ofxGenericTimer
{
public:
    static ofPtr< ofxGenericTimer > create( float timeInterval, bool repeats, ofPtrWeak< ofxGenericTimerDelegate > delegate );
    
    void fire();
    void stop();
    bool isRunning();
    
    float getTimeInterval();
    
    virtual ~ofxGenericTimer();
protected:
    ofxGenericTimer();
    virtual void init( ofPtrWeak< ofxGenericTimer > setThis, float timeInterval, bool repeats, ofPtrWeak< ofxGenericTimerDelegate > delegate );
    ofPtrWeak< ofxGenericTimer > _this;
    
    float _timeInterval;
    bool _repeats;
    
#if TARGET_OS_IPHONE
    NSTimer* _timer;
    ofxGenericTimerForwarder* _forwarder;
#endif

    ofPtrWeak< ofxGenericTimerDelegate > _delegate;    
};

class ofxGenericTimerDelegate
{
public:
    virtual ~ofxGenericTimerDelegate() {};
    
    virtual void timer_fired( ofPtr< ofxGenericTimer > timer ) {};
};