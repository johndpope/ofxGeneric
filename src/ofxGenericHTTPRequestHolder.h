//
//  ofxGenericHTTPRequestHolder.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 8/5/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericHTTPRequest.h"

class ofxGenericHTTPRequestHolder : public ofxGenericHTTPRequestDelegate
{
public:
    static ofPtr< ofxGenericHTTPRequestHolder > createInstance();
    static ofPtr< ofxGenericHTTPRequestHolder > getInstance();
    
    void holdRequestUntilComplete( ofPtr< ofxGenericHTTPRequest > request );
    
    void httpRequest_finishedWithError( ofPtr< ofxGenericHTTPRequest > request );
    void httpRequest_finishedSuccessfully( ofPtr< ofxGenericHTTPRequest > request );
    
    virtual ~ofxGenericHTTPRequestHolder();
    
protected:
    ofxGenericHTTPRequestHolder();
    static ofPtr< ofxGenericHTTPRequestHolder > _instance;
    void setofxGenericHTTPRequestHolderInstanceToThis();
    
    std::list< ofPtr< ofxGenericHTTPRequest > > _holdRequestUntilComplete;
    void removeHeldRequest( ofPtr< ofxGenericHTTPRequest > request );
};
