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
    virtual void setAcceptHeader( string value );
    
protected:
    ofxGenericOAuthHTTPRequest();
    
    virtual void init( ofPtrWeak< ofxGenericOAuthHTTPRequest > setThis, string url, string method, ofPtr< ofxGenericOAuthToken > token, ofPtr< ofxGenericValueStore > body, float timeout = 20.0f, ofPtr< ofxGenericHTTPRequestDelegate > delegate = ofPtr< ofxGenericHTTPRequestDelegate >() );
    
    virtual void init( ofPtrWeak< ofxGenericOAuthHTTPRequest > setThis, string url, string method, ofPtr< ofxGenericOAuthToken > token, string body, float timeout = 20.0f, ofPtr< ofxGenericHTTPRequestDelegate > delegate = ofPtr< ofxGenericHTTPRequestDelegate >() );

    static ofPtr< ofxGenericValueStore > createBody( string clientId, string clientSecretKey );
};