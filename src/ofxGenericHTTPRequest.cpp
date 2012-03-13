//
//  ofxGenericHTTPRequest.cpp
//
//  Created by Ian Grossberg on 1/23/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//


// TODO: test reusing same instance multiple times

#include "ofxGenericHTTPRequest.h"
#include "ofxGenericUtility.h"

ofPtr< ofxGenericHTTPRequest > ofxGenericHTTPRequest::create( string url, string method, void* data, int dataByteLength, ofPtr< ofxGenericHTTPRequestDelegate > delegate )
{
    ofPtr< ofxGenericHTTPRequest > create = ofPtr< ofxGenericHTTPRequest >( new ofxGenericHTTPRequest() );
    create->init( create, url, method, data, dataByteLength, delegate );
    return create;
}

ofxGenericHTTPRequest::ofxGenericHTTPRequest()
:
#if TARGET_OS_IPHONE
 _connection( nil ), _forwarder( nil ), _request( nil )
#endif
{
}

void ofxGenericHTTPRequest::init( ofPtrWeak< ofxGenericHTTPRequest > setThis, string url, string method, void* data, int dataByteLength, ofPtr< ofxGenericHTTPRequestDelegate > delegate )
{
    _this = setThis;
    
    _delegate = delegate;
    
#if DEBUG
    ofxGLog( OF_LOG_VERBOSE, "HTTPRequest: " + url + method );
#endif
    
#if TARGET_OS_IPHONE
    // TODO: allow caching and timeout specification
    _request = [ [ NSMutableURLRequest alloc ] initWithURL:[ NSURL URLWithString:ofxStringToNSString( url ) ] cachePolicy:NSURLRequestReloadIgnoringCacheData timeoutInterval:timeout ];
    [ _request setHTTPShouldHandleCookies:YES ];
    [ _request setHTTPMethod:[ NSString stringWithCString:method.c_str() encoding:NSUTF8StringEncoding ] ];
    
    if ( data && dataByteLength > 0 )
    {
#if DEBUG
        NSString* dataString = [ [ [ NSString alloc ] initWithBytes:data length:dataByteLength encoding:NSUTF8StringEncoding ] autorelease ];
        NSLog( @"HTTPRequest - body: %@", dataString );
#endif
        [ _request setHTTPBody:[ NSData dataWithBytes:data length:dataByteLength ] ];
    }

    _forwarder = [ [ NSURLConnectionDelegateForwarder alloc ] initWithDelegate:_this ];
#endif
}

ofxGenericHTTPRequest::~ofxGenericHTTPRequest()
{
    cancel();
#if TARGET_OS_IPHONE
    release( _forwarder );
    release( _request );
#endif
}

void ofxGenericHTTPRequest::start()
{
#if TARGET_OS_IPHONE
    if ( !_connection )
    {
        _connection = [ [ NSURLConnection alloc ] initWithRequest:_request delegate:_forwarder startImmediately:YES ];
    }
#endif
}

void ofxGenericHTTPRequest::cancel()
{
#if TARGET_OS_IPHONE
    [ _connection cancel ];
    release( _connection );
#endif
}

void ofxGenericHTTPRequest::finishedWithError( ofPtr< ofxGenericHTTPResponse > response )
{
    if ( _delegate )
    {
        _delegate->httpRequest_finishedWithError( _this.lock(), response );
    }
    release( _connection );
}

void ofxGenericHTTPRequest::finishedSuccessfully( ofPtr< ofxGenericHTTPResponse > response )
{
    if ( _delegate )
    {
        _delegate->httpRequest_finishedSuccessfully( _this.lock(), response );
    }
    release( _connection );
}

#if TARGET_OS_IPHONE
@implementation NSURLConnectionDelegateForwarder

-( id )initWithDelegate:( ofPtrWeak< ofxGenericHTTPRequest > )delegate
{
    self = [ super init ];
    if ( self )
    {
        _delegate = delegate;
        _receivedData = [ [ NSMutableData alloc ] init ];
        _response = nil;
    }
    return self;
}

-( void )dealloc
{
    release( _receivedData );
    release( _response );
    [ super dealloc ];
}

-( void )connection:( NSURLConnection* )connection didFailWithError:( NSError* )error
{
    if ( _delegate )
    {
        ofPtr< ofxGenericHTTPResponse > response = ofxGenericHTTPResponse::create( error );
        _delegate.lock()->finishedWithError( response );
    }
}

-( void )connection:( NSURLConnection* )connection didReceiveResponse:( NSURLResponse* )response
{
    _response = [ response retain ];
}

-( void )connection:( NSURLConnection* )connection didReceiveData:( NSData* )data
{
    [ _receivedData appendData:data ];
    NSString* newStr = [ [ [ NSString alloc] initWithData:data encoding:NSUTF8StringEncoding] autorelease ];
    NSLog( @"%@", newStr );
}

-( void )connectionDidFinishLoading:( NSURLConnection* )connection
{
    if ( _delegate )
    {
        ofPtr< ofxGenericHTTPResponse > response = ofxGenericHTTPResponse::create( _response, _receivedData );
        _delegate.lock()->finishedSuccessfully( response );
    }
}

#ifdef SSL_PROTOCOL_VERIFICATION_OFF
-( BOOL )connection:( NSURLConnection* )connection canAuthenticateAgainstProtectionSpace:( NSURLProtectionSpace * )protectionSpace
{
    return [ protectionSpace.authenticationMethod isEqualToString:NSURLAuthenticationMethodServerTrust ];
}

-( void )connection:( NSURLConnection*)connection didReceiveAuthenticationChallenge:( NSURLAuthenticationChallenge* )challenge
{
    if ( [ challenge.protectionSpace.authenticationMethod isEqualToString:NSURLAuthenticationMethodServerTrust ] )
    {
//        if ( [ trustedHosts containsObject:challenge.protectionSpace.host])
        {
            [ challenge.sender useCredential:[ NSURLCredential credentialForTrust:challenge.protectionSpace.serverTrust ] forAuthenticationChallenge:challenge ];
        }
    }
    [ challenge.sender continueWithoutCredentialForAuthenticationChallenge:challenge ];
}
#endif

@end
#endif
