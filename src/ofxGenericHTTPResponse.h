//
//  ofxGenericHTTPResponse.h
//  iOS
//
//  Created by Ian Grossberg on 1/23/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofMain.h"

class ofxGenericHTTPResponse
{
public:
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
        
    ofxGenericHTTPResponse();
#if TARGET_OS_IPHONE
    ofxGenericHTTPResponse( NSError* error );
    ofxGenericHTTPResponse( NSURLResponse* response, NSData* data );
#endif
    
    virtual ~ofxGenericHTTPResponse();
    
protected:
#if TARGET_OS_IPHONE
    NSData* _dataSource;
#endif
};
