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
    static ofPtr< ofxGenericHTTPRequest > create( string url, string method, void* data = 0, int dataByteLength = 0, ofPtr< ofxGenericHTTPRequestDelegate > delegate = ofPtr< ofxGenericHTTPRequestDelegate >() );
    virtual ~ofxGenericHTTPRequest();
    
    virtual void cancel();
    
    virtual void finishedWithError( ofPtr< ofxGenericHTTPResponse > response );
    virtual void finishedSuccessfully( ofPtr< ofxGenericHTTPResponse > response );
    
protected:
    ofxGenericHTTPRequest( string url, string method, void* data = 0, int dataByteLength = 0, ofPtr< ofxGenericHTTPRequestDelegate > delegate = ofPtr< ofxGenericHTTPRequestDelegate >() );
    ofPtrWeak< ofxGenericHTTPRequest > _this;
    
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

    virtual void httpRequest_finishedWithError( ofPtr< ofxGenericHTTPRequest > request, ofPtr< ofxGenericHTTPResponse > response ) = 0;
    virtual void httpRequest_finishedSuccessfully( ofPtr< ofxGenericHTTPRequest > request, ofPtr< ofxGenericHTTPResponse > response ) = 0;
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
