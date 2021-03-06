//
//  ofxGenericOAuthHTTPRequest.h
//  iOS
//
//  Created by Ian Grossberg on 8/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericHTTPRequest.h"

class ofxGenericValueStore;
class ofxGenericOAuthToken;

class ofxGenericOAuthHTTPRequest : public ofxGenericHTTPRequest
{
public:
    static ofPtr< ofxGenericOAuthHTTPRequest > create(
                                                      string url,
                                                      string method,
                                                      ofPtr< ofxGenericOAuthToken > token,
                                                      ofxGenericMIMEType format,
                                                      ofPtr< ofxGenericValueStore > body = ofPtr< ofxGenericValueStore >(),
                                                      ofPtr< ofxGenericHTTPRequestDelegate > delegate = ofPtr< ofxGenericHTTPRequestDelegate >(),
                                                      float timeout = ofxGenericHTTPRequestDefaultTimeout,
                                                      string clientId = "",
                                                      string clientSecretKey = ""
                                                      );
    
    static void addClientInfo( ofPtr< ofxGenericValueStore > body, string clientId, string clientSecretKey );
    
protected:
    ofxGenericOAuthHTTPRequest();
    
    virtual void init(
                      ofPtrWeak< ofxGenericOAuthHTTPRequest > setThis,
                      string url,
                      string method,
                      ofPtr< ofxGenericOAuthToken > token,
                      ofxGenericMIMEType format,
                      ofPtr< ofxGenericValueStore > body,
                      ofPtr< ofxGenericHTTPRequestDelegate > delegate,
                      float timeout,
                      string clientId = "",
                      string clientSecretKey = ""
                      );
};