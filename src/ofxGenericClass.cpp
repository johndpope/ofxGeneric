//
//  ofxGenericClass.cpp
//  ofxGeneric
//
//  Created by Ian Grossberg on 3/13/13.
//  Copyright (c) 2013 Lumos Labs. All rights reserved.
//

#include "ofxGenericClass.h"

ofxGenericClass::ofxGenericClass()
{
}

void ofxGenericClass::init( ofPtrWeak< ofxGenericClass > setThis )
{
    _this = setThis;
}

ofxGenericClass::~ofxGenericClass()
{
}

std::string ofxGenericClass::toString() const
{
    std::string result;
#if DEBUG
    const std::type_info& typeInfo = typeid( *this );
    result += typeInfo.name();
#endif
    return result;
}