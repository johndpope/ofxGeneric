//
//  ofxGenericOAuthToken.cpp
//  iOS
//
//  Created by Ian Grossberg on 8/1/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericOAuthToken.h"

ofPtr< ofxGenericOAuthToken > ofxGenericOAuthToken::create( string accessToken, string scopes )
{
    ofPtr< ofxGenericOAuthToken > create( new ofxGenericOAuthToken() );
    create->init( create, accessToken, scopes );
    return create;
}

ofxGenericOAuthToken::ofxGenericOAuthToken()
{
}

ofxGenericOAuthToken::~ofxGenericOAuthToken()
{    
}

void ofxGenericOAuthToken::init( ofPtrWeak< ofxGenericOAuthToken > setThis, string accessToken, string scopes )
{
    _this = setThis;

    _accessToken = accessToken;
    _scopes = ofxGSplit( scopes, ' ' );
}

string ofxGenericOAuthToken::getAccessToken()
{
    return _accessToken;
}

const std::vector< string >& ofxGenericOAuthToken::getScopes()
{
    return _scopes;
}

bool ofxGenericOAuthToken::hasScope( string scope )
{
    return std::find( getScopes().begin(), getScopes().end(), scope ) != getScopes().end();
}