//
//  ofxGenericUtility.h
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 12/29/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"

#if TARGET_OS_IPHONE
#import "ofxiPhoneExtras.h"
#elif TARGET_ANDROID
#include "JNIRect.h"
#endif

// memory macros
#if TARGET_OS_IPHONE
#define release( objectiveCObject ) \
    [ objectiveCObject release ]; \
    objectiveCObject = nil;

#define releaseView( objectiveCView ) \
    [ objectiveCView removeFromSuperview ]; \
    release( objectiveCView );

#define releaseViewController( objectCViewController ) \
    [ objectCViewController release ]; \
    objectCViewController = nil;
#endif

string ofxGPathToDataFolder( string fileName = string() );
string ofxGPathToDocumentsFolder( string fileName = string() );

#if TARGET_OS_IPHONE

#include "ofxGenericUtilityiOS.h"

#elif TARGET_ANDROID

JNIRect ofRectangleToJNIRect( const ofRectangle& from );
ofRectangle JNIRectToofRectangle( JNIRect& from );

jint ofColorToJNIColor( const ofColor& from );
ofColor JNIColorToofColor( const jint& from );

#endif

float ofxGFontSizeForText( string text, string fontName, float startingFontSize, const ofPoint& constrainedSize );
ofPoint ofxGPointSizeForText( string text, string fontName, float fontSize, float constrainedWidth );

void ofxGLog( ofLogLevel level, const string & message );
void ofxGLogError( const string & message );
void ofxGLogFatalError( const string & message );
void ofxGLogNotice( const string & message );
void ofxGLogVerbose( const string & message );
void ofxGLogWarning( const string & message );
void ofxGLog( ofLogLevel level, const char* format, ... );

#define intToString( value ) ofxGIntegerToString( value )
string ofxGIntegerToString( int value );
#define floatToString( value ) ofxGFloatToString( value )
string ofxGFloatToString( float value );

string ofxGGetPathFromFileName( string fileName );
//returns 0 on success, -1 on failure (including if the file already exists)
int ofxGmkdir( string loc, bool useDocuments );

int ofxRandomInRange( int minimum, int maximum );
