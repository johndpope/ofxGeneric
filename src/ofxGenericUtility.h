//
//  ofxGenericUtility.h
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 12/29/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofMain.h"

#import <UIKit/UIKit.h>
#import "ofxiPhoneExtras.h"

#define deleteSafe( cPlusPlusObject ) \
    if ( cPlusPlusObject ) \
        delete cPlusPlusObject; \
    cPlusPlusObject = NULL;

#define release( objectiveCObject ) \
    [ objectiveCObject release ]; \
    objectiveCObject = nil;

#define releaseView( objectiveCView ) \
    [ objectiveCView removeFromSuperview ]; \
    release( objectiveCView );

#define releaseViewController( objectCViewController ) \
    [ objectCViewController removeFromParentViewController ]; \
    [ objectCViewController release ]; \
    objectCViewController = nil;

UIColor* ofColorToUIColor( const ofColor& from );
ofColor UIColorToofColor( UIColor* from );

NSString* pathToBundle( NSString* resourceFileName = nil );

CGFloat getWindowScale();

CGRect ofRectangleToCGRect( const ofRectangle& from );
ofRectangle CGRectToofRectangle( const CGRect& from );


#define singletonInheretableHeader( className ) \
public: \
    virtual ~className(); \
    static className* getInstance(); \
\
protected: \
    className(); \
    static className* _instance; \
    void set ## className ## InstanceToThis();

#define singletonInheretableSource( className ) \
className* className::_instance = NULL; \
\
className* className::getInstance() \
{ \
    if ( className::_instance == NULL ) \
    { \
        ( new className() )->set ## className ## InstanceToThis(); \
    } \
    return className::_instance; \
}

#define singletonInheretableSourceBase( className ) \
singletonInheretableSource( className ); \
\
void className::set ## className ## InstanceToThis() \
{ \
    if ( className::_instance == NULL ) \
    { \
        className::_instance = this; \
    } else \
    { \
    } \
}

#define singletonInheretableSourceInhereted( className, superClassName ) \
singletonInheretableSource( className ); \
\
void className::set ## className ## InstanceToThis() \
{ \
    if ( className::_instance == NULL ) \
    { \
        className::_instance = this; \
        className::_instance->set ## superClassName ## InstanceToThis(); \
    } else \
    { \
    } \
}