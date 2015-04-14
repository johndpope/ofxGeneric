//
//  ofCommon.m
//  ofxGeneric
//
//  Created by Dusan Terzic on 4/14/15.
//  Copyright (c) 2015 Lumos Labs. All rights reserved.
//

#import "ofCommon.h"

#import <UIKit/UIKit.h>
#include <vector>

using namespace std;

@implementation ofCommon


CGRect ofxRectangleToCGRect(const ofRectangle & from){
    return CGRectMake(from.x, from.y, from.width, from.height);
}

ofRectangle ofxCGRectToofRectangle(const CGRect & from){
    return ofRectangle(from.origin.x, from.origin.y, from.size.width, from.size.height);
}

string ofxNSStringToString(NSString * s){
    const char * stringBuffer = [s UTF8String];
    if(stringBuffer && strlen(stringBuffer) > 0){
        return string(stringBuffer);
    }
    return string();
}

NSString * ofxStringToNSString(string s){
    return [[[NSString alloc] initWithCString: s.c_str() encoding:NSUTF8StringEncoding] autorelease];
}

inline float ofxColorUnsignedCharToFloat(unsigned char component){
    if(component < 0){
        component = 0;
    }
    return ((float)component) / 255.0f;
}

inline unsigned char ofxColorFloatToUnsignedChar(float component){
    return (unsigned char)(component * 255.0f);
}

ofColor ofxUIColorToofColor(UIColor * from){
    ofColor to;
    CGColorRef color = [from CGColor];
    
    const float * colorComponents = (const float *) CGColorGetComponents(color);
    to.r = 0;
    to.g = 0;
    to.b = 0;
    to.a = ofxColorFloatToUnsignedChar(CGColorGetAlpha(color));
    
    if(CGColorGetNumberOfComponents(color) == 2){
        to.r = to.g = to.b = ofxColorFloatToUnsignedChar(colorComponents[0]);
    }
    else if(CGColorGetNumberOfComponents(color) >= 3){
        to.r = ofxColorFloatToUnsignedChar(colorComponents[0]);
        to.g = ofxColorFloatToUnsignedChar(colorComponents[1]);
        to.b = ofxColorFloatToUnsignedChar(colorComponents[2]);
    }
    return to;
}

UIColor * ofxColorToUIColor(const ofColor & from){
    UIColor * to = [[UIColor alloc] initWithRed: ofxColorUnsignedCharToFloat(from.r)
                                          green: ofxColorUnsignedCharToFloat(from.g)
                                           blue: ofxColorUnsignedCharToFloat(from.b)
                                          alpha: ofxColorUnsignedCharToFloat(from.a)];
    [to autorelease];
    return to;
}

@end
