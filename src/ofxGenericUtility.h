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

#include <list>

class ofxGenericCoordinate;

//////////////////////////////// Path //////////////////////////////////

string ofxGGetPathFromFileName( string fileName );

bool ofxGmkdir( string location, bool useDocuments );
bool ofxGrmdir( string location, bool useDocuments, bool onlyIfEmpty = true );

bool ofxGFileExists( string absolutePath );
bool ofxGFileExists( string location, bool useDocuments );

//////////////////////////////// Font //////////////////////////////////

float ofxGFontSizeForText( string text, string fontName, float startingFontSize, const ofPoint& constrainedSize );
ofPoint ofxGPointSizeForText( string text, string fontName, float fontSize, float constrainedWidth );

//////////////////////////////// Logging //////////////////////////////////

void ofxGLogError( const string & message );
void ofxGLogFatalError( const string & message );
void ofxGLogNotice( const string & message );
void ofxGLogVerbose( const string & message );
void ofxGLogWarning( const string & message );

//////////////////////////////// String conversions //////////////////////////////////

string ofxGToString( int value );
string ofxGToString( long int value );
string ofxGToString( unsigned int value );
string ofxGToString( unsigned long int value );
string ofxGToString( double value );
string ofxGToString( float value );
string ofxGToString( bool value );
string ofxGToString( const ofPoint& value, bool addSpaceAfterComma = false );
string ofxGToString( const ofxGenericCoordinate& value, bool addSpaceAfterComma = false );
string ofxGToString( const ofColor& color );
string ofxGToString( const ofRectangle& rect );
string ofxGToString( const void* value, unsigned int byteSize );
string ofxGToString( float value, unsigned int decimalCount );
string ofxGToString( double value, unsigned int decimalCount );
string ofxGTrimDecimals( string str, unsigned int decimalCount );
bool ofxGToBool( string value );

string ofxGSPrintf( string format, const std::vector< string >& replaceWith );
string ofxGSPrintf( string format, string first );
string ofxGSPrintf( string format, string first, string second );
string ofxGSPrintf( string format, string first, string second, string third );

std::vector< string > ofxGSplit( string value, char splitOn );

string ofxGToUpperCase( const string& text );
string ofxGToLowerCase( const string& text );

string ofxGURLSafeString( const string& text );
bool ofxGStringIsXMLSafe( const string& text );

ofPoint ofxGToPoint( const string& value );
ofxGenericCoordinate ofxGToCoordinate( const string& value );

//////////////////////////////// Math //////////////////////////////////

int ofxGRandom();
int ofxGRandomInRange( int minimum, int maximum );
int ofxGRandomInRange( std::pair< int, int > range );
int ofxGRandomInRangeExclusive( int minimumInclusive, int maximumExclusive );

ofRectangle ofxGGetCenteredSquareFit( const ofRectangle& rectangle );

//////////////////////////////// HTTP //////////////////////////////////

string ofxGMIMETypeToString( ofxGenericMIMEType type );
ofxGenericMIMEType ofxGStringToMIMEType( string type );
string ofxGMIMETypeToExtension( ofxGenericMIMEType type );

//////////////////////////////// Compression //////////////////////////////////

bool ofxGUntar( string tarLocation, bool tarIsInDocuments, string destinationWithinDocuments );
bool ofxGUnzip( string zipLocation, bool zipIsInDocuments, string destinationWithinDocuments );

//////////////////////////////// Color //////////////////////////////////

ofPoint ofxGToPoint( const ofColor& color );
ofColor ofxGToColor( const ofPoint& point );

template< typename _InputIterator, typename _Tp >
inline bool ofxGContains( _InputIterator __first, _InputIterator __end, const _Tp& __val )
{
    // concept requirements
    __glibcxx_function_requires( _InputIteratorConcept< _InputIterator > )
    __glibcxx_function_requires( _EqualOpConcept<
                                typename iterator_traits< _InputIterator >::value_type, _Tp > )
    __glibcxx_requires_valid_range( __first, __end );
    return std::__find( __first, __end, __val, std::__iterator_category( __first ) ) != __end;
}

template< typename ElementType >
bool ofxGContains( const std::list< ElementType >& container, const ElementType& value )
{
    return std::find( container.begin(), container.end(), value ) != container.end();
}
