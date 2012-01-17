//
//  ofxGenericUtility.mm
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 12/30/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#include "ofxGenericUtility.h"

inline float ColorUnsignedCharToFloat( unsigned char component )
{
    return ( ( float )component ) / 255.0f;
}

inline unsigned char ColorFloatToUnsignedChar( float component )
{
    return ( unsigned char )( component * 255.0f );
}

UIColor* ofColorToUIColor( const ofColor& from )
{
    UIColor* to = [ [ UIColor alloc ] initWithRed: ColorUnsignedCharToFloat( from.r )
                                            green: ColorUnsignedCharToFloat( from.g )
                                             blue: ColorUnsignedCharToFloat( from.b )
                                            alpha: ColorUnsignedCharToFloat( from.a ) ];
    [ to autorelease ];
    return to;
}

ofColor UIColorToofColor( UIColor* from )
{
    ofColor to;
    CGFloat red, green, blue, alpha;
    [ from getRed:&red green:&green blue:&blue alpha:&alpha ];
    to.r = ColorFloatToUnsignedChar( red );
    to.g = ColorFloatToUnsignedChar( green );
    to.b = ColorFloatToUnsignedChar( blue );
    to.a = ColorFloatToUnsignedChar( alpha );
    return to;
}

NSString* pathToBundle( NSString* resourceFileName )
{
    NSString* bundlePath = [ [ NSBundle mainBundle ] resourcePath ];
    if ( resourceFileName != nil )
    {
        return [ NSString stringWithFormat:@"%@/%@", bundlePath, resourceFileName ];
    }
    return [ NSString stringWithFormat:@"%@/", bundlePath ];
}

CGFloat getWindowScale()
{
    if ( [ [ UIScreen mainScreen ] respondsToSelector:@selector( scale ) ] ) 
    {
        return [ [ UIScreen mainScreen ] scale ];
    }
    return 1;
}

CGRect ofRectangleToCGRect( const ofRectangle& from )
{
    return CGRectMake( from.x, from.y, from.width, from.height );
}

ofRectangle CGRectToofRectangle( const CGRect& from )
{
    return ofRectangle( from.origin.x, from.origin.y, from.size.width, from.size.height );
}