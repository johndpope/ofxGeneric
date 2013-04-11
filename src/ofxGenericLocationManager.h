//
//  ofxGenericLocationManager.h
//  ofxGeneric
//
//  Created by Ian on 3/30/13.
//  Copyright (c) 2013 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"

class ofxGenericLocationManagerDelegate;
#if TARGET_OS_IPHONE
@class ofxGenericLocationManagerForwarder;
#endif

class ofxGenericLocationManager
{
public:
    static bool locationServicesEnabled();
    static bool locationServicesAuthorized();
    
    static ofPtr< ofxGenericLocationManager > getInstance();
    
    void startUpdating();
    void stopUpdating();
    
    void setDelegate( ofPtrWeak< ofxGenericLocationManagerDelegate > delegate );
    
    void updateLocation( const ofPoint& coordinate );
    void unableToRetrieveLocation( string error );
    
    const ofPoint& getLocation();
    double getSurfaceDistanceFromLocation( const ofPoint& location );
    
    virtual ~ofxGenericLocationManager();
protected:
    
    ofxGenericLocationManager();
    static ofPtr< ofxGenericLocationManager > _this;
    virtual void init( ofPtr< ofxGenericLocationManager > setThis );
    
#if TARGET_OS_IPHONE
    CLLocationManager* _nativeManager;
    ofxGenericLocationManagerForwarder* _forwarder;
#endif
    ofPtrWeak< ofxGenericLocationManagerDelegate > _delegate;
    
    ofPoint _coordinate;
};

class ofxGenericLocationManagerDelegate
{
public:
    virtual ~ofxGenericLocationManagerDelegate() {}
    
    virtual void locationManager_locationUpdated( const ofPoint& coordinate ) {};
    virtual void locationManager_unableToRetrieveLocation( string error ) {};
};