//
//  ofxGenericObject.cpp
//  ofxGeneric
//
//  Created by Eli Delventhal on 7/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericObject.h"

ofxGenericObject::~ofxGenericObject()
{
    
}

ofxGenericObject::ofxGenericObject()
{
    
}

ofPtr< ofxGenericObject > ofxGenericObject::create()
{
    ofPtr< ofxGenericObject > c = ofPtr< ofxGenericObject >( new ofxGenericObject() );
    c->init( c );
    return c;
}

void ofxGenericObject::init( ofPtrWeak< ofxGenericObject > setThis )
{
    _this = setThis;
}