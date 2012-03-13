//
//  ofxGenericHTTPResponse.h
//  iOS
//
//  Created by Ian Grossberg on 1/23/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofMain.h"
#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>
#endif

class ofxGenericHTTPResponse
{
public:
    static ofPtr< ofxGenericHTTPResponse > create();
    static ofPtr< ofxGenericHTTPResponse > create( string setErrorDescription, string setErrorFailureReason = "", string setErrorRecoverySuggestions = "" );
    static ofPtr< ofxGenericHTTPResponse > create( int statusCode, string MIMEType, string textEncoding, void* data, int dataByteLength, string suggestedFilename = "" );
#if TARGET_OS_IPHONE
    static ofPtr< ofxGenericHTTPResponse > create( NSURLResponse* response, NSData* data );
    static ofPtr< ofxGenericHTTPResponse > create( NSError* error );
#endif
    
    // TODO: readonly accessors
    int statusCode;
    string MIMEType;
    string textEncoding;
    string suggestedFilename;
    void* data;
    int dataByteLength;
    
    string errorDescription;
    string errorFailureReason;
    string errorRecoverySuggestions;
        
    virtual ~ofxGenericHTTPResponse();
    
protected:
    ofxGenericHTTPResponse();
    
    virtual void init( ofPtrWeak< ofxGenericHTTPResponse > setThis );
    virtual void init( ofPtrWeak< ofxGenericHTTPResponse > setThis, int statusCode, string MIMEType, string textEncoding, void* data, int dataByteLength, string suggestedFilename = "" );
    virtual void init( ofPtrWeak< ofxGenericHTTPResponse > setThis, string setErrorDescription, string setErrorFailureReason = "", string setErrorRecoverySuggestions = ""  );
    
    ofPtrWeak< ofxGenericHTTPResponse > _this;
    
#if TARGET_OS_IPHONE
    NSData* _dataSource;
#endif
};
