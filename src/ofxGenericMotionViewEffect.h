//
//  ofxGenericMotionViewEffect.h
//  ofxGeneric
//
//  Created by Konstantin Yavichev on 8/21/13.
//  Copyright (c) 2013 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericViewEffect.h"
#include "ofxGenericConstants.h"

class ofxGenericView;

class ofxGenericMotionViewEffect : public ofxGenericViewEffect
{
public:
    static ofPtr< ofxGenericMotionViewEffect > create( ofxGenericMotionEffectType type, float minRelativeValue, float maxRelativeValue );
    
    virtual void applyTo( ofPtr< ofxGenericView > view );
    
protected:
    ofxGenericMotionViewEffect();
    virtual void init( ofPtrWeak< ofxGenericMotionViewEffect > setThis, ofxGenericMotionEffectType type, float minRelativeValue, float maxRelativeValue );
    
    ofxGenericMotionEffectType _type;
    float _minRelativeValue;
    float _maxRelativeValue;
};