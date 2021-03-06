//
//  ofxGenericFontManager.cpp
//  ofxGeneric
//
//  Created by Jinwoo Baek on 7/23/14.
//  Copyright (c) 2014 Lumos Labs. All rights reserved.
//

#include "ofxGenericFontManager.h"

ofPtr< ofxGenericFont > ofxGenericFontManager::getLocalizedFont(const string& inputFontName, float fontSize)
{
    string fontName = inputFontName;
    if (this->fontCategoryIsInitialized()) {
        string categoryKey = _fontCategories->convertFontToCategory(fontName);
        
        if (this->fontSettingIsInitialized()) {
            if (this->fontSettingExistForCategory(categoryKey)) {
                fontName = _fontSettings->getLocalizedFontName(categoryKey);
            }
        }
    }
    ofPtr< ofxGenericFont > font = ofxGenericFont::create( fontName, fontSize );
    return font;
}

void ofxGenericFontManager::setFontSettings( ofPtr < ofxGenericFontSettings > fontSettings)
{
    _fontSettings = fontSettings;
}

bool ofxGenericFontManager::fontSettingIsInitialized()
{
    return (_fontSettings);
}

void ofxGenericFontManager::setFontCategories(ofPtr<ofxGenericFontCategory> fontCategories)
{
    _fontCategories = fontCategories;
}

bool ofxGenericFontManager::fontCategoryIsInitialized()
{
    return (_fontCategories);
}

bool ofxGenericFontManager::fontSettingExistForCategory(const string& categoryKey)
{
    bool returnValue = false;
    string fontName = _fontSettings->getLocalizedFontName(categoryKey);
    if (!fontName.empty()) {
        returnValue = true;
    }
    return returnValue;
}
