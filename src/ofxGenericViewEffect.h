//
//  ofxGenericViewEffect.h
//  ofxGeneric
//
//  Created by Konstantin Yavichev on 8/21/13.
//  Copyright (c) 2013 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"

class ofxGenericView;

class ofxGenericViewEffect
{
public:    
    virtual void applyTo( ofPtr< ofxGenericView > view ) = 0;
    
    virtual ~ofxGenericViewEffect();
protected:
    ofxGenericViewEffect();
    virtual void init( ofPtrWeak< ofxGenericViewEffect > setThis );
    ofPtrWeak< ofxGenericViewEffect > _this;
};