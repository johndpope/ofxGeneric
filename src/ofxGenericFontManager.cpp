//
//  ofxGenericFontManager.cpp
//  ofxGeneric
//
//  Created by Jinwoo Baek on 7/23/14.
//  Copyright (c) 2014 Lumos Labs. All rights reserved.
//

#include "ofxGenericFontManager.h"

ofPtr< ofxGenericFont > ofxGenericFontManager::getLocalizedFontFromFontSettings(string fontName, float fontSize)
{
    if (this->fontCategoryIsInitialized()) {
        string categoryKey = this->convertFontToCategory(fontName);
        
        if (this->fontSettingIsInitialized()) {
            if (this->fontSettingExistForCategory(categoryKey)) {
                fontName = this->getLocalizedFontName(categoryKey);
                fontSize = this->getLocalizedFontSize(categoryKey, fontSize);
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

bool ofxGenericFontManager::fontSettingExistForCategory(string categoryKey)
{
    bool returnValue = false;
    string fontName = this->getLocalizedFontName(categoryKey);
    float fontSizeMultiplier = this->getLocalizedFontSizeMultiplier(categoryKey);
    if (fontName.size() > 0 && fontSizeMultiplier > 0.0f) {
        returnValue = true;
    }
    return returnValue;
}

string ofxGenericFontManager::getLocalizedFontName(string categoryKey)
{
    return _fontSettings->getLocalizedFontName(categoryKey);
}

float ofxGenericFontManager::getLocalizedFontSizeMultiplier(string categoryKey)
{
    return _fontSettings->getLocalizedFontSizeMultiplier(categoryKey);
}

float ofxGenericFontManager::getLocalizedFontSize(string categoryKey, float baseFontSize)
{
    float fontMultiplier = _fontSettings->getLocalizedFontSizeMultiplier(categoryKey);
    return baseFontSize * fontMultiplier;
}

string ofxGenericFontManager::convertFontToCategory(string fontName)
{
    return _fontCategories->convertFontToCategory(fontName);
}