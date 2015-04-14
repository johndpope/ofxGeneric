//
//  ofxGenericClass.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 3/13/13.
//  Copyright (c) 2013 Lumos Labs. All rights reserved.
//

#pragma once

// TZLA-619 // #include "ofMain.h"
#include "ofTypes.h"

class ofxGenericClass
{
public:
    virtual ~ofxGenericClass();
    
    virtual std::string toString() const;
    
protected:
    ofxGenericClass();
    ofPtrWeak< ofxGenericClass > _this;
    virtual void init( ofPtrWeak< ofxGenericClass > setThis );
};