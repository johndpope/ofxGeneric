//
//  ofxGenericLayoutConstraint.cpp
//  ofxGeneric
//
//  Created by Patrick Barry on 11/19/13.
//  Copyright (c) 2013 Lumos Labs. All rights reserved.
//

#include "ofxGenericLayoutConstraint.h"


ofPtr< ofxGenericLayoutConstraint > ofxGenericLayoutConstraint::create(ofPtr<ofxGenericView> firstView, ofxGenericLayoutAttribute firstViewAttribute, ofxGenericLayoutRelation relation, ofPtr<ofxGenericView> secondView, ofxGenericLayoutAttribute secondViewAttribute, float multiplier, float constant, float priority)
{
    ofPtr< ofxGenericLayoutConstraint > instance ( new ofxGenericLayoutConstraint() );
    if (instance)
    {
        instance->init(firstView, firstViewAttribute, relation, secondView, secondViewAttribute, multiplier, constant, priority);
    }
    
    return instance;
}

void ofxGenericLayoutConstraint::init(ofPtr<ofxGenericView> firstView, ofxGenericLayoutAttribute firstViewAttribute, ofxGenericLayoutRelation relation, ofPtr<ofxGenericView> secondView, ofxGenericLayoutAttribute secondViewAttribute, float multiplier, float constant, float priority)
{
    [firstView->getNativeView() setTranslatesAutoresizingMaskIntoConstraints:NO];
    [secondView->getNativeView() setTranslatesAutoresizingMaskIntoConstraints:NO];
    NSLayoutConstraint *constraint = [NSLayoutConstraint constraintWithItem:firstView->getNativeView() attribute:(NSLayoutAttribute)firstViewAttribute relatedBy:(NSLayoutRelation)relation toItem:secondView->getNativeView() attribute:(NSLayoutAttribute)secondViewAttribute multiplier:multiplier constant:constant];
    constraint.priority = priority;
    
    _nativeLayoutConstraint = [constraint retain];
}

NSLayoutConstraint* ofxGenericLayoutConstraint::getNativeLayoutConstraint()
{
    return _nativeLayoutConstraint;
}

ofxGenericLayoutConstraint::~ofxGenericLayoutConstraint()
{
    [_nativeLayoutConstraint release];
    _nativeLayoutConstraint = nil;
}