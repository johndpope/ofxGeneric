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
    ofxGenericHTTPRequest( string url, string method, void* data = 0, int dataByteLength = 0, ofPtr< ofxGenericHTTPRequestDelegate > delegate = ofPtr< ofxGenericHTTPRequestDelegate >() );
    virtual ~ofxGenericHTTPRequest();
    
    virtual void cancel();
    
    virtual void finishedWithError( ofPtr< ofxGenericHTTPResponse > response );
    virtual void finishedSuccessfully( ofPtr< ofxGenericHTTPResponse > response );
    
protected:
    ofPtr< ofxGenericHTTPRequestDelegate > _delegate;
    
#if TARGET_OS_IPHONE
    NSURLConnection* _connection;
    NSURLConnectionDelegateForwarder* _forwarder;
#endif
};

class ofxGenericHTTPRequestDelegate
{
public:
    virtual ~ofxGenericHTTPRequestDelegate(){};
    // TODO: straight pointer :(
    virtual void finishedWithError( ofxGenericHTTPRequest* request, ofPtr< ofxGenericHTTPResponse > response ) = 0;
    virtual void finishedSuccessfully( ofxGenericHTTPRequest* request, ofPtr< ofxGenericHTTPResponse > response ) = 0;
};

#if TARGET_OS_IPHONE
@interface NSURLConnectionDelegateForwarder : NSObject< NSURLConnectionDelegate >
{
@protected
    ofxGenericHTTPRequest* _delegate;
    NSMutableData* _receivedData;
    NSURLResponse* _response;
}
-( id )initWithDelegate:( ofxGenericHTTPRequest* )delegate;

-( void )connection:( NSURLConnection* )connection didFailWithError:( NSError* )error;
-( void )connection:( NSURLConnection* )connection didReceiveResponse:( NSURLResponse* )response;
-( void )connection:( NSURLConnection* )connection didReceiveData:( NSData* )data;
-( void )connectionDidFinishLoading:( NSURLConnection* )connection;

@end
#endif
