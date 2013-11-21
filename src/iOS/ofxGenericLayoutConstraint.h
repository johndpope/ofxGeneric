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

#import <UIKit/NSLayoutConstraint.h>


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
    
private:
    ofxGenericLayoutConstraint();
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