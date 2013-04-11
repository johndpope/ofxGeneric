//
//  ofxGenericHeadingManager.cpp
//  ofxGeneric
//
//  Created by Ian on 3/30/13.
//  Copyright (c) 2013 Lumos Labs. All rights reserved.
//

#include "ofxGenericHeadingManager.h"

#include "ofxGenericLocationManager.h"

#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>
#import <CoreLocation/CoreLocation.h>

@interface ofxGenericHeadingManagerForwarder : NSObject< CLLocationManagerDelegate >
{
@private
    ofPtrWeak< ofxGenericHeadingManager > _forwardTo;
}

-( id )initWithForwardTo:( ofPtrWeak< ofxGenericHeadingManager > )forwardTo;
-( void )locationManager:( CLLocationManager* )manager didUpdateHeading:( CLHeading* )newHeading;
-( void )locationManager:( CLLocationManager* )manager didFailWithError:( NSError* )error;

@end

#endif

bool ofxGenericHeadingManager::locationServicesEnabled()
{
    return ofxGenericLocationManager::locationServicesEnabled();
}

bool ofxGenericHeadingManager::locationServicesAuthorized()
{
    return ofxGenericLocationManager::locationServicesAuthorized();
}

ofPtr< ofxGenericHeadingManager > ofxGenericHeadingManager::_this;

ofPtr< ofxGenericHeadingManager > ofxGenericHeadingManager::getInstance()
{
    if ( !ofxGenericHeadingManager::_this )
    {
        ofPtr< ofxGenericHeadingManager > instance = ofPtr< ofxGenericHeadingManager >( new ofxGenericHeadingManager() );
        instance->init( instance );
    }
    return ofxGenericHeadingManager::_this;
}

ofxGenericHeadingManager::ofxGenericHeadingManager()
{
}

void ofxGenericHeadingManager::init( ofPtr< ofxGenericHeadingManager > setThis )
{
    ofxGenericHeadingManager::_this = setThis;
    
#if TARGET_OS_IPHONE
    _nativeManager = [ [ CLLocationManager alloc ] init ];
    _forwarder = [ [ ofxGenericHeadingManagerForwarder alloc ] initWithForwardTo:_this ];
    [ _nativeManager setDelegate:_forwarder ];
#endif
}

ofxGenericHeadingManager::~ofxGenericHeadingManager()
{
}

void ofxGenericHeadingManager::startUpdating()
{
#if TARGET_OS_IPHONE
    [ _nativeManager startUpdatingHeading ];
#else
    throw ofxGenericExceptionMemberNotImplement( "ofxGenericHeadingManager", "startUpdating" )
#endif
}

void ofxGenericHeadingManager::stopUpdating()
{
#if TARGET_OS_IPHONE
    [ _nativeManager stopUpdatingHeading ];
#else
    throw ofxGenericExceptionMemberNotImplement( "ofxGenericHeadingManager", "stopUpdating" )
#endif
}

void ofxGenericHeadingManager::setDelegate( ofPtrWeak< ofxGenericHeadingManagerDelegate > delegate )
{
    _delegate = delegate;
}

void ofxGenericHeadingManager::updateHeading( const float& heading )
{
    _heading = heading;
    
    ofPtr< ofxGenericHeadingManagerDelegate > delegate = _delegate.lock();
    if ( delegate )
    {
        delegate->headingManager_headingUpdated( _heading );
    }
}

void ofxGenericHeadingManager::unableToRetrieveHeading( string error )
{
    ofPtr< ofxGenericHeadingManagerDelegate > delegate = _delegate.lock();
    if ( delegate )
    {
        delegate->headingManager_unableToRetrieveHeading( error );
    }
}

const float& ofxGenericHeadingManager::getHeading()
{
    return _heading;
}

#if TARGET_OS_IPHONE
@implementation ofxGenericHeadingManagerForwarder

-( id )initWithForwardTo:( ofPtrWeak< ofxGenericHeadingManager > )forwardTo
{
    self = [ super init ];
    if ( self )
    {
        _forwardTo = forwardTo;
    }
    return self;
}

-( void )locationManager:( CLLocationManager* )manager didUpdateHeading:( CLHeading* )newHeading
{
    ofPtr< ofxGenericHeadingManager > forwardTo = _forwardTo.lock();
    if ( forwardTo )
    {
        forwardTo->updateHeading( ( float )[ newHeading magneticHeading ] );
    }
}

-( void )locationManager:( CLLocationManager* )manager didFailWithError:( NSError* )error
{
    ofPtr< ofxGenericHeadingManager > forwardTo = _forwardTo.lock();
    if ( forwardTo )
    {
        forwardTo->unableToRetrieveHeading( ofxNSStringToString( [ error localizedDescription ] ) );
    }
}

@end

#endif
