//
//  ofxGenericFontCategory.h
//  ofxGeneric
//
//  Created by Jinwoo Baek on 7/25/14.
//  Copyright (c) 2014 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericSettings.h"

class ofxGenericFontCategory : public ofxGenericSettings
{
    
public:
    virtual string convertFontToCategory(string fontName) = 0;
    
protected:
    void parse( ofPtr< ofxGenericValueStore >& settings );
    ofPtr< ofxGenericValueStore > _settings;
};