//
//  ofCommon.h
//  ofxGeneric
//
//  Created by Dusan Terzic on 4/7/15.
//  Copyright (c) 2015 Lumos Labs. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

#import "ofRectangle.h"
#import "ofColor.h"

@interface ofCommon : NSObject

CGRect ofxRectangleToCGRect(const ofRectangle & from);
ofRectangle ofxCGRectToofRectangle(const CGRect & from);

string ofxNSStringToString(NSString * s);
NSString * ofxStringToNSString(string s);

inline CGFloat ofxColorUnsignedCharToFloat(unsigned char component);
inline unsigned char ofxColorFloatToUnsignedChar(CGFloat component);
UIColor * ofxColorToUIColor(const ofColor & from);
ofColor ofxUIColorToofColor(UIColor * from);

@end
