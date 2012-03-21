//
//  ofxGenericHTTPRequest.cpp
//
//  Created by Ian Grossberg on 1/23/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//


// TODO: test reusing same instance multiple times

#include "ofxGenericHTTPRequest.h"
#include "ofxGenericUtility.h"

#if TARGET_OS_IPHONE
@interface NSURLConnectionForwarder : NSObject< NSURLConnectionDelegate >
{
@protected
    ofPtrWeak< ofxGenericHTTPRequest > _delegate;
    NSMutableData* _receivedData;
    NSURLResponse* _response;
}
-( id )initWithDelegate:( ofPtrWeak< ofxGenericHTTPRequest > )delegate;

-( void )connection:( NSURLConnection* )connection didFailWithError:( NSError* )error;
-( void )connection:( NSURLConnection* )connection didReceiveResponse:( NSURLResponse* )response;
-( void )connection:( NSURLConnection* )connection didReceiveData:( NSData* )data;
-( void )connectionDidFinishLoading:( NSURLConnection* )connection;

@end
#endif

ofPtr< ofxGenericHTTPRequest > ofxGenericHTTPRequest::create( string url, string method, string format, void* data, int dataByteLength, float timeout, ofPtr< ofxGenericHTTPRequestDelegate > delegate )
{
    ofPtr< ofxGenericHTTPRequest > create = ofPtr< ofxGenericHTTPRequest >( new ofxGenericHTTPRequest() );
    create->init( create, url, method, format, data, dataByteLength, timeout, delegate );
    return create;
}

ofxGenericHTTPRequest::ofxGenericHTTPRequest()
:
#if TARGET_OS_IPHONE
 _connection( nil ), _forwarder( nil ), _request( nil )
#endif
{
}

void ofxGenericHTTPRequest::init( ofPtrWeak< ofxGenericHTTPRequest > setThis, string url, string method, string format, void* data, int dataByteLength, float timeout, ofPtr< ofxGenericHTTPRequestDelegate > delegate )
{
    _this = setThis;
    
    _delegate = delegate;
    
    string urlWithFormat = url;
    if ( !format.empty() )
    {
        if ( urlWithFormat.find( "?" ) == string::npos )
        {
            urlWithFormat += "?format=" + format;
        } else
        {
            urlWithFormat += "&format=" + format;
        }
    }

    ofxGLog( OF_LOG_VERBOSE, "HTTPRequest - " + urlWithFormat + " " + method );

#if TARGET_OS_IPHONE
    // TODO: allow caching and timeout specification
    _request = [ [ NSMutableURLRequest alloc ] initWithURL:[ NSURL URLWithString:ofxStringToNSString( urlWithFormat ) ] cachePolicy:NSURLRequestReloadIgnoringCacheData timeoutInterval:timeout ];
    [ _request setHTTPShouldHandleCookies:YES ];
    [ _request setHTTPMethod:[ NSString stringWithCString:method.c_str() encoding:NSUTF8StringEncoding ] ];
    
    if ( data && dataByteLength > 0 )
    {
        NSString* dataString = [ [ [ NSString alloc ] initWithBytes:data length:dataByteLength encoding:NSUTF8StringEncoding ] autorelease ];
        NSLog( @"HTTPRequest - \nBody: %@", dataString );
        [ _request setHTTPBody:[ NSData dataWithBytes:data length:dataByteLength ] ];
    }

    _forwarder = [ [ NSURLConnectionForwarder alloc ] initWithDelegate:_this ];
#endif
    
    _format = format;
    std::transform( _format.begin(), _format.end(), _format.begin(), ::tolower );    
    if ( _format == "xml" )
    {
#if TARGET_OS_IPHONE
        [ _request setValue:@"application/xml" forHTTPHeaderField:@"Accept" ];
        [ _request setValue:@"application/xml" forHTTPHeaderField:@"Content-Type" ];        
#endif
    }
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

ofPtr< ofxGenericHTTPResponse > ofxGenericHTTPRequest::createResponse()
{
    return ofPtr< ofxGenericHTTPResponse >( new ofxGenericHTTPResponse() );
}

void ofxGenericHTTPRequest::finishedWithError( string errorDescription, string errorFailureReason, string errorRecoverySuggestion )
{
    _lastResponse = createResponse();
    _lastResponse->init( _lastResponse, errorDescription, errorFailureReason, errorRecoverySuggestion );
    if ( _delegate )
    {
        _delegate->httpRequest_finishedWithError( _this.lock() );
    }
}

void ofxGenericHTTPRequest::finishedSuccessfully( int statusCode, string MIMEType, string textEncoding, void* data, int dataByteLength, string suggestedFilename )
{
    _lastResponse = createResponse();
    _lastResponse->init( _lastResponse, statusCode, MIMEType, textEncoding, data, dataByteLength, suggestedFilename );
    if ( _delegate )
    {
        _delegate->httpRequest_finishedSuccessfully( _this.lock() );
    }
}

#if TARGET_OS_IPHONE
void ofxGenericHTTPRequest::finishedWithError( NSError* error )
{
    finishedWithError( 
                      ofxNSStringToString( [ error localizedDescription ] ), 
                      ofxNSStringToString( [ error localizedFailureReason ] ),
                      ofxNSStringToString( [ error localizedRecoverySuggestion ] )
                      );
    release( _connection );
}

void ofxGenericHTTPRequest::finishedSuccessfully( NSURLResponse* response, NSData* data )
{
    int statusCode;
    if ( [ response isKindOfClass:[ NSHTTPURLResponse class ] ] )
    {
        NSHTTPURLResponse* httpResponse = ( NSHTTPURLResponse* )response;
        statusCode = [ httpResponse statusCode ];
    } else
    {
        statusCode = -1;
    }
    
    finishedSuccessfully(
                         statusCode,
                         ofxNSStringToString( [ response MIMEType ] ),
                         ofxNSStringToString( [ response textEncodingName ] ),
                         ( void* )[ data bytes ],
                         [ data length ],
                         ofxNSStringToString( [ response suggestedFilename ] )
                         );

    if ( getLastResponse() )
    {
        getLastResponse()->retainData( data );
    }
    release( _connection );
}
#endif

ofPtr< ofxGenericHTTPResponse > ofxGenericHTTPRequest::getLastResponse()
{
    return _lastResponse;
}

#if TARGET_OS_IPHONE
@implementation NSURLConnectionForwarder

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
        _delegate.lock()->finishedWithError( error );
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
        _delegate.lock()->finishedSuccessfully( _response, _receivedData );
    }
}

// TODO: FIX PROPIGATING PREDEF VALUE
//#ifdef SSL_PROTOCOL_VERIFICATION_OFF
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
//#endif

@end
#endif
