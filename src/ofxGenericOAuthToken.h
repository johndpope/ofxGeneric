//
//  ofxGenericOAuthToken.h
//  iOS
//
//  Created by Ian Grossberg on 8/1/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"

class ofxGenericOAuthToken
{
public:
    static ofPtr< ofxGenericOAuthToken > create( string accessToken, string scopes );
    
    string getAccessToken();
    const std::vector< string >& getScopes();
    bool hasScope( string scope );
    
    virtual ~ofxGenericOAuthToken();
protected:
    ofxGenericOAuthToken();
    virtual void init( ofPtrWeak< ofxGenericOAuthToken > setThis, string accessToken, string scopes );
    ofPtrWeak< ofxGenericOAuthToken > _this;
    
    string _accessToken;
    std::vector< string > _scopes;
};