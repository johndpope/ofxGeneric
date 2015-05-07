//
//  ofxGenericHeadingManager.h
//  ofxGeneric
//
//  Created by Ian on 3/30/13.
//  Copyright (c) 2013 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"

class ofxGenericHeadingManagerDelegate;
#if TARGET_OS_IPHONE
@class ofxGenericHeadingManagerForwarder;
@class CLLocationManager;
#endif

class ofxGenericHeadingManager
{
public:
    static bool locationServicesEnabled();
    static bool locationServicesAuthorized();
    
    static ofPtr< ofxGenericHeadingManager > getInstance();
    
    void startUpdating();
    void stopUpdating();
    
    void setDelegate( ofPtrWeak< ofxGenericHeadingManagerDelegate > delegate );
    
    void updateHeading( const float& heading );
    void unableToRetrieveHeading( string error );
    
    const float& getHeading();
    
    virtual ~ofxGenericHeadingManager();
protected:
    
    ofxGenericHeadingManager();
    static ofPtr< ofxGenericHeadingManager > _this;
    virtual void init( ofPtr< ofxGenericHeadingManager > setThis );
    
#if TARGET_OS_IPHONE
    CLLocationManager* _nativeManager;
    ofxGenericHeadingManagerForwarder* _forwarder;
#endif
    ofPtrWeak< ofxGenericHeadingManagerDelegate > _delegate;
    
    float _heading;
};

class ofxGenericHeadingManagerDelegate
{
public:
    virtual ~ofxGenericHeadingManagerDelegate() {}
    
    virtual void headingManager_headingUpdated( const float& heading ) {};
    virtual void headingManager_unableToRetrieveHeading( string error ) {};
};