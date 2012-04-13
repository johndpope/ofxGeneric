//
//  ofxGenericUtility.h
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 12/29/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofMain.h"

#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>
#import "ofxiPhoneExtras.h"
#elif TARGET_ANDROID
#include "JNIRect.h"
#endif

#include "ofxGenericConstants.h"

// memory macros
#define deleteSafe( cPlusPlusObject ) \
    if ( cPlusPlusObject ) \
    { \
        delete cPlusPlusObject; \
        cPlusPlusObject = NULL; \
    }

#define deleteThreadSafe( cPlusPlusObject, type ) \
    if ( cPlusPlusObject ) \
    { \
    	type buffer = cPlusPlusObject; \
        cPlusPlusObject = NULL; \
        delete buffer; \
    }

#if TARGET_OS_IPHONE
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
#endif

// singleton model macros

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

#if TARGET_OS_IPHONE
// iOS utilities
NSString* pathToBundle( NSString* resourceFileName = nil );

CGFloat getWindowScale();

UITextAlignment ofxGenericTextHorizontalAlignmentToiOS( ofxGenericTextHorizontalAlignment from );
ofxGenericTextHorizontalAlignment iOSToofxGenericTextHorizontalAlignment( UITextAlignment from );
UITableViewCellSeparatorStyle ofxGenericTableViewSeparatorStyleToiOS( ofxGenericTableViewSeparatorStyle from );
UIReturnKeyType     ofxGenericKeyboardReturnKeyToiOS( ofxGenericKeyboardReturnKey from );
UIKeyboardType ofxGenericKeyboardTypeToiOS( ofxGenericKeyboardType from );
UITextAutocapitalizationType ofxGenericTextAutoCapitalizationToiOS( ofxGenericTextAutoCapitalization from );
UIDeviceOrientation ofOrientationToiOS( ofOrientation from );
ofOrientation iOSToofOrientation( UIDeviceOrientation from );
UILineBreakMode ofxGenericTextLinebreakModeToiOS( ofxGenericTextLinebreakMode from );
ofxGenericTextLinebreakMode iOSToofxGenericTextLinebreakMode( UILineBreakMode from );
UIViewAnimationCurve ofxGenericViewAnimationCurveToiOS( ofxGenericViewAnimationCurve from );
UIViewAnimationTransition ofxGenericViewAnimationTransitionToiOS( ofxGenericViewAnimationTransition from );

#elif TARGET_ANDROID

JNIRect ofRectangleToJNIRect( const ofRectangle& from );
ofRectangle JNIRectToofRectangle( JNIRect& from );

jint ofColorToJNIColor( const ofColor& from );
ofColor JNIColorToofColor( const jint& from );

#endif

void ofxGLog( ofLogLevel level, const string & message );
void ofxGLogError( const string & message );
void ofxGLogFatalError( const string & message );
void ofxGLogNotice( const string & message );
void ofxGLogVerbose( const string & message );
void ofxGLogWarning( const string & message );
void ofxGLog( ofLogLevel level, const char* format, ... );
