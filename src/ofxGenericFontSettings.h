//
//  ofxGenericFontSettings.h
//  ofxGeneric
//
//  Created by Jinwoo Baek on 7/24/14.
//  Copyright (c) 2014 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericSettings.h"

class ofxGenericFontSettings : public ofxGenericSettings
{

public:
    virtual string getLocalizedFontName(string categoryKey) = 0;

protected:
    void parse( ofPtr< ofxGenericValueStore >& settings );
    ofPtr< ofxGenericValueStore > _settings;
};