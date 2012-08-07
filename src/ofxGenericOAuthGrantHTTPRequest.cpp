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


void ofxGenericOAuthGrantHTTPRequest::addGrantType( ofPtr< ofxGenericValueStore > body, string grantType )
{
    if ( body )
    {
        body->write( "grant_type", grantType );
    }
}

ofPtr< ofxGenericHTTPResponse > ofxGenericOAuthGrantHTTPRequest::createResponse(
                                                                                int statusCode,
                                                                                string MIMEType,
                                                                                string textEncoding,
                                                                                void* body,
                                                                                unsigned int bodyByteLength,
                                                                                string suggestedFileName
                                                                                )
{
    return ofxGenericOAuthGrantHTTPResponse::create( statusCode, MIMEType, textEncoding, body, bodyByteLength, suggestedFileName );
}

//////////////////////////////////////////////////////////////////////

ofPtr< ofxGenericOAuthGrantHTTPResponse > ofxGenericOAuthGrantHTTPResponse::create( int statusCode, string MIMEType, string textEncoding, void* body, unsigned int bodyByteLength, string suggestedFileName )
{
    ofPtr< ofxGenericOAuthGrantHTTPResponse > create( new ofxGenericOAuthGrantHTTPResponse() );
    create->init( create, statusCode, MIMEType, textEncoding, body, bodyByteLength, suggestedFileName );
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
    if ( getParsedBody() )
    {
        return getParsedBody()->read( AccessTokenCacheKey, "" );
    }
    return string();
}

string ofxGenericOAuthGrantHTTPResponse::getScope()
{
    if ( getParsedBody() )
    {
        return getParsedBody()->read( ScopeCacheKey, "" );
    }
    return "";
}