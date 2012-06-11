//
//  ofxGenericCacheUtility.cpp
//  ofxGeneric
//
//  Created by Ian Grossberg on 6/8/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericCacheUtility.h"

#define ColorRedCacheKey "red"
#define ColorGreenCacheKey "green"
#define ColorBlueCacheKey "blue"
#define ColorAlphaCacheKey "alpha"

ofColor toofColor( ofPtr< ofxGenericCache > from )
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