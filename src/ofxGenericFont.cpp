//
//  ofxGenericFont.cpp
//  ofxGeneric
//
//  Created by Ian Grossberg on 6/4/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericFont.h"

#include "ofxGenericUtility.h"

#import "ofCommon.h"

ofPtr< ofxGenericFont > ofxGenericFont::create( string name, float size )
{
    ofPtr< ofxGenericFont > create( new ofxGenericFont() );
    create->init( create, name, size );
    return create;
}

float ofxGenericFont::getSize()
{
    return _size;
}

string ofxGenericFont::getName()
{
    return _name;
}

NativeFont ofxGenericFont::getNativeFont()
{
    return _nativeFont;
}

void ofxGenericFont::init( ofPtrWeak< ofxGenericFont > setThis, string name, float size )
{
    _this = setThis;
    _name = name;
    _size = size;
    
#if TARGET_OS_IPHONE
    _nativeFont = [ UIFont fontWithName:ofxStringToNSString( _name ) size:_size ];
#endif
}

ofxGenericFont::~ofxGenericFont()
{
}

ofxGenericFont::ofxGenericFont()
{
}

