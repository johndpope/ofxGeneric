//
//  ofxGenericValueStoreUtility.cpp
//  ofxGeneric
//
//  Created by Ian Grossberg on 6/8/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericValueStoreUtility.h"

ofPtr< ofxGenericValueStore > ofxGGetWithCaseInsensitiveKey( ofPtr< ofxGenericValueStore > from, string key )
{
    ofPtr< ofxGenericValueStore > value;
    
    if ( from )
    {
        value = from->read( key );
        if ( !value )
        {
            string lowerKey = ofxGToLowerCase( key );
            if ( lowerKey != key )
            {
                value = from->read( lowerKey );
            }
        }
        if ( !value )
        {
            string upperKey = ofxGToUpperCase( key );
            if ( upperKey != key )
            {
                value = from->read( upperKey );
            }
        }
        // TODO: and everything in between
    }
    
    return value;
}


int ofxGToInt( ofPtr< ofxGenericValueStore > from )
{
    if ( from )
    {
        return from->asInt();
    }
    return 0;
}

int ofxGValueToInt( ofPtr< ofxGenericValueStore > from, string key )
{
    return ofxGToInt( ofxGGetWithCaseInsensitiveKey( from, key ) );
}

#define ColorRedCacheKey "Red"
#define ColorGreenCacheKey "Green"
#define ColorBlueCacheKey "Blue"
#define ColorAlphaCacheKey "Alpha"

ofColor toofColor( ofPtr< ofxGenericValueStore > from )
{
    if ( from && from->isObject() )
    {
        return ofColor(
                       ofxGValueToInt( from, ColorRedCacheKey ),
                       ofxGValueToInt( from, ColorGreenCacheKey ),
                       ofxGValueToInt( from, ColorBlueCacheKey ),
                       ofxGValueToInt( from, ColorAlphaCacheKey )
                       );
    }
    return ofColor();
}

#define RectangleXCacheKey "X"
#define RectangleYCacheKey "Y"
#define RectangleWidthCacheKey "Width"
#define RectangleHeightCacheKey "Height"

ofRectangle toofRectangle( ofPtr< ofxGenericValueStore > from )
{
    if ( from && from->isObject() )
    {
        return ofRectangle( 
                       ofxGValueToInt( from, RectangleXCacheKey ),
                       ofxGValueToInt( from, RectangleYCacheKey ),
                       ofxGValueToInt( from, RectangleWidthCacheKey ),
                       ofxGValueToInt( from, RectangleHeightCacheKey )
                       );
    }
    return ofRectangle();
}

#define RectangleLowerXCacheKey "x"
#define RectangleLowerYCacheKey "y"
#define RectangleLowerWidthCacheKey "width"
#define RectangleLowerHeightCacheKey "height"

ofRectangle toofRectangleLowercase( ofPtr< ofxGenericValueStore > from )
{
    if ( from && from->isObject() )
    {
        return ofRectangle(
                           ofxGValueToInt( from, RectangleLowerXCacheKey ),
                           ofxGValueToInt( from, RectangleLowerYCacheKey ),
                           ofxGValueToInt( from, RectangleLowerWidthCacheKey ),
                           ofxGValueToInt( from, RectangleLowerHeightCacheKey )
                           );
    }
    return ofRectangle();
}

#define PointXCacheKey "X"
#define PointYCacheKey "Y"

ofPoint toofPoint( ofPtr< ofxGenericValueStore > from )
{
    if ( from && from->isObject() )
    {
        ofPtr< ofxGenericValueStore > x = from->read( PointXCacheKey );
        return ofPoint(
                       ofxGValueToInt( from, PointXCacheKey ),
                       ofxGValueToInt( from, PointYCacheKey )
                       );
    }
    return ofPoint();
}

#define PointWidthCacheKey "Width"
#define PointHeightCacheKey "Height"

ofPoint toofPointSize( ofPtr< ofxGenericValueStore > from )
{
    if ( from && from->isObject() )
    {
        return ofPoint(
                       ofxGValueToInt( from, PointWidthCacheKey ),
                       ofxGValueToInt( from, PointHeightCacheKey )
                       );
    }
    return ofPoint();
}

#define TextAlignmentLeft "left"
#define TextAlignmentCenter "center"
#define TextAlignmentRight "right"

ofxGenericTextHorizontalAlignment toofxGenericTextHorizontalAlignment( ofPtr< ofxGenericValueStore > from )
{
    string name = from->asString();
    if ( name == TextAlignmentLeft )
    {
        return ofxGenericTextHorizontalAlignmentLeft;
    }
    else if ( name == TextAlignmentRight )
    {
        return ofxGenericTextHorizontalAlignmentRight;
    }
    else if ( name == TextAlignmentCenter )
    {
        return ofxGenericTextHorizontalAlignmentCenter;
    }
    return ofxGenericTextHorizontalAlignmentLeft;
}

std::vector< unsigned int > toVectorOfUnsignedInts( ofPtr< ofxGenericValueStore > from )
{
    std::vector< unsigned int > ints;
    if ( from && from->isArray() )
    {
        for( ofxGenericValueStoreArrayIterator travValues = from->arrayBegin(); travValues != from->arrayEnd(); travValues ++ )
        {
            if ( ( *travValues ) )
            {
                ints.push_back( ( unsigned int )( *travValues )->asInt() ); // TODO: Uint support
            }
        }
    }
    return ints;
}

std::vector< string > toVectorOfStrings( ofPtr< ofxGenericValueStore > from )
{
    std::vector< string > strings;
    if ( from && from->isArray() )
    {
        for( ofxGenericValueStoreArrayIterator travValues = from->arrayBegin(); travValues != from->arrayEnd(); travValues ++ )
        {
            if ( ( *travValues ) )
            {
                strings.push_back( ( *travValues )->asString() );
            }
        }
    }
    return strings;    
}
