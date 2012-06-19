//
//  ofxGenericFont.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 6/4/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"

#if TARGET_OS_IPHONE
typedef UIFont* NativeFont;
#endif

class ofxGenericFont
{
public:
    static ofPtr< ofxGenericFont > create( string name, float size );
    
    float getSize();
    string getName();
    NativeFont getNativeFont();
    
protected:
    ofxGenericFont();
    virtual void init( ofPtrWeak< ofxGenericFont > setThis, string name, float size );
    ofPtrWeak< ofxGenericFont > _this;
    
    string _name;
    float _size;
    NativeFont _nativeFont;
    
public:
    virtual ~ofxGenericFont();
};