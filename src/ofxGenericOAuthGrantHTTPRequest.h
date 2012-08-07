//
//  ofxGenericOAuthGrantHTTPRequest.h
//  iOS
//
//  Created by Ian Grossberg on 8/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericOAuthHTTPRequest.h"
#include "ofxGenericHTTPResponse.h"

class ofxGenericOAuthToken;

class ofxGenericOAuthGrantHTTPRequest : public ofxGenericOAuthHTTPRequest
{
public:    
    static void addGrantType( ofPtr< ofxGenericValueStore > body, string grantType );

protected:
    virtual void init(
                      ofPtrWeak< ofxGenericOAuthGrantHTTPRequest > setThis,
                      string url,
                      string method,
                      ofPtr< ofxGenericOAuthToken > token,
                      string grantType,
                      string format,
                      ofPtr< ofxGenericValueStore > body,
                      ofPtr< ofxGenericHTTPRequestDelegate > delegate = ofPtr< ofxGenericHTTPRequestDelegate >(),
                      float timeout = ofxGenericHTTPRequestDefaultTimeout,
                      string clientId = "",
                      string clientSecretKey = ""
                      );
    
    ofPtr< ofxGenericHTTPResponse > createResponse(
                                                   int statusCode,
                                                   string MIMEType,
                                                   string textEncoding,
                                                   void* body,
                                                   unsigned int bodyByteLength,
                                                   string suggestedFileName
                                                   );
};

class ofxGenericOAuthGrantHTTPResponse : public ofxGenericHTTPResponse
{
public:
    static ofPtr< ofxGenericOAuthGrantHTTPResponse > create( int statusCode, string MIMEType, string textEncoding, void* data, unsigned int dataByteLength, string suggestedFilename = "" );
    
    ofPtr< ofxGenericOAuthToken > getToken();
    
    string getAccessToken();
    string getScope();

protected:
    ofxGenericOAuthGrantHTTPResponse();
};