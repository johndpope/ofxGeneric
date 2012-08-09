//
//  ofxGenericOAuthToken.cpp
//  iOS
//
//  Created by Ian Grossberg on 8/1/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericOAuthToken.h"
#include "ofxGenericValueStore.h"

#define AccessTokenCacheKey "access_token"
#define ScopeCacheKey "scope"

ofPtr< ofxGenericOAuthToken > ofxGenericOAuthToken::create( ofPtr< ofxGenericValueStore > from )
{
    if ( from )
    {
        return ofxGenericOAuthToken::create( from->read( AccessTokenCacheKey, "" ), from->read( ScopeCacheKey, "" ) );
    }
    return ofPtr< ofxGenericOAuthToken >();
}

ofPtr< ofxGenericOAuthToken > ofxGenericOAuthToken::create( string accessToken, string scope )
{
    ofPtr< ofxGenericOAuthToken > create( new ofxGenericOAuthToken() );
    create->init( create, accessToken, scope );
    return create;
}

ofxGenericOAuthToken::ofxGenericOAuthToken()
{
}

ofxGenericOAuthToken::~ofxGenericOAuthToken()
{    
}

void ofxGenericOAuthToken::init( ofPtrWeak< ofxGenericOAuthToken > setThis, string accessToken, string scope )
{
    ofxGenericValueStore::init( setThis, ofxGenericValueStoreTypeObject );

    write( AccessTokenCacheKey, accessToken );
    write( ScopeCacheKey, scope );
}

string ofxGenericOAuthToken::getAccessToken()
{
    return read( AccessTokenCacheKey, "" );
}

std::vector< string > ofxGenericOAuthToken::getScope()
{
    return ofxGSplit( read( ScopeCacheKey, "" ), ' ' );
}

bool ofxGenericOAuthToken::hasScope( string checkScope )
{
    std::vector< string > scope = getScope();
    return std::find( scope.begin(), scope.end(), checkScope ) != scope.end();
}