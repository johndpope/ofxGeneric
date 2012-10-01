//
//  ofxGenericValueStoreUtility.cpp
//  ofxGeneric
//
//  Created by Ian Grossberg on 6/8/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericValueStoreUtility.h"

#define ColorRedCacheKey "red"
#define ColorGreenCacheKey "green"
#define ColorBlueCacheKey "blue"
#define ColorAlphaCacheKey "alpha"

ofColor toofColor( ofPtr< ofxGenericValueStore > from )
{
    if ( from && from->isObject() )
    {
        return ofColor( 
                       from->read( ColorRedCacheKey,   0 ),
                       from->read( ColorGreenCacheKey, 0 ),
                       from->read( ColorBlueCacheKey,  0 ),
                       from->read( ColorAlphaCacheKey, 0 ) 
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
                       from->read( RectangleXCacheKey,   0 ),
                       from->read( RectangleYCacheKey, 0 ),
                       from->read( RectangleWidthCacheKey,  0 ),
                       from->read( RectangleHeightCacheKey, 0 ) 
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
        return ofPoint(
                       from->read( PointXCacheKey,   0 ),
                       from->read( PointYCacheKey, 0 )
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
                       from->read( PointWidthCacheKey,   0 ),
                       from->read( PointHeightCacheKey, 0 )
                       );
    }
    return ofPoint();
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
