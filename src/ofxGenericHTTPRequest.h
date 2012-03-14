//
//  ofxGenericHTTPRequest.h
//  iOS
//
//  Created by Ian Grossberg on 1/23/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "ofxGenericHTTPResponse.h"

class ofxGenericHTTPRequestDelegate;
class ofxGenericHTTPResponse;
#if TARGET_OS_IPHONE
@class NSURLConnectionDelegateForwarder;
#endif

class ofxGenericHTTPRequest
{
public:
    static ofPtr< ofxGenericHTTPRequest > create( string url, string method, string format, void* data = 0, int dataByteLength = 0, float timeout = 20.0f, ofPtr< ofxGenericHTTPRequestDelegate > delegate = ofPtr< ofxGenericHTTPRequestDelegate >() );
    virtual ~ofxGenericHTTPRequest();
    
    virtual void start();
    virtual void cancel();
    
#if TARGET_OS_IPHONE
    virtual void finishedWithError( NSError* error );
    virtual void finishedSuccessfully( NSURLResponse* urlResponse, NSData* receivedData );
#endif
    
    ofPtr< ofxGenericHTTPResponse > getLastResponse();
    bool responseOk();
    
protected:
    ofxGenericHTTPRequest();
    virtual void init( ofPtrWeak< ofxGenericHTTPRequest > setThis, string url, string method, string format, void* data = 0, int dataByteLength = 0, float timeout = 20.0f, ofPtr< ofxGenericHTTPRequestDelegate > delegate = ofPtr< ofxGenericHTTPRequestDelegate >() );
    ofPtrWeak< ofxGenericHTTPRequest > _this;
    
    ofPtr< ofxGenericHTTPRequestDelegate > _delegate;
    
    string _format;
    
#if TARGET_OS_IPHONE
    NSMutableURLRequest* _request;
    NSURLConnection* _connection;
    NSURLConnectionDelegateForwarder* _forwarder;
#endif
    
    ofPtr< ofxGenericHTTPResponse > _lastResponse;
};

class ofxGenericHTTPRequestDelegate
{
public:
    virtual ~ofxGenericHTTPRequestDelegate(){};

    virtual void httpRequest_finishedWithError( ofPtr< ofxGenericHTTPRequest > request ) = 0;
    virtual void httpRequest_finishedSuccessfully( ofPtr< ofxGenericHTTPRequest > request ) = 0;
};

#if TARGET_OS_IPHONE
@interface NSURLConnectionDelegateForwarder : NSObject< NSURLConnectionDelegate >
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
