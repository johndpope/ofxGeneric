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
@class NSURLConnectionForwarder;
#endif

class ofxGenericHTTPRequest
{
public:
    static ofPtr< ofxGenericHTTPRequest > create( string url, string method, string format, void* data = 0, int dataByteLength = 0, float timeout = 20.0f, ofPtr< ofxGenericHTTPRequestDelegate > delegate = ofPtr< ofxGenericHTTPRequestDelegate >() );
    static ofPtr< ofxGenericHTTPRequest > create( string url, string method, string format, string data, float timeout = 20.0f, ofPtr< ofxGenericHTTPRequestDelegate > delegate = ofPtr< ofxGenericHTTPRequestDelegate >() );
    virtual ~ofxGenericHTTPRequest();
    
    virtual void start();
    virtual void cancel();
    
    void finishedWithError( string errorDescription, string errorFailureReason = "", string errorRecoverySuggestion = "" );
    void finishedSuccessfully( int statusCode, string MIMEType, string textEncoding, void* data, int dataByteLength, string suggestedFilename = "" );
#if TARGET_OS_IPHONE
    virtual void finishedWithError( NSError* error );
    virtual void finishedSuccessfully( NSURLResponse* urlResponse, NSData* receivedData );
#endif
    
    void setDelegate( ofPtr< ofxGenericHTTPRequestDelegate > delegate );
    
    ofPtr< ofxGenericHTTPResponse > getLastResponse();
    
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
    NSURLConnectionForwarder* _forwarder;
#endif
    
    virtual ofPtr< ofxGenericHTTPResponse > createResponse();
    ofPtr< ofxGenericHTTPResponse > _lastResponse;
};

class ofxGenericHTTPRequestDelegate
{
public:
    virtual ~ofxGenericHTTPRequestDelegate(){};

    virtual void httpRequest_finishedWithError( ofPtr< ofxGenericHTTPRequest > request ) = 0;
    virtual void httpRequest_finishedSuccessfully( ofPtr< ofxGenericHTTPRequest > request ) = 0;
};

/////////////////////////////////////////////////////////

class ofxGenericHTTPRequestHolder : public ofxGenericHTTPRequestDelegate
{
public:
    virtual ~ofxGenericHTTPRequestHolder();
    static ofPtr< ofxGenericHTTPRequestHolder > getInstance();

    void holdRequestUntilComplete( ofPtr< ofxGenericHTTPRequest > request );
    
    void httpRequest_finishedWithError( ofPtr< ofxGenericHTTPRequest > request );
    void httpRequest_finishedSuccessfully( ofPtr< ofxGenericHTTPRequest > request );
    
protected:
    ofxGenericHTTPRequestHolder();
    static ofPtr< ofxGenericHTTPRequestHolder > _instance;
    void setofxGenericHTTPRequestHolderInstanceToThis();
    
    std::list< ofPtr< ofxGenericHTTPRequest > > _holdRequestUntilComplete;
    void removeHeldRequest( ofPtr< ofxGenericHTTPRequest > request );
};
