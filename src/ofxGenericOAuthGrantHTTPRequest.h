//
//  ofxGenericOAuthGrantHTTPRequest.h
//  iOS
//
//  Created by Ian Grossberg on 8/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericOAuthHTTPRequest.h"

class ofxGenericOAuthToken;

class ofxGenericOAuthGrantHTTPRequest : public ofxGenericOAuthHTTPRequest
{
public:    
    static void addGrantType( ofPtr< ofxGenericValueStore > body, string grantType );

    ofPtr< ofxGenericOAuthToken > getToken();
    
    string getAccessToken();
    string getScope();
    
protected:
    virtual void init(
                      ofPtrWeak< ofxGenericOAuthGrantHTTPRequest > setThis,
                      string url,
                      string method,
                      ofPtr< ofxGenericOAuthToken > token,
                      string grantType,
                      ofxGenericMIMEType format,
                      ofPtr< ofxGenericValueStore > body,
                      ofPtr< ofxGenericHTTPRequestDelegate > delegate = ofPtr< ofxGenericHTTPRequestDelegate >(),
                      float timeout = ofxGenericHTTPRequestDefaultTimeout,
                      string clientId = "",
                      string clientSecretKey = ""
                      );
};