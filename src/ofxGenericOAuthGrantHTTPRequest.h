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
protected:
    static ofPtr< ofxGenericValueStore > createBody( string clientId, string clientSecretKey, string grantType );
    
    ofPtr< ofxGenericHTTPResponse > createResponse( int statusCode, string MIMEType, string textEncoding, void* data, int dataByteLength, string suggestedFileName );
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