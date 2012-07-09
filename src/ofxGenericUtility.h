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
#include "ofxGenericUtilityiOS.h"
#elif TARGET_ANDROID
#include "ofxGenericUtilityAndroid.h"
#endif

//////////////////////////////// Path //////////////////////////////////

string ofxGPathToDataFolder( string fileName = string() );
string ofxGPathToDocumentsFolder( string fileName = string() );

string ofxGGetPathFromFileName( string fileName );

//returns 0 on success, -1 on failure (including if the file already exists)
int ofxGmkdir( string loc, bool useDocuments );

//////////////////////////////// Font //////////////////////////////////

float ofxGFontSizeForText( string text, string fontName, float startingFontSize, const ofPoint& constrainedSize );
ofPoint ofxGPointSizeForText( string text, string fontName, float fontSize, float constrainedWidth );

//////////////////////////////// Logging //////////////////////////////////

void ofxGLog( ofLogLevel level, const string & message );
void ofxGLogError( const string & message );
void ofxGLogFatalError( const string & message );
void ofxGLogNotice( const string & message );
void ofxGLogVerbose( const string & message );
void ofxGLogWarning( const string & message );
void ofxGLog( ofLogLevel level, const char* format, ... ); // TODO: remove, unsafe

//////////////////////////////// String conversions //////////////////////////////////

#define intToString( value ) ofxGIntegerToString( value )
string ofxGIntegerToString( int value );
#define floatToString( value ) ofxGFloatToString( value )
string ofxGFloatToString( float value );

string ofxGSPrintf( string format, const std::vector< string >& replaceWith );
string ofxGSPrintf( string format, string first );
string ofxGSPrintf( string format, string first, string second );
string ofxGSPrintf( string format, string first, string second, string third );

//////////////////////////////// Math //////////////////////////////////

int ofxRandomInRange( int minimum, int maximum );
