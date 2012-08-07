//
//  ofxGenericHTTPRequest.h
//  iOS
//
//  Created by Ian Grossberg on 1/23/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"

class ofxGenericHTTPRequestDelegate;
class ofxGenericHTTPResponse;
#if TARGET_OS_IPHONE
@class ofxGenericHTTPConnectionForwarder;
#endif

class ofxGenericHTTPRequest
{
public:
    static ofPtr< ofxGenericHTTPRequest > create( string url, string method, string format, void* data = 0, int dataByteLength = 0, float timeout = 20.0f, ofPtr< ofxGenericHTTPRequestDelegate > delegate = ofPtr< ofxGenericHTTPRequestDelegate >() );
    static ofPtr< ofxGenericHTTPRequest > create( string url, string method, string format, string data, float timeout = 20.0f, ofPtr< ofxGenericHTTPRequestDelegate > delegate = ofPtr< ofxGenericHTTPRequestDelegate >() );
    virtual ~ofxGenericHTTPRequest();
    
    string getUrl() const;
    string getMethod() const;
    float getTimeout() const;
    string getHeaders() const;
    string getBodyAsString() const;
    
    virtual void setBody( string body );
    virtual void setBody( void* body, unsigned int bodyByteLength );
    virtual void setHeaderField( string field, string value );
    virtual void setTimeout( float timeout );

    virtual void setContentTypeHeader( string value );
    virtual void setAcceptHeader( string value );
    virtual void setAuthorizationHeader( string value );
    
    virtual void start();
    virtual void cancel();
    
    void setDelegate( ofPtr< ofxGenericHTTPRequestDelegate > delegate );
    
    string toString() const;
    
    void finished( int statusCode, string MIMEType, string textEncoding, void* data, int dataByteLength, string suggestedFileName = "" );
#if TARGET_OS_IPHONE
    virtual void finishedWithError( NSError* error );
    virtual void finishedSuccessfully( NSURLResponse* urlResponse, NSData* receivedData );
#endif
    
    ofPtr< ofxGenericHTTPResponse > getLastResponse();
    bool isLastResponseOk();
    
    static string getWithPercentEscapes( string unencoded );
    static string getFromPercentEscapes( string encoded );
    
protected:
    ofxGenericHTTPRequest();
    virtual void init( ofPtrWeak< ofxGenericHTTPRequest > setThis, string url, string method, string format, void* data = 0, int dataByteLength = 0, float timeout = 20.0f, ofPtr< ofxGenericHTTPRequestDelegate > delegate = ofPtr< ofxGenericHTTPRequestDelegate >() );
    virtual void init( ofPtrWeak< ofxGenericHTTPRequest > setThis, string url, string method, string format, string data = "", float timeout = 20.0f, ofPtr< ofxGenericHTTPRequestDelegate > delegate = ofPtr< ofxGenericHTTPRequestDelegate >() );
    ofPtrWeak< ofxGenericHTTPRequest > _this;
    
    ofPtr< ofxGenericHTTPRequestDelegate > _delegate;
    
    string _format;
    
#if TARGET_OS_IPHONE
    NSMutableURLRequest* _request;
    NSURLConnection* _connection;
    ofxGenericHTTPConnectionForwarder* _forwarder;
#endif
    
    virtual ofPtr< ofxGenericHTTPResponse > createResponse( int statusCode, string MIMEType, string textEncoding, void* data, int dataByteLength, string suggestedFileName );
    ofPtr< ofxGenericHTTPResponse > _lastResponse;
    
    virtual void setMethod( string method );
    
    virtual void stop();
};

class ofxGenericHTTPRequestDelegate
{
public:
    virtual ~ofxGenericHTTPRequestDelegate(){};

    virtual void httpRequest_finishedWithError( ofPtr< ofxGenericHTTPRequest > request ) = 0;
    virtual void httpRequest_finishedSuccessfully( ofPtr< ofxGenericHTTPRequest > request ) = 0;
};