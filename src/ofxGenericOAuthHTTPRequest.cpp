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

void ofxGenericOAuthHTTPRequest::init(
                                      ofPtrWeak< ofxGenericOAuthHTTPRequest > setThis,
                                      string url,
                                      string method,
                                      ofPtr< ofxGenericOAuthToken > token,
                                      string format,
                                      ofPtr< ofxGenericValueStore > body,
                                      ofPtr< ofxGenericHTTPRequestDelegate > delegate,
                                      float timeout,
                                      string clientId,
                                      string clientSecretKey
                                      )
{
    if ( !body && ( !clientId.empty() || !clientSecretKey.empty() ) )
    {
        body = ofxGenericValueStore::create( false );
    }

    ofxGenericOAuthHTTPRequest::addClientInfo( body, clientId, clientSecretKey );
    
    ofxGenericHTTPRequest::init( setThis, url, method, format, body, delegate, timeout );

    if ( token )
    {
        setAuthorizationHeader( "OAuth " + token->getAccessToken() );
    }    
}

void ofxGenericOAuthHTTPRequest::addClientInfo( ofPtr< ofxGenericValueStore > body, string clientId, string clientSecretKey )
{
    if ( body )
    {
        body->write( "client_id", clientId, true );
        body->write( "client_secret", clientSecretKey, true );
    }
}