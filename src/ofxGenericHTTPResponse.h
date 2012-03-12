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
    static ofPtr< ofxGenericHTTPResponse > create( NSError* error );
    static ofPtr< ofxGenericHTTPResponse > create( NSURLResponse* response, NSData* data );
    
    // TODO: readonly
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
#if TARGET_OS_IPHONE
    ofxGenericHTTPResponse( NSError* error );
    ofxGenericHTTPResponse( NSURLResponse* response, NSData* data );
#endif    
    
    ofPtrWeak< ofxGenericHTTPResponse > _this;
    
#if TARGET_OS_IPHONE
    NSData* _dataSource;
#endif
};
