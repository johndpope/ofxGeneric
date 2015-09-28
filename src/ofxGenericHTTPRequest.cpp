//
//  ofxGenericHTTPRequest.cpp
//
//  Created by Ian Grossberg on 1/23/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//


// TODO: test reusing same instance multiple times

#include "ofxGenericHTTPRequest.h"
#include "ofxGenericUtility.h"
#include "ofxGenericValueStore.h"
#include "ofxGenericException.h"

#import "ofCommon.h"

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

ofPtr< ofxGenericHTTPRequest > ofxGenericHTTPRequest::create(
                                                             string url,
                                                             string method,
                                                             ofxGenericMIMEType format,
                                                             string body,
                                                             ofPtr< ofxGenericHTTPRequestDelegate > delegate,
                                                             float timeout
                                                             )
{
    ofPtr< ofxGenericHTTPRequest > create = ofPtr< ofxGenericHTTPRequest >( new ofxGenericHTTPRequest() );
    create->init( create, url, method, format, body, delegate, timeout );
    return create;
}

ofPtr< ofxGenericHTTPRequest > ofxGenericHTTPRequest::create(
                                                             string url,
                                                             string method,
                                                             ofxGenericMIMEType format,
                                                             void* body,
                                                             unsigned int bodyByteLength,
                                                             ofPtr< ofxGenericHTTPRequestDelegate > delegate,
                                                             float timeout
                                                             )
{
    ofPtr< ofxGenericHTTPRequest > create = ofPtr< ofxGenericHTTPRequest >( new ofxGenericHTTPRequest() );
    create->init(
                 create,
                 url,
                 method,
                 format,
                 body,
                 bodyByteLength,
                 delegate,
                 timeout
                 );
    return create;
}

ofxGenericHTTPRequest::ofxGenericHTTPRequest()
:
#if TARGET_OS_IPHONE
 _connection( nil ), _forwarder( nil ), _request( nil ),
#endif
 _responseStatusCode( -1 ), _responseBody( NULL ), _responseBodyByteLength( 0 ),
 _dumpBodyOnError( true )
{
}

void ofxGenericHTTPRequest::init(
                                 ofPtrWeak< ofxGenericHTTPRequest > setThis,
                                 string url,
                                 string method,
                                 ofxGenericMIMEType format,
                                 ofPtr< ofxGenericValueStore > body,
                                 ofPtrWeak< ofxGenericHTTPRequestDelegate > delegate,
                                 float timeout
                                 )
{
    string bodyAsString;
    
    if ( body )
    {
        bodyAsString = body->asString( format );
    }
    
    ofxGenericHTTPRequest::init(
                                setThis,
                                url,
                                method,
                                format,
                                bodyAsString,
                                delegate,
                                timeout
                                );
}

void ofxGenericHTTPRequest::init(
                                 ofPtrWeak< ofxGenericHTTPRequest > setThis,
                                 string url,
                                 string method,
                                 ofxGenericMIMEType format,
                                 string body,
                                 ofPtrWeak< ofxGenericHTTPRequestDelegate > delegate,
                                 float timeout
                                 )
{
    ofxGenericHTTPRequest::init(
                                setThis,
                                url,
                                method,
                                format,
                                ( void* )body.c_str(),
                                (unsigned int)body.length(),
                                delegate,
                                timeout
                                );
}

void ofxGenericHTTPRequest::init(
                                 ofPtrWeak< ofxGenericHTTPRequest > setThis,
                                 string url,
                                 string method,
                                 ofxGenericMIMEType format,
                                 void* body,
                                 unsigned int bodyByteLength,
                                 ofPtrWeak< ofxGenericHTTPRequestDelegate > delegate,
                                 float timeout
                                 )
{
    _this = setThis;
    
    _delegate = delegate;
    
    _format = format;
    string urlWithFormat = url;
    //urlWithFormat += "." + ofxGMIMETypeToExtension( _format );

#if TARGET_OS_IPHONE
    // TODO: allow caching specification
    
    NSURL* nsUrl = [ NSURL URLWithString:ofxStringToNSString( getWithPercentEscapes( urlWithFormat ) ) ];
    _request = [ [ NSMutableURLRequest alloc ] initWithURL:nsUrl cachePolicy:NSURLRequestReloadIgnoringCacheData timeoutInterval:timeout ];
    [ _request setHTTPShouldHandleCookies:YES ];
    _forwarder = [ [ ofxGenericHTTPConnectionForwarder alloc ] initWithDelegate:_this ];
#endif
    
    setMethod( method );
    setBody( body, bodyByteLength );
    setTimeout( timeout );

    setContentTypeHeader( ofxGMIMETypeToString( _format ) );
    setAcceptHeader( ofxGMIMETypeToString( _format ) );
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
    destroyResponseBody();
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
    setBody( ( void* )body.c_str(), (unsigned int)body.length() );
}

