//
//  ofxGenericOAuthToken.h
//  iOS
//
//  Created by Ian Grossberg on 8/1/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericValueStore.h"

class ofxGenericOAuthToken : public ofxGenericValueStore
{
public:
    static ofPtr< ofxGenericOAuthToken > create( ofPtr< ofxGenericValueStore > from );
    static ofPtr< ofxGenericOAuthToken > create( string accessToken, string scope );
    
    string getAccessToken();
    std::vector< string > getScope();
    bool hasScope( string scope );
    
    virtual ~ofxGenericOAuthToken();
protected:
    ofxGenericOAuthToken();
    virtual void init( ofPtrWeak< ofxGenericOAuthToken > setThis, string accessToken, string scope );
};