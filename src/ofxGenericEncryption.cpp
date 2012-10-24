//
//  ofxGenericEncryption.cpp
//  ofxGeneric
//
//  Created by Ian Grossberg on 10/24/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericEncryption.h"

#include "AESCrypt.h"

string ofxGenericEncryption::encrypt( string raw, string key )
{
    string result;
#if TARGET_OS_IPHONE
    NSString* encrypted = [ AESCrypt encrypt:ofxStringToNSString( raw ) password:ofxStringToNSString( key ) ];
    result = ofxNSStringToString( encrypted );
#endif
    return result;
}

string ofxGenericEncryption::decrypt( string encrypted, string key )
{
    string result;
#if TARGET_OS_IPHONE
    NSString* decrypted = [ AESCrypt decrypt:ofxStringToNSString( encrypted ) password:ofxStringToNSString( key ) ];
    result = ofxNSStringToString( decrypted );
#endif
    return result;
}