void ofxGenericHTTPRequest::setBody( void* body, unsigned int bodyByteLength )
{
#if TARGET_OS_IPHONE
    NSData* bodyAsData = nil;
    if ( body && bodyByteLength > 0 )
    {
        bodyAsData = [ NSData dataWithBytes:body length:bodyByteLength ];
    }
    [ _request setHTTPBody:bodyAsData ];
#endif
}

void ofxGenericHTTPRequest::setHeaderField( string field, string value )
{
#if TARGET_OS_IPHONE
    [ _request setValue:ofxStringToNSString( value ) forHTTPHeaderField:ofxStringToNSString( field ) ];
#endif
}

void ofxGenericHTTPRequest::setTimeout( float timeout )
{    
    _timeoutTimer = ofxGenericTimer::create( timeout, false, dynamic_pointer_cast< ofxGenericTimerDelegate >( _this ) );
#if TARGET_OS_IPHONE
    [ _request setTimeoutInterval:( NSTimeInterval )timeout ];
#endif
}

#if TARGET_OS_IPHONE
#define UserAgentHTTPHeaderField @"User-Agent"
#endif

string ofxGenericHTTPRequest::getUserAgent() const
{
    string result;
    
#if TARGET_OS_IPHONE
    result = ofxNSStringToString( [ _request valueForHTTPHeaderField:UserAgentHTTPHeaderField ] );
#elif TARGET_ANDROID
    throw ofxGenericExceptionMemberNotImplement( "ofxGenericHTTPRequest", "getUserAgent" );
#endif
    
    return result;
}

void ofxGenericHTTPRequest::setUserAgent( string value )
{
#if TARGET_OS_IPHONE
    [ _request setValue:ofxStringToNSString( value ) forHTTPHeaderField:UserAgentHTTPHeaderField ];
#elif TARGET_ANDROID
    throw ofxGenericExceptionMemberNotImplement( "ofxGenericHTTPRequest", "setUserAgent" );
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
    ofxGLogVerbose( toString( false ) );
#if DEBUG
    ofxGLogVerbose( "\nBody:\n" + getBodyAsString() );
#endif
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
    
    if( _timeoutTimer )
    {
        _timeoutTimer->stop();
        _timeoutTimer = ofPtr< ofxGenericTimer >();
    }
#endif
}


#define ErrorCacheKey "error"
#define ErrorDescriptionCacheKey "error_description"
#define ErrorRecoverySuggestionsCacheKey "error_recovery"

