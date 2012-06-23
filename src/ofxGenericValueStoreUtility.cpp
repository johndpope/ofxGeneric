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