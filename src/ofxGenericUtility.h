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
#endif

#include "ofxGenericConstants.h"

#if TARGET_ANDROID
#include <jni.h>
#endif

// memory macros
#define deleteSafe( cPlusPlusObject ) \
    if ( cPlusPlusObject ) \
        delete cPlusPlusObject; \
    cPlusPlusObject = NULL;

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

UITextAlignment ofxGenericTextHorizontalAlignmentToUITextAlignment( ofxGenericTextHorizontalAlignment from );
#endif

void ofxGLog( ofLogLevel level, const string & message );
void ofxGLogError( const string & message );
void ofxGLogFatalError( const string & message );
void ofxGLogNotice( const string & message );
void ofxGLogVerbose( const string & message );
void ofxGLogWarning( const string & message );
void ofxGLog( ofLogLevel level, const char* format, ... );

#if TARGET_ANDROID

#include <exception>
class JNIException : public std::exception
{
public:
	JNIException( string message ) : _what( message ) {}
	~JNIException() throw() {}
	const char* what() const throw() { return _what.c_str(); }

private:
	string _what;
};

jobject JNICallClassStaticObjectMethod( const char* className, const char* methodName, const char* methodSig );
#endif