void ofxGenericHTTPRequest::timer_fired( ofPtr< ofxGenericTimer > timer )
{
    if( timer )
    {
        if( timer == _timeoutTimer )
        {
            ofPtr< ofxGenericValueStore > body = ofxGenericHTTPRequest::createErrorBody( "Connection to server has timed out." );
            finished(
                     408,
                     ofxGMIMETypeToString( ofxGenericMIMETypeJSON ),
                     "utf-8",
                     body,
                     ""
                     );
        }
    }
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
    if ( _timeoutTimer )
    {
        return _timeoutTimer->getTimeInterval();
    }
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

void ofxGenericHTTPRequest::appendQueryValueFieldPair( string value, string field )
{
#if TARGET_OS_IPHONE
    NSString *nsValue = [ofxStringToNSString(value) stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    NSString *nsField = [ofxStringToNSString(field) stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    if (![nsValue length] || ![nsField length]) {
        return;
    }
    NSString *nsValueFieldPair = [NSString stringWithFormat:@"%@=%@", nsValue, nsField];

    NSURLComponents *existingURLComponents = [[NSURLComponents alloc] initWithURL:[_request URL] resolvingAgainstBaseURL:YES];
    NSString *existingQuery = existingURLComponents.query;
    if ([existingQuery length] > 0) {
        nsValueFieldPair = [@"&" stringByAppendingString:nsValueFieldPair];
    } else {
        existingQuery = @"?";
    }
    existingURLComponents.query = [existingQuery stringByAppendingString:nsValueFieldPair];
    NSURL *newURL = [existingURLComponents URL];
    if (newURL && newURL.scheme && newURL.host) {
        _request.URL = newURL;
    } else {
        NSLog(@"Lumosity - Error: %@", [NSString stringWithFormat:@"URL (%@) with query string is invalid with query parameters: %@", [_request.URL description], nsValueFieldPair]);
    }
#endif
}

void ofxGenericHTTPRequest::appendSplitTestQueryValues( NSArray* splitTestList )
{
    if ( splitTestList == nil || [splitTestList count] == 0 )
        return;
    
    //form split test query string
    NSString *splitTestNameQueryParams = @"";
    for (id splitTestName in splitTestList) {
        if ( [splitTestNameQueryParams length] > 0 ) {
            splitTestNameQueryParams = [splitTestNameQueryParams stringByAppendingString:[NSString stringWithFormat:@"&[split_tests][name][]=%@", [splitTestName stringByAddingPercentEscapesUsingEncoding: NSUTF8StringEncoding]]];
        } else {
            splitTestNameQueryParams = [splitTestNameQueryParams stringByAppendingString:[NSString stringWithFormat:@"[split_tests][name][]=%@", [splitTestName stringByAddingPercentEscapesUsingEncoding: NSUTF8StringEncoding]]];
        }
    }

    //append string to existing call
    NSURLComponents *existingURLComponents = [[NSURLComponents alloc] initWithURL:[_request URL] resolvingAgainstBaseURL:YES];
    NSString *existingQuery = [existingURLComponents.query stringByAddingPercentEscapesUsingEncoding: NSUTF8StringEncoding];
    if ([existingQuery length] > 0) {
        splitTestNameQueryParams = [@"&" stringByAppendingString:splitTestNameQueryParams];
    } else {
        existingQuery = @"?";
    }
    
    //grab base url from existingUrlComponents
    //append param string manually because those damn brackets keep getting encoded
    //MAGIC DONT TOUCH
    existingURLComponents.query = nil;
    existingURLComponents.fragment = nil;
    NSURL *baseURL = [existingURLComponents URL];
    NSString *paramString = [existingQuery stringByAppendingString:splitTestNameQueryParams];
    NSString *fullUrlString = [[baseURL absoluteString] stringByAppendingString:paramString];
    NSURL *newURL = [[NSURL alloc] initWithString:fullUrlString];
    
    if (newURL && newURL.scheme && newURL.host) {
        _request.URL = newURL;
    } else {
        NSLog(@"Lumosity - Error: %@", [NSString stringWithFormat:@"URL (%@) with query string is invalid with split test query parameters: %@", [_request.URL description], splitTestNameQueryParams]);
    }

}

string ofxGenericHTTPRequest::toString( bool includebody ) const
{
    string result = "HTTPRequest -";
    result += " Url: " + getUrl();
    result += "\nMethod: " + getMethod();
    result += "\nTimeout: " + ofxGToString( getTimeout() );
    result += "\nHeaders:\n" + getHeaders();
    if ( includebody )
    {
        result += "\nBody:\n" + getBodyAsString();
    }
    return result;
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

/////////////////////////////////////////////////////////////////////////////
// Response

void ofxGenericHTTPRequest::finished(
              int statusCode,
              string stringMIMEType,
              string textEncoding,
              ofPtr< ofxGenericValueStore > body,
              string suggestedFileName
              )
{
    finished(
             statusCode,
             stringMIMEType,
             textEncoding,
             body->asString( ofxGStringToMIMEType( stringMIMEType ) ),
             suggestedFileName
             );
}

void ofxGenericHTTPRequest::finished(
              int statusCode,
              string stringMIMEType,
              string textEncoding,
              string body,
              string suggestedFileName
              )
{
    finished(
             statusCode,
             stringMIMEType,
             textEncoding,
             ( void* )body.c_str(),
             (unsigned int)body.length(),
             suggestedFileName
        );
}


void ofxGenericHTTPRequest::finished(
                                     int statusCode,
                                     string stringMIMEType,
                                     string textEncoding,
                                     void* data,
                                     unsigned int dataByteLength,
                                     string suggestedFileName
                                     )
{
    stop();
    
    _responseStatusCode = statusCode;
    _responseMIMEType = ofxGStringToMIMEType( stringMIMEType );
    _responseTextEncoding = textEncoding;
    setResponseBody( data, dataByteLength );
    _responseSuggestedFileName = suggestedFileName;
    
    if ( isParsable( _responseMIMEType, _responseTextEncoding ) )
    {
        string bodyAsString = getResponseBodyAsString();
        if ( !bodyAsString.empty() )
        {
            _responseParsedBody = ofxGenericValueStore::createFrom( _responseMIMEType, bodyAsString );
        }
    }
    
    if ( isResponseOk() )
    {
#if DEBUG
        ofxGLogVerbose( responseToString( false ) );
        ofxGLogVerbose( "\nBody:\n" + getResponseBodyAsString() + "\n" );
#endif
    }
    else
    {
        string warning = toString( _dumpBodyOnError ) + "\n" + responseToString( _dumpBodyOnError ) + "\n";
        ofxGLogWarning( warning );
    }
    
    doRequestFinished();
    
    if ( _delegate )
    {
        if ( isResponseOk() )
        {
            _delegate.lock()->httpRequest_finishedSuccessfully( _this.lock() );
        } else
        {
            _delegate.lock()->httpRequest_finishedWithError( _this.lock() );
        }
        _delegate.lock()->httpRequest_finished( _this.lock() );
    }
}

void ofxGenericHTTPRequest::doRequestFinished()
{
    //for subclasses to do things after the request has finished
}

void ofxGenericHTTPRequest::destroyResponseBody()
{
    if ( _responseBody )
    {
        delete [] ( BodyType )_responseBody;
        _responseBody = NULL;
        _responseBodyByteLength = 0;
    }
}

#if TARGET_OS_IPHONE
void ofxGenericHTTPRequest::finishedWithError( NSError* error )
{
    ofPtr< ofxGenericValueStore > body = ofxGenericHTTPRequest::createErrorBody(
                                                                                ofxNSStringToString( [ error localizedDescription ] ),
                                                                                ofxNSStringToString( [ error localizedFailureReason ] ),
                                                                                ofxNSStringToString( [ error localizedRecoverySuggestion ] )
                                                                                );
    finished(
             400,
             ofxGMIMETypeToString( ofxGenericMIMETypeJSON ),
             "utf-8",
             body,
             ""
             );
}

void ofxGenericHTTPRequest::finishedSuccessfully( NSURLResponse* response, NSData* data )
{
    int statusCode;
    if ( [ response isKindOfClass:[ NSHTTPURLResponse class ] ] )
    {
        NSHTTPURLResponse* httpResponse = ( NSHTTPURLResponse* )response;
        statusCode = (int)[ httpResponse statusCode ];
    } else
    {
        statusCode = 200;
    }
    
    finished(
             statusCode,
             ofxNSStringToString( [ response MIMEType ] ),
             ofxNSStringToString( [ response textEncodingName ] ),
             ( void* )[ data bytes ],
             (unsigned int)[ data length ],
             ofxNSStringToString( [ response suggestedFilename ] )
             );
}
#endif

ofPtr< ofxGenericValueStore > ofxGenericHTTPRequest::getResponseParsedBody() const
{
    return _responseParsedBody;
}

bool ofxGenericHTTPRequest::isResponseOk() const
{
    return getResponseStatusCode() >= 200 && getResponseStatusCode() <= 299;
}

int ofxGenericHTTPRequest::getResponseStatusCode() const
{
    return _responseStatusCode;
}

ofxGenericMIMEType ofxGenericHTTPRequest::getResponseMIMEType() const
{
    return _responseMIMEType;
}

string ofxGenericHTTPRequest::getResponseTextEncoding() const
{
    return _responseTextEncoding;
}

string ofxGenericHTTPRequest::getResponseSuggestedFileName() const
{
    return _responseSuggestedFileName;
}

void ofxGenericHTTPRequest::setResponseBody( void* body, unsigned int bodyByteLength )
{
    if ( body != NULL & bodyByteLength > 0 )
    {
        _responseBodyByteLength = bodyByteLength;
        _responseBody = new BodyType[ _responseBodyByteLength ];
        memcpy( _responseBody, body, _responseBodyByteLength );
    } else
    {
        _responseBodyByteLength = 0;
        _responseBody = 0;
    }
}

void* ofxGenericHTTPRequest::getResponseBody() const
{
    return _responseBody;
}

unsigned int ofxGenericHTTPRequest::getResponseBodyByteLength() const
{
    return _responseBodyByteLength;
}

bool ofxGenericHTTPRequest::isParsable( ofxGenericMIMEType MIMEType, string textEncoding )
{
    return textEncoding == "utf-8" && ( MIMEType == ofxGenericMIMETypeJSON || MIMEType == ofxGenericMIMETypeXML );
}

string ofxGenericHTTPRequest::getResponseBodyAsString() const
{
    return ofxGToString( _responseBody, _responseBodyByteLength );
}

string ofxGenericHTTPRequest::getResponseErrorName() const
{
    ofPtr< ofxGenericValueStore > parsedData = getResponseParsedBody();
    if ( parsedData )
    {
        return parsedData->read( ErrorCacheKey, "" );
    }
    return string();
}

string ofxGenericHTTPRequest::getResponseErrorDescription() const
{
    ofPtr< ofxGenericValueStore > parsedBody = getResponseParsedBody();
    if ( parsedBody )
    {
        return parsedBody->read( ErrorDescriptionCacheKey, "" );
    }
    return string();
}

string ofxGenericHTTPRequest::getResponseErrorRecoverySuggestions() const
{
    ofPtr< ofxGenericValueStore > parsedBody = getResponseParsedBody();
    if ( parsedBody )
    {
        return parsedBody->read( ErrorRecoverySuggestionsCacheKey, "" );
    }
    return string();
}

string ofxGenericHTTPRequest::responseToString( bool includeBody ) const
{
    string result = "HTTPResponse - Status: " + ofxGToString( getResponseStatusCode() );
    if ( isResponseOk() )
    {
        result += " MIMEType: " + ofxGMIMETypeToString( getResponseMIMEType() ) + " Text Encoding: " + getResponseTextEncoding() + " Suggested File Name: " + getResponseSuggestedFileName();
        
        if ( includeBody )
        {
            string bodyAsString = getResponseBodyAsString();
            if ( !bodyAsString.empty() )
            {
                result += " Body:\n " + bodyAsString;
            }
        }
    } else
    {
        result += " Error\n";
        if ( !getResponseErrorName().empty() )
        {
            result += "Name: " + getResponseErrorName() + "\n";
        }
        if ( !getResponseErrorDescription().empty() )
        {
            result += "Description: " + getResponseErrorDescription() + "\n";
        }
        if ( !getResponseErrorRecoverySuggestions().empty() )
        {
            result += "Suggestions: " + getResponseErrorRecoverySuggestions() + "\n";
        }
        if ( includeBody )
        {
            string bodyAsString = getResponseBodyAsString();
            if ( !bodyAsString.empty() )
            {
                result += "Body:\n " + bodyAsString;
            }
        }
    }
    return result;
}

ofPtr< ofxGenericValueStore > ofxGenericHTTPRequest::createErrorBody( string errorName, string errorDescription, string errorRecoverySuggestions )
{
    ofPtr< ofxGenericValueStore > body = ofxGenericValueStore::create( false );
    body->write( ErrorCacheKey, errorName );
    body->write( ErrorDescriptionCacheKey, errorDescription );
    body->write( ErrorRecoverySuggestionsCacheKey, errorRecoverySuggestions );
    return body;
}

void ofxGenericHTTPRequest::setDumpBodyOnError( bool dump )
{
    _dumpBodyOnError = dump;
}

/////////////////////////////////////////////////////////////////////////////
// iOS Forwarder

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

@end
#endif