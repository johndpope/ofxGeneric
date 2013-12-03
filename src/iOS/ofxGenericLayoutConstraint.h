//
//  ofxGenericLayoutConstraint.h
//  ofxGeneric
//
//  Created by Patrick Barry on 11/19/13.
//  Copyright (c) 2013 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"
#include "ofxGenericView.h"


enum ofxGenericLayoutAttribute
{
    ofxGenericLayoutAttributeLeft = NSLayoutAttributeLeft,
    ofxGenericLayoutAttributeRight = NSLayoutAttributeRight,
    ofxGenericLayoutAttributeTop = NSLayoutAttributeTop,
    ofxGenericLayoutAttributeBottom = NSLayoutAttributeBottom,
    ofxGenericLayoutAttributeLeading = NSLayoutAttributeLeading,
    ofxGenericLayoutAttributeTrailing = NSLayoutAttributeTrailing,
    ofxGenericLayoutAttributeWidth = NSLayoutAttributeWidth,
    ofxGenericLayoutAttributeHeight = NSLayoutAttributeHeight,
    ofxGenericLayoutAttributeCenterX = NSLayoutAttributeCenterX,
    ofxGenericLayoutAttributeCenterY = NSLayoutAttributeCenterY,
    ofxGenericLayoutAttributeBaseline = NSLayoutAttributeBaseline,
    
    ofxGenericLayoutAttributeNotAnAttribute = NSLayoutAttributeNotAnAttribute
};

enum ofxGenericLayoutRelation
{
    ofxGenericLayoutRelationLessThanOrEqual = NSLayoutRelationLessThanOrEqual,
    ofxGenericLayoutRelationEqual = NSLayoutRelationEqual,
    ofxGenericLayoutRelationGreaterThanOrEqual = NSLayoutRelationGreaterThanOrEqual,
};

class ofxGenericLayoutConstraint
{
public:
    
    static ofPtr< ofxGenericLayoutConstraint > create(
                                                      ofPtr< ofxGenericView >firstView,
                                                      ofxGenericLayoutAttribute firstViewAttribute,
                                                      ofxGenericLayoutRelation relation,
                                                      ofPtr< ofxGenericView >secondView,
                                                      ofxGenericLayoutAttribute secondViewAttribute,
                                                      float multiplier,
                                                      float constant,
                                                      float priority
                                                      );
    virtual ~ofxGenericLayoutConstraint();
    
    NSLayoutConstraint* getNativeLayoutConstraint();
    
    float getConstant() { return getNativeLayoutConstraint().constant; }
    void setConstant( float constant ) { getNativeLayoutConstraint().constant = constant; }
    void changeConstantByDelta (float delta) { getNativeLayoutConstraint().constant += delta; }
    
private:
    ofxGenericLayoutConstraint() {}
    void init(
              ofPtr< ofxGenericView >firstView,
              ofxGenericLayoutAttribute firstViewAttribute,
              ofxGenericLayoutRelation relation,
              ofPtr< ofxGenericView >secondView,
              ofxGenericLayoutAttribute secondViewAttribute,
              float multiplier,
              float constant,
              float priority
              );
    
    NSLayoutConstraint *_nativeLayoutConstraint;

};