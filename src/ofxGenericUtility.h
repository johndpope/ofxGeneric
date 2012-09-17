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

bool ofxGmkdir( string loc, bool useDocuments );
bool ofxGrmdir( string loc, bool useDocuments, bool onlyIfEmpty = true );

bool ofxGFileExists( string absolutePath );
bool ofxGFileExists( string location, bool useDocuments );

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

string ofxGToString( int value );
string ofxGToString( long int value );
string ofxGToString( unsigned int value );
string ofxGToString( unsigned long int value );
string ofxGToString( double value );
string ofxGToString( float value );
string ofxGToString( bool value );
string ofxGToString( const ofPoint& point );
string ofxGToString( const ofColor& color );
string ofxGToString( const void* value, unsigned int byteSize );
bool ofxGToBool( string value );

string ofxGSPrintf( string format, const std::vector< string >& replaceWith );
string ofxGSPrintf( string format, string first );
string ofxGSPrintf( string format, string first, string second );
string ofxGSPrintf( string format, string first, string second, string third );

std::vector< string > ofxGSplit( string value, char splitOn );

//////////////////////////////// Math //////////////////////////////////

int ofxGRandom();
int ofxGRandomInRange( int minimum, int maximum );
int ofxGRandomInRangeExclusive( int minimumInclusive, int maximumExclusive );

//////////////////////////////// HTTP //////////////////////////////////

string ofxGMIMETypeToString( ofxGenericMIMEType type );
ofxGenericMIMEType ofxGStringToMIMEType( string type );
string ofxGMIMETypeToExtension( ofxGenericMIMEType type );