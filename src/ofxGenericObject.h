//
//  ofxGenericObject.h
//  ofxGeneric
//
//  Created by Eli Delventhal on 7/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"

class ofxGenericObject
{
public:
    virtual ~ofxGenericObject();
    static ofPtr< ofxGenericObject > create();
protected:
    ofxGenericObject();
    virtual void init( ofPtrWeak< ofxGenericObject > setThis );
    
    ofPtrWeak< ofxGenericObject > _this;
};
