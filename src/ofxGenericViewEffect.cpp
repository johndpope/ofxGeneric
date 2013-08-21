//
//  ofxGenericViewEffect.cpp
//  ofxGeneric
//
//  Created by Konstantin Yavichev on 8/21/13.
//  Copyright (c) 2013 Lumos Labs. All rights reserved.
//

#include "ofxGenericViewEffect.h"

#include "ofxGenericView.h"

ofxGenericViewEffect::ofxGenericViewEffect()
{
}

ofxGenericViewEffect::~ofxGenericViewEffect()
{
}

void ofxGenericViewEffect::init( ofPtrWeak< ofxGenericViewEffect > setThis )
{
    _this = setThis;
}