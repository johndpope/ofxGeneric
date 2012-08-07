//
//  ofxGenericOAuthHTTPRequest.cpp
//  iOS
//
//  Created by Ian Grossberg on 8/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericOAuthHTTPRequest.h"

#include "ofxGenericValueStore.h"

#include "ofxGenericOAuthToken.h"

ofxGenericOAuthHTTPRequest::ofxGenericOAuthHTTPRequest()
{
}

void ofxGenericOAuthHTTPRequest::init( ofPtrWeak< ofxGenericOAuthHTTPRequest > setThis, string url, string method, ofPtr< ofxGenericOAuthToken > token, string body, float timeout, ofPtr< ofxGenericHTTPRequestDelegate > delegate )
{
    ofxGenericHTTPRequest::init( setThis, url, method, "json", body, timeout, delegate );

    if ( token )
    {
        setAuthorizationHeader( "OAuth " + token->getAccessToken() );
    }
}

void ofxGenericOAuthHTTPRequest::init( ofPtrWeak< ofxGenericOAuthHTTPRequest > setThis, string url, string method, ofPtr< ofxGenericOAuthToken > token, ofPtr< ofxGenericValueStore > body, float timeout, ofPtr< ofxGenericHTTPRequestDelegate > delegate )
{
    string JSONBodyString;
    if ( body )
    {
        JSONBodyString = body->toJSONString();
    }
    init( setThis, url, method, token, JSONBodyString, timeout, delegate );
}

ofPtr< ofxGenericValueStore > ofxGenericOAuthHTTPRequest::createBody( string clientId, string clientSecretKey )
{
    ofPtr< ofxGenericValueStore > body = ofxGenericValueStore::create( ofxGenericValueStore::ofxGenericValueStoreTypeObject );
    if ( body )
    {
        body->write( "client_id", clientId );
        body->write( "client_secret", clientSecretKey );
    }
    return body;
}

void ofxGenericOAuthHTTPRequest::setAcceptHeader( string value )
{
    ofxGenericHTTPRequest::setAcceptHeader( value + "; application/vnd.lumoslabs.com; version=v2" );
}
