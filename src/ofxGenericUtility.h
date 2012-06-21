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

#include "ofxGenericConstants.h"

#include "ofxGenericException.h"

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
    [ objectCViewController release ]; \
    objectCViewController = nil;
#endif

string ofxGPathToDataFolder( string fileName = string() );
string ofxGPathToDocumentsFolder( string fileName = string() );

#if TARGET_OS_IPHONE
// iOS utilities
CGFloat getWindowScale();

UITextAlignment ofxGenericTextHorizontalAlignmentToiOS( ofxGenericTextHorizontalAlignment from );
ofxGenericTextHorizontalAlignment iOSToofxGenericTextHorizontalAlignment( UITextAlignment from );

UITableViewCellSeparatorStyle ofxGenericTableViewSeparatorStyleToiOS( ofxGenericTableViewSeparatorStyle from );

UIReturnKeyType     ofxGenericKeyboardReturnKeyToiOS( ofxGenericKeyboardReturnKey from );
ofxGenericKeyboardReturnKey iOSToofxGenericKeyboardReturnKey( UIReturnKeyType from );

UIKeyboardType ofxGenericKeyboardTypeToiOS( ofxGenericKeyboardType from );
ofxGenericKeyboardType iOSToofxGenericKeyboardType( UIKeyboardType from );

UITextAutocapitalizationType ofxGenericTextAutoCapitalizationToiOS( ofxGenericTextAutoCapitalization from );
ofxGenericTextAutoCapitalization iOSToofxGenericTextAutoCapitalization( UITextAutocapitalizationType from );

UIDeviceOrientation ofOrientationToiOS( ofOrientation from );
ofOrientation iOSToofOrientation( UIDeviceOrientation from );

UIInterfaceOrientation ofInterfaceOrientationToiOS( ofOrientation from );
ofOrientation iOSToofOrientation( UIInterfaceOrientation from );

UILineBreakMode ofxGenericTextLinebreakModeToiOS( ofxGenericTextLinebreakMode from );
ofxGenericTextLinebreakMode iOSToofxGenericTextLinebreakMode( UILineBreakMode from );

UIViewAnimationCurve ofxGenericViewAnimationCurveToiOS( ofxGenericViewAnimationCurve from );
UIViewAnimationTransition ofxGenericViewAnimationTransitionToiOS( ofxGenericViewAnimationTransition from );

UIViewAutoresizing ofxGenericViewAutoresizingToiOS( ofxGenericViewAutoresizing from );
ofxGenericViewAutoresizing iOSToofxGenericViewAutoresizing( UIViewAutoresizing from );

UITextBorderStyle ofxGenericTextViewBorderStyleToiOS( ofxGenericTextViewBorderStyle from );
ofxGenericTextViewBorderStyle iOSToofxGenericTextViewBorderStyle( UITextBorderStyle from );

UISwipeGestureRecognizerDirection ofxGenericGestureTypeSwipeToiOS( ofxGenericGestureTypeSwipe from );
ofxGenericGestureTypeSwipe iOSToofxGenericGestureTypeSwipe( UISwipeGestureRecognizerDirection from );

UIButtonType ofxGenericButtonTypeToiOS( ofxGenericButtonType from );
ofxGenericButtonType iOSToofxGenericButtonType( UIButtonType from );

UIImage* OFImageToUIImage( ofImage& image );

#elif TARGET_ANDROID

JNIRect ofRectangleToJNIRect( const ofRectangle& from );
ofRectangle JNIRectToofRectangle( JNIRect& from );

jint ofColorToJNIColor( const ofColor& from );
ofColor JNIColorToofColor( const jint& from );

#endif

float ofxGFontSizeForText( string text, string fontName, float startingFontSize, const ofPoint& constrainedSize );

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

//returns 0 on success, -1 on failure (including if the file already exists)
int ofxGmkdir( string loc, bool useDocuments );
