//
//  ofxGenericHMAC.cpp
//  ofxGeneric
//
//  Created by Konstantin Yavichev on 10/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericHMAC.h"

#if TARGET_OS_IPHONE
#include <CommonCrypto/CommonHMAC.h>
#endif

string ofxGenericHMAC::createHMAC( string key, string text )
{
    string result;

#if TARGET_OS_IPHONE    
    const char *cKey  = key.c_str();
    const char *cData = text.c_str();
    unsigned char cHMAC[CC_SHA1_DIGEST_LENGTH];
    
    CCHmac( kCCHmacAlgSHA1, cKey, strlen(cKey), cData, strlen(cData), cHMAC );
    
    NSMutableString* output = [NSMutableString stringWithCapacity:CC_SHA1_DIGEST_LENGTH * 2];
    for(int i = 0; i < CC_SHA1_DIGEST_LENGTH; i++)
    {
        [output appendFormat:@"%02x", cHMAC[i]];
    }
    result = string( [output UTF8String] );
#endif
    
    return result;
}
