//
//  ofxGenericOAuthGrantHTTPRequest.cpp
//  iOS
//
//  Created by Ian Grossberg on 8/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericOAuthGrantHTTPRequest.h"

#include "ofxGenericValueStore.h"
#include "ofxGenericOAuthToken.h"

#define AccessTokenCacheKey "access_token"
#define ScopeCacheKey "scope"

ofPtr< ofxGenericValueStore > ofxGenericOAuthGrantHTTPRequest::createBody( string clientId, string clientSecretKey, string grantType )
{
    ofPtr< ofxGenericValueStore > body = ofxGenericOAuthHTTPRequest::createBody( clientId, clientSecretKey );
    if ( body )
    {
        body->write( "grant_type", grantType );
    }
    return body;
}

ofPtr< ofxGenericHTTPResponse > ofxGenericOAuthGrantHTTPRequest::createResponse( int statusCode, string MIMEType, string textEncoding, void* data, int dataByteLength, string suggestedFileName )
{
    return ofxGenericOAuthGrantHTTPResponse::create( statusCode, MIMEType, textEncoding, data, dataByteLength, suggestedFileName );
}

//////////////////////////////////////////////////////////////////////

ofPtr< ofxGenericOAuthGrantHTTPResponse > ofxGenericOAuthGrantHTTPResponse::create( int statusCode, string MIMEType, string textEncoding, void* data, unsigned int dataByteLength, string suggestedFileName )
{
    ofPtr< ofxGenericOAuthGrantHTTPResponse > create( new ofxGenericOAuthGrantHTTPResponse() );
    create->init( create, statusCode, MIMEType, textEncoding, data, dataByteLength, suggestedFileName );
    return create;
}

ofxGenericOAuthGrantHTTPResponse::ofxGenericOAuthGrantHTTPResponse()
{
}

ofPtr< ofxGenericOAuthToken > ofxGenericOAuthGrantHTTPResponse::getToken()
{
    return ofxGenericOAuthToken::create( getAccessToken(), getScope() );
}

string ofxGenericOAuthGrantHTTPResponse::getAccessToken()
{
    if ( getParsedData() )
    {
        return getParsedData()->read( AccessTokenCacheKey, "" );
    }
    return string();
}

string ofxGenericOAuthGrantHTTPResponse::getScope()
{
    if ( getParsedData() )
    {
        return getParsedData()->read( ScopeCacheKey, "" );
    }
    return "";
}