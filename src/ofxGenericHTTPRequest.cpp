//
//  ofxGenericHTTPRequest.cpp
//
//  Created by Ian Grossberg on 1/23/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//


// TODO: test reusing same instance multiple times

#include "ofxGenericHTTPRequest.h"

#include "ofxGenericHTTPResponse.h"
#include "ofxGenericUtility.h"

#include "ofxGenericValueStore.h"

#if TARGET_OS_IPHONE
@interface ofxGenericHTTPConnectionForwarder : NSObject< NSURLConnectionDelegate >
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

ofPtr< ofxGenericHTTPRequest > ofxGenericHTTPRequest::create( string url, string method, string format, string data, float timeout, ofPtr< ofxGenericHTTPRequestDelegate > delegate )
{
    ofPtr< ofxGenericHTTPRequest > create = ofPtr< ofxGenericHTTPRequest >( new ofxGenericHTTPRequest() );
    create->init( create, url, method, format, data, timeout, delegate );
    return create;
}

ofxGenericHTTPRequest::ofxGenericHTTPRequest()
:
#if TARGET_OS_IPHONE
 _connection( nil ), _forwarder( nil ), _request( nil )
#endif
{
}

void ofxGenericHTTPRequest::init( ofPtrWeak< ofxGenericHTTPRequest > setThis, string url, string method, string format, string data, float timeout, ofPtr< ofxGenericHTTPRequestDelegate > delegate )
{
    ofxGenericHTTPRequest::init( setThis, url, method, format, (void*) data.c_str(), data.length(), timeout, delegate );
}

void ofxGenericHTTPRequest::init( ofPtrWeak< ofxGenericHTTPRequest > setThis, string url, string method, string format, void* data, int dataByteLength, float timeout, ofPtr< ofxGenericHTTPRequestDelegate > delegate )
{
    _this = setThis;
    
    _delegate = delegate;
    
    _format = format;
    std::transform( _format.begin(), _format.end(), _format.begin(), ::tolower );    
    string urlWithFormat = url;
    if ( _format == "xml" )
    {
        urlWithFormat += ".xml";
    } else if ( _format == "json" )
    {
        urlWithFormat += ".json";
    }

#if TARGET_OS_IPHONE
    // TODO: allow caching specification
    
    NSURL* nsUrl = [ NSURL URLWithString:ofxStringToNSString( getWithPercentEscapes( urlWithFormat ) ) ];
    _request = [ [ NSMutableURLRequest alloc ] initWithURL:nsUrl cachePolicy:NSURLRequestReloadIgnoringCacheData timeoutInterval:timeout ];
    [ _request setHTTPShouldHandleCookies:YES ];
    _forwarder = [ [ ofxGenericHTTPConnectionForwarder alloc ] initWithDelegate:_this ];
#endif
    
    setMethod( method );
    setBody( data, dataByteLength );
    setTimeout( timeout );

    if ( _format == "xml" )
    {
        setContentTypeHeader( "application/xml" );
        setAcceptHeader( "application/xml" );
    } else if ( _format == "json" )
    {
        setContentTypeHeader( "application/json" );
        setAcceptHeader( "application/json" );
    }
}

ofxGenericHTTPRequest::~ofxGenericHTTPRequest()
{
    cancel();
#if TARGET_OS_IPHONE
    [ _forwarder release ];
    _forwarder = nil;
    [ _request release ];
    _request = nil;
#endif
}

void ofxGenericHTTPRequest::setDelegate( ofPtr< ofxGenericHTTPRequestDelegate > delegate )
{
    _delegate = delegate;
}

void ofxGenericHTTPRequest::setMethod( string method )
{
#if TARGET_OS_IPHONE
    [ _request setHTTPMethod:ofxStringToNSString( method ) ];
#endif
}

void ofxGenericHTTPRequest::setBody( string body )
{
    setBody( ( void* )body.c_str(), body.length() );
}

void ofxGenericHTTPRequest::setBody( void* body, unsigned int bodyByteLength )
{
    if ( body && bodyByteLength > 0 )
    {
#if DEBUG
        if ( ofGetLogLevel() == OF_LOG_VERBOSE )
        {
            char* dumpBody = new char[ bodyByteLength + 1 ];
            memcpy( ( void* )dumpBody, body, bodyByteLength );
            dumpBody[ bodyByteLength ] = '\0';
        }
#endif
        
#if TARGET_OS_IPHONE
        [ _request setHTTPBody:[ NSData dataWithBytes:body length:bodyByteLength ] ];
#endif
    }
}

void ofxGenericHTTPRequest::setHeaderField( string field, string value )
{
#if TARGET_OS_IPHONE
    [ _request setValue:ofxStringToNSString( value ) forHTTPHeaderField:ofxStringToNSString( field ) ];
#endif
}

void ofxGenericHTTPRequest::setTimeout( float timeout )
{
#if TARGET_OS_IPHONE
    [ _request setTimeoutInterval:( NSTimeInterval )timeout ];
#endif
}

void ofxGenericHTTPRequest::setContentTypeHeader( string value )
{
    setHeaderField( "Content-Type", value );
}

void ofxGenericHTTPRequest::setAcceptHeader( string value )
{
    setHeaderField( "Accept", value );
}

void ofxGenericHTTPRequest::setAuthorizationHeader( string value )
{
    setHeaderField( "Authorization", value );
}

void ofxGenericHTTPRequest::start()
{
#if TARGET_OS_IPHONE
    if ( !_connection )
    {
        _connection = [ [ NSURLConnection alloc ] initWithRequest:_request delegate:_forwarder startImmediately:YES ];
    }
#endif
    ofxGLogVerbose( toString() );
}

void ofxGenericHTTPRequest::cancel()
{
#if TARGET_OS_IPHONE
    [ _connection cancel ];
#endif
    stop();
}

void ofxGenericHTTPRequest::stop()
{
#if TARGET_OS_IPHONE
    [ _connection release ];
    _connection = nil;
#endif
}

string ofxGenericHTTPRequest::getUrl() const
{
#if TARGET_OS_IPHONE
    return ofxNSStringToString( [ [ _request URL ] absoluteString ] );
#endif
}

string ofxGenericHTTPRequest::getMethod() const
{
#if TARGET_OS_IPHONE
    return ofxNSStringToString( [ _request HTTPMethod ] );
#endif
}

float ofxGenericHTTPRequest::getTimeout() const
{
#if TARGET_OS_IPHONE
    return ( float )[ _request timeoutInterval ];
#endif
}

string ofxGenericHTTPRequest::getHeaders() const
{
#if TARGET_OS_IPHONE
    NSDictionary* headers = [ _request allHTTPHeaderFields ];
    NSArray* names = [ headers allKeys ];

    string result;
    for( NSString* name in names )
    {
        NSString* value = [ headers objectForKey:name ];
        result += "\n" + ofxNSStringToString( name ) + ": " + ofxNSStringToString( value );
    }
    return result;
#endif
}

string ofxGenericHTTPRequest::getBodyAsString() const
{
#if TARGET_OS_IPHONE
    return ofxGToString( [ _request HTTPBody ] );
#endif
}

string ofxGenericHTTPRequest::toString() const
{
    string result = "HTTPRequest -";
    result += " Url: " + getUrl();
    result += "\nMethod: " + getMethod();
    result += "\nTimeout: " + ofxGToString( getTimeout() );
    result += "\nHeaders:\n" + getHeaders();
    result += "\nBody:\n" + getBodyAsString();
    return result;
}

ofPtr< ofxGenericHTTPResponse > ofxGenericHTTPRequest::createResponse( int statusCode, string MIMEType, string textEncoding, void* data, int dataByteLength, string suggestedFileName )
{
    return ofxGenericHTTPResponse::create( statusCode, MIMEType, textEncoding, data, dataByteLength, suggestedFileName );
}

void ofxGenericHTTPRequest::finished( int statusCode, string MIMEType, string textEncoding, void* data, int dataByteLength, string suggestedFileName )
{
    _lastResponse = createResponse( statusCode, MIMEType, textEncoding, data, dataByteLength, suggestedFileName );
    
    stop();
    if ( _delegate )
    {
        if ( isLastResponseOk() )
        {
            _delegate->httpRequest_finishedSuccessfully( _this.lock() );
        } else
        {
            _delegate->httpRequest_finishedWithError( _this.lock() );
        }
    }
}

#if TARGET_OS_IPHONE
void ofxGenericHTTPRequest::finishedWithError( NSError* error )
{
    string JSONBody;
    
    ofPtr< ofxGenericValueStore > body = ofxGenericHTTPResponse::createBody( ofxNSStringToString( [ error localizedDescription ] ), ofxNSStringToString( [ error localizedFailureReason ] ), ofxNSStringToString( [ error localizedRecoverySuggestion ] ) );
    if ( body )
    {
        JSONBody = body->toJSONString();
    }

    finished(
             400,
             MIMEType_json,
             "utf-8",
             ( void* )JSONBody.c_str(),
             JSONBody.size()
             );
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
        statusCode = 200;
    }
    
    finished(
             statusCode,
             ofxNSStringToString( [ response MIMEType ] ),
             ofxNSStringToString( [ response textEncodingName ] ),
             ( void* )[ data bytes ],
             [ data length ],
             ofxNSStringToString( [ response suggestedFilename ] )
            );
}
#endif

ofPtr< ofxGenericHTTPResponse > ofxGenericHTTPRequest::getLastResponse()
{
    return _lastResponse;
}

bool ofxGenericHTTPRequest::isLastResponseOk()
{
    ofPtr< ofxGenericHTTPResponse > response = getLastResponse();
    return response && response->isOk();
}


string ofxGenericHTTPRequest::getWithPercentEscapes( string unencoded )
{
#if TARGET_OS_IPHONE
    NSString* encoded = [ ofxStringToNSString( unencoded ) stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding ];
    return ofxNSStringToString( encoded );
#endif
}

string ofxGenericHTTPRequest::getFromPercentEscapes( string encoded )
{
#if TARGET_OS_IPHONE
    NSString* unencoded = [ ofxStringToNSString( encoded ) stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding ];
    return ofxNSStringToString( unencoded );
#endif
}

#if TARGET_OS_IPHONE
@implementation ofxGenericHTTPConnectionForwarder

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
    [ _receivedData release ];
    _receivedData = nil;
    [ _response release ];
    _response = nil;
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