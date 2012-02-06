//
//  ofxGenericHTTPRequest.mm
//  iOS
//
//  Created by Ian Grossberg on 1/23/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#import "ofxGenericHTTPRequest.h"
#import "ofxGenericUtility.h"
#import "Settings.h"

ofxGenericHTTPRequest::ofxGenericHTTPRequest( string url, string method, void* data, int dataByteLength, ofPtr< ofxGenericHTTPRequestDelegate > delegate )
: _delegate( delegate )
, _connection( nil ), _forwarder( nil )
{
    // TODO: allow caching and timeout specification
    NSURLRequest* request = [ NSURLRequest requestWithURL:[ NSURL URLWithString:ofxStringToNSString( url ) ] cachePolicy:NSURLRequestReloadIgnoringCacheData timeoutInterval:20.0f ];

    _forwarder = [ [ NSURLConnectionDelegateForwarder alloc ] initWithDelegate:this ];
    _connection = [ [ NSURLConnection alloc ] initWithRequest:request delegate:_forwarder startImmediately:YES ];
}

ofxGenericHTTPRequest::~ofxGenericHTTPRequest()
{
    cancel();
    release( _forwarder );
}

void ofxGenericHTTPRequest::cancel()
{
    [ _connection cancel ];
    release( _connection );
}

void ofxGenericHTTPRequest::finishedWithError( ofPtr< ofxGenericHTTPResponse > response )
{
    if ( _delegate )
    {
        _delegate->finishedWithError( this, response );
    }
}

void ofxGenericHTTPRequest::finishedSuccessfully( ofPtr< ofxGenericHTTPResponse > response )
{
    if ( _delegate )
    {
        _delegate->finishedSuccessfully( this, response );
    }
}

@implementation NSURLConnectionDelegateForwarder

-( id )initWithDelegate:( ofxGenericHTTPRequest* )delegate
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
    _delegate = NULL;
    release( _receivedData );
    release( _response );
    [ super dealloc ];
}

-( void )connection:( NSURLConnection* )connection didFailWithError:( NSError* )error
{
    if ( _delegate )
    {
        ofPtr< ofxGenericHTTPResponse > response( new ofxGenericHTTPResponse( error ) );
        _delegate->finishedWithError( response );
    }
}

-( void )connection:( NSURLConnection* )connection didReceiveResponse:( NSURLResponse* )response
{
    _response = [ response retain ];
}

-( void )connection:( NSURLConnection* )connection didReceiveData:( NSData* )data
{
    [ _receivedData appendData:data ];
}

-( void )connectionDidFinishLoading:( NSURLConnection* )connection
{
    if ( _delegate )
    {
        ofPtr< ofxGenericHTTPResponse > response( new ofxGenericHTTPResponse( _response, _receivedData ) );
        _delegate->finishedSuccessfully( response );
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