//
//  ofxGenericFontManager.h
//  ofxGeneric
//
//  Created by Jinwoo Baek on 7/23/14.
//  Copyright (c) 2014 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericValueStore.h"
#include "ofxGenericFontSettings.h"
#include "ofxGenericFontCategory.h"
#include "ofxGenericFont.h"

class ofxGenericFontManager
{
public:
    static ofxGenericFontManager& getInstance()
    {
        static ofxGenericFontManager instance;
        return instance;
    }
    
    ofPtr< ofxGenericFont > getLocalizedFont(const string& inputFontName, float fontSize);
    
    void setFontSettings( ofPtr < ofxGenericFontSettings > fontSettings);
    void setFontCategories( ofPtr < ofxGenericFontCategory > fontCategories);
    
private:
    
    ofPtr < ofxGenericFontSettings > _fontSettings;
    ofPtr < ofxGenericFontCategory > _fontCategories;
    
    bool fontSettingIsInitialized();
    bool fontCategoryIsInitialized();
    bool fontSettingExistForCategory(const string& categoryKey);
    
    ofxGenericFontManager() {};
    ofxGenericFontManager(ofxGenericFontManager const&);
    void operator=(ofxGenericFontManager const&);
};