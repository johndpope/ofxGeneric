//
//  ofxGenericLocationManager.h
//  ofxGeneric
//
//  Created by Ian on 3/30/13.
//  Copyright (c) 2013 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"

#import <UIKit/UIKit.h>
#import <CoreLocation/CoreLocation.h>

class ofxGenericLocationManagerDelegate;
#if TARGET_OS_IPHONE
@class ofxGenericLocationManagerForwarder;
#endif

class ofxGenericCoordinate
{
public:
    double x, y;
    ofxGenericCoordinate() : x( 0 ), y( 0 ) {};
    ofxGenericCoordinate( double setX, double setY ) : x( setX ), y( setY ) {};
};

class ofxGenericLocationManager
{
public:
    static bool locationServicesEnabled();
    static bool locationServicesAuthorized();
    
    static ofPtr< ofxGenericLocationManager > getInstance();
    
    void startUpdating();
    void stopUpdating();
    
    void setDelegate( ofPtrWeak< ofxGenericLocationManagerDelegate > delegate );
    
    void updateCurrentLocation( const ofxGenericCoordinate& location );
    void unableToRetrieveLocation( string error );
    
    const ofxGenericCoordinate& getCurrentLocation();
    double getSurfaceDistanceFromCurrentLocation( const ofxGenericCoordinate& location );
    
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
    
    ofxGenericCoordinate _currentLocation;
};

class ofxGenericLocationManagerDelegate
{
public:
    virtual ~ofxGenericLocationManagerDelegate() {}
    
    virtual void locationManager_locationUpdated( const ofxGenericCoordinate& location ) {};
    virtual void locationManager_unableToRetrieveLocation( string error ) {};
};