//
//  ofxGenericTimer.cpp
//  ofxGeneric
//
//  Created by Ian Grossberg on 5/8/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericTimer.h"

#if TARGET_OS_IPHONE

@interface ofxGenericTimerForwarder : NSObject 
{
@private
    ofPtrWeak< ofxGenericTimer > _forwardTo;
}
-( id )initWithForwardTo:( ofPtrWeak< ofxGenericTimer > ) forwardTo;
-( void )fire;

@end

#endif

ofPtr< ofxGenericTimer > ofxGenericTimer::create( float timeInterval, bool repeats, ofPtrWeak< ofxGenericTimerDelegate > delegate )
{
    ofPtr< ofxGenericTimer > create( new ofxGenericTimer() );
    create->init( create, timeInterval, repeats, delegate );
    return create;
}

ofxGenericTimer::~ofxGenericTimer()
{
#if TARGET_OS_IPHONE
    [ _forwarder release ];
    [ _timer release ];
#endif
}

void ofxGenericTimer::init( ofPtrWeak< ofxGenericTimer > setThis, float timeInterval, bool repeats, ofPtrWeak< ofxGenericTimerDelegate > delegate )
{
    _this = setThis;
    _repeats = repeats;
    _delegate = delegate;
    
#if TARGET_OS_IPHONE
    _forwarder = [ [ ofxGenericTimerForwarder alloc ] initWithForwardTo:_this ];
    _timer = [ [ NSTimer scheduledTimerWithTimeInterval:( NSTimeInterval )timeInterval target:_forwarder selector:@selector( fire ) userInfo:nil repeats:( BOOL )repeats ] retain ];
#endif
}

void ofxGenericTimer::fire()
{
    if ( _delegate )
    {
        _delegate.lock()->timer_fired( _this.lock() );
    }
}

void ofxGenericTimer::stop()
{
#if TARGET_OS_IPHONE
    if ( _timer )
    {
        [ _timer invalidate ];
    }
#endif
}

bool ofxGenericTimer::isRunning()
{
#if TARGET_OS_IPHONE
    if ( _timer )
    {
        return ( [ _timer isValid ] ? true : false );
    }
#endif
    return false;
}

#if TARGET_OS_IPHONE
              
@implementation ofxGenericTimerForwarder

-( id )initWithForwardTo:( ofPtrWeak< ofxGenericTimer > ) forwardTo;
{
    self = [ super init ];
    if ( self )
    {
        _forwardTo = forwardTo;
    }
    return self;
}

-( void )fire
{
    if ( _forwardTo )
    {
        _forwardTo.lock()->fire();
    }
}

@end

#endif