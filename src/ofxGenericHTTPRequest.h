//
//  ofxGenericHTTPRequest.h
//  iOS
//
//  Created by Ian Grossberg on 1/23/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"

#include "ofxGenericConstants.h"
#include "ofxGenericTimer.h"

class ofxGenericHTTPRequestDelegate;
#if TARGET_OS_IPHONE
@class ofxGenericHTTPConnectionForwarder;
#endif

class ofxGenericValueStore;

#define ofxGenericHTTPRequestDefaultTimeout 20.0f

class ofxGenericHTTPRequest : public ofxGenericTimerDelegate
{
public:
    static ofPtr< ofxGenericHTTPRequest > create(
                                                 string url,
                                                 string method,
                                                 ofxGenericMIMEType format,
                                                 string body,
                                                 ofPtr< ofxGenericHTTPRequestDelegate > delegate = ofPtr< ofxGenericHTTPRequestDelegate >(),
                                                 float timeout = ofxGenericHTTPRequestDefaultTimeout
                                                 );
    
    static ofPtr< ofxGenericHTTPRequest > create(
                                                 string url,
                                                 string method,
                                                 ofxGenericMIMEType format,
                                                 void* body = 0,
                                                 unsigned int bodyByteLength = 0,
                                                 ofPtr< ofxGenericHTTPRequestDelegate > delegate = ofPtr< ofxGenericHTTPRequestDelegate >(),
                                                 float timeout = ofxGenericHTTPRequestDefaultTimeout
                                                 );
    
    virtual ~ofxGenericHTTPRequest();
    
    string getUrl() const;
    string getMethod() const;
    float getTimeout() const;
    string getHeaders() const;
    string getBodyAsString() const;
    
    void appendQueryValueFieldPair( string value, string field );
    
    virtual void setBody( string body );
    virtual void setBody( void* body, unsigned int bodyByteLength );
    virtual void setHeaderField( string field, string value );
    virtual void setTimeout( float timeout );

    string getUserAgent() const;
    virtual void setUserAgent( string value );

    virtual void setContentTypeHeader( string value );
    virtual void setAcceptHeader( string value );
    virtual void setAuthorizationHeader( string value );
    
    virtual void start();
    virtual void cancel();
    
    void setDelegate( ofPtr< ofxGenericHTTPRequestDelegate > delegate );
    
    string toString( bool includeBody = true ) const;
    
    void finished(
                  int statusCode,
                  string stringMIMEType,
                  string textEncoding,
                  void* data,
                  unsigned int dataByteLength,
                  string suggestedFileName
                  );
    
    void finished(
                  int statusCode,
                  string stringMIMEType,
                  string textEncoding,
                  ofPtr< ofxGenericValueStore > body,
                  string suggestedFileName
                  );
    
    void finished(
                  int statusCode,
                  string stringMIMEType,
                  string textEncoding,
                  string body,
                  string suggestedFileName
                  );
    
#if TARGET_OS_IPHONE
    virtual void finishedWithError( NSError* error );
    virtual void finishedSuccessfully( NSURLResponse* urlResponse, NSData* receivedData );
#endif
    
    static string getWithPercentEscapes( string unencoded );
    static string getFromPercentEscapes( string encoded );
    
    static ofPtr< ofxGenericValueStore > createErrorBody(
                                                         string errorName,
                                                         string errorDescription = "",
                                                         string errorRecoverySuggestions = ""
                                                         );
    
    virtual bool isResponseOk() const;
    
    int getResponseStatusCode() const;
    
    ofxGenericMIMEType getResponseMIMEType() const;
    string getResponseTextEncoding() const;
    string getResponseSuggestedFileName() const;
    
    void* getResponseBody() const;
    unsigned int getResponseBodyByteLength() const;
    string getResponseBodyAsString() const;
    ofPtr< ofxGenericValueStore > getResponseParsedBody() const;
    
    virtual string getResponseErrorName() const;
    virtual string getResponseErrorDescription() const;
    string getResponseErrorRecoverySuggestions() const;
    
    string responseToString( bool includeBody = true ) const;

    void setDumpBodyOnError( bool dump );
protected:
    ofxGenericHTTPRequest();

    virtual void init(
                      ofPtrWeak< ofxGenericHTTPRequest > setThis,
                      string url,
                      string method,
                      ofxGenericMIMEType format,
                      ofPtr< ofxGenericValueStore > body,
                      ofPtrWeak< ofxGenericHTTPRequestDelegate > delegate,
                      float timeout
                      );
    
    virtual void init(
                      ofPtrWeak< ofxGenericHTTPRequest > setThis,
                      string url,
                      string method,
                      ofxGenericMIMEType format,
                      string body,
                      ofPtrWeak< ofxGenericHTTPRequestDelegate > delegate,
                      float timeout
                      );
    
    virtual void init(
                      ofPtrWeak< ofxGenericHTTPRequest > setThis,
                      string url,
                      string method,
                      ofxGenericMIMEType format,
                      void* body,
                      unsigned int bodyByteLength,
                      ofPtrWeak< ofxGenericHTTPRequestDelegate > delegate,
                      float timeout
                      );

    
    ofPtrWeak< ofxGenericHTTPRequest > _this;
    
    ofPtrWeak< ofxGenericHTTPRequestDelegate > _delegate;
    
    ofxGenericMIMEType _format;
    
#if TARGET_OS_IPHONE
    NSMutableURLRequest* _request;
    NSURLConnection* _connection;
    ofxGenericHTTPConnectionForwarder* _forwarder;
    ofPtr< ofxGenericTimer > _timeoutTimer;
#endif
    
    virtual void setMethod( string method );
    
    virtual void stop();
    
    virtual void doRequestFinished();
    
    void timer_fired( ofPtr< ofxGenericTimer > timer );
    
    int _responseStatusCode;
    ofxGenericMIMEType _responseMIMEType;
    string _responseTextEncoding;
    string _responseSuggestedFileName;
    void setResponseBody( void* body, unsigned int bodyByteLength );
    void destroyResponseBody();
    typedef unsigned char* BodyType;
    void* _responseBody;
    unsigned int _responseBodyByteLength;
    
    static bool isParsable( ofxGenericMIMEType MIMEType, string textEncoding );
    ofPtr< ofxGenericValueStore > _responseParsedBody;
    
    bool _dumpBodyOnError;
};

class ofxGenericHTTPRequestDelegate
{
public:
    virtual ~ofxGenericHTTPRequestDelegate(){};

    // TODO: get rid of error and success?
    virtual void httpRequest_finishedWithError( ofPtr< ofxGenericHTTPRequest > request ) {};
    virtual void httpRequest_finishedSuccessfully( ofPtr< ofxGenericHTTPRequest > request ) {};

    virtual void httpRequest_finished( ofPtr< ofxGenericHTTPRequest > request ) {};
};