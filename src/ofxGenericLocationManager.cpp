//
//  ofxGenericLocationManager.cpp
//  ofxGeneric
//
//  Created by Ian on 3/30/13.
//  Copyright (c) 2013 Lumos Labs. All rights reserved.
//

#include "ofxGenericLocationManager.h"

#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>
#import <CoreLocation/CoreLocation.h>

@interface ofxGenericLocationManagerForwarder : NSObject< CLLocationManagerDelegate >
{
@private
    ofPtrWeak< ofxGenericLocationManager > _forwardTo;
}

-( id )initWithForwardTo:( ofPtrWeak< ofxGenericLocationManager > )forwardTo;
-( void )locationManager:( CLLocationManager* )manager didUpdateLocations:( NSArray* )locations;
-( void )locationManager:( CLLocationManager* )manager didFailWithError:( NSError* )error;

@end

#endif

bool ofxGenericLocationManager::locationServicesEnabled()
{
#if TARGET_OS_IPHONE
    return ( bool )[ CLLocationManager locationServicesEnabled ];
#else
    throw ofxGenericExceptionMemberNotImplement( "ofxGenericLocationManager", "locationServicesEnabled" );
#endif
}

bool ofxGenericLocationManager::locationServicesAuthorized()
{
#if TARGET_OS_IPHONE
    return [ CLLocationManager authorizationStatus ] == kCLAuthorizationStatusAuthorized;
#else
    throw ofxGenericExceptionMemberNotImplement( "ofxGenericLocationManager", "locationServicesAvailability" );
#endif
}

ofPtr< ofxGenericLocationManager > ofxGenericLocationManager::_this;

ofPtr< ofxGenericLocationManager > ofxGenericLocationManager::getInstance()
{
    if ( !ofxGenericLocationManager::_this )
    {
        ofPtr< ofxGenericLocationManager > instance = ofPtr< ofxGenericLocationManager >( new ofxGenericLocationManager() );
        instance->init( instance );
    }
    return ofxGenericLocationManager::_this;
}

ofxGenericLocationManager::ofxGenericLocationManager()
{
}

void ofxGenericLocationManager::init( ofPtr< ofxGenericLocationManager > setThis )
{
    ofxGenericLocationManager::_this = setThis;
    
#if TARGET_OS_IPHONE
    _nativeManager = [ [ CLLocationManager alloc ] init ];
    [ _nativeManager setDesiredAccuracy:kCLLocationAccuracyBest ];
    [ _nativeManager setDistanceFilter:1.0 ];
    
    _forwarder = [ [ ofxGenericLocationManagerForwarder alloc ] initWithForwardTo:_this ];
    [ _nativeManager setDelegate:_forwarder ];
#endif
}

ofxGenericLocationManager::~ofxGenericLocationManager()
{
}

void ofxGenericLocationManager::startUpdating()
{
#if TARGET_OS_IPHONE
    [ _nativeManager startUpdatingLocation ];
#else
    throw ofxGenericExceptionMemberNotImplement( "ofxGenericLocationManager", "startUpdating" )
#endif
}

void ofxGenericLocationManager::stopUpdating()
{
#if TARGET_OS_IPHONE
    [ _nativeManager stopUpdatingLocation ];
#else
    throw ofxGenericExceptionMemberNotImplement( "ofxGenericLocationManager", "stopUpdating" )
#endif
}

void ofxGenericLocationManager::setDelegate( ofPtrWeak< ofxGenericLocationManagerDelegate > delegate )
{
    _delegate = delegate;
}

void ofxGenericLocationManager::updateCurrentLocation( const ofxGenericCoordinate& location )
{
    _currentLocation = location;
    
    ofPtr< ofxGenericLocationManagerDelegate > delegate = _delegate.lock();
    if ( delegate )
    {
        delegate->locationManager_locationUpdated( _currentLocation );
    }
}

void ofxGenericLocationManager::unableToRetrieveLocation( string error )
{
    ofPtr< ofxGenericLocationManagerDelegate > delegate = _delegate.lock();
    if ( delegate )
    {
        delegate->locationManager_unableToRetrieveLocation( error );
    }
}

const ofxGenericCoordinate& ofxGenericLocationManager::getCurrentLocation()
{
    return _currentLocation;
}

double ofxGenericLocationManager::getSurfaceDistanceFromCurrentLocation( const ofxGenericCoordinate& location )
{
    double result;

#if TARGET_OS_IPHONE
    CLLocation* fromLocation = [ [ [ CLLocation alloc ] initWithLatitude:location.x longitude:location.y ] autorelease ];
    CLLocationDistance distance = [ [ _nativeManager location ] distanceFromLocation:fromLocation ];
    result = ( double )distance;
#else
    throw ofxGenericExceptionMemberNotImplement( "ofxGenericLocationManager", "getSurfaceDistanceFromLocation" );
#endif
    
    return result;
}

#if TARGET_OS_IPHONE
@implementation ofxGenericLocationManagerForwarder

-( id )initWithForwardTo:( ofPtrWeak< ofxGenericLocationManager > )forwardTo
{
    self = [ super init ];
    if ( self )
    {
        _forwardTo = forwardTo;
    }
    return self;
}

-( void )locationManager:( CLLocationManager* )manager didUpdateLocations:( NSArray* )locations
{
    ofPtr< ofxGenericLocationManager > forwardTo = _forwardTo.lock();
    if ( forwardTo )
    {
        if ( [ locations count ] > 0 )
        {
            CLLocation* location = [ locations objectAtIndex:0 ];
            CLLocationCoordinate2D coordinate = [ location coordinate ];
            forwardTo->updateCurrentLocation( ofxGenericCoordinate( coordinate.latitude, coordinate.longitude ) );
        }
    }
}

-( void )locationManager:( CLLocationManager* )manager didFailWithError:( NSError* )error
{
    ofPtr< ofxGenericLocationManager > forwardTo = _forwardTo.lock();
    if ( forwardTo )
    {
        forwardTo->unableToRetrieveLocation( ofxNSStringToString( [ error localizedDescription ] ) );
    }
}

@end

#endif
