//
//  ofxGenericHTTPResponse.h
//  iOS
//
//  Created by Ian Grossberg on 1/23/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"

#define MIMEType_xml "application/xml"
#define MIMEType_json "application/json"

class ofxGenericValueStore;

class ofxGenericHTTPResponse
{
public:
    static ofPtr< ofxGenericHTTPResponse > create(
                                                  int statusCode,
                                                  string MIMEType,
                                                  string textEncoding,
                                                  void* body,
                                                  unsigned int bodyByteLength,
                                                  string suggestedFileName
                                                  );
    
    int getStatusCode() const;
    
    string getMIMEType() const;
    string getTextEncoding() const;
    string getSuggestedFileName() const;
    
    void* getBody() const;
    unsigned int getBodyByteLength() const;
    string getBodyAsString() const;
    ofPtr< ofxGenericValueStore > getParsedBody() const;
    
    virtual string getErrorName() const;
    virtual string getErrorDescription() const;
    string getErrorRecoverySuggestions() const;
        
    virtual bool isOk() const;
    
    virtual ~ofxGenericHTTPResponse();
    
    string toString( bool includeBody = true ) const;
    
    static ofPtr< ofxGenericValueStore > createErrorBody( string errorName, string errorDescription = "", string errorRecoverySuggestions = "" );
    
protected:
    ofxGenericHTTPResponse();
    
    virtual void init( ofPtrWeak< ofxGenericHTTPResponse > setThis, int statusCode, string MIMEType, string textEncoding, void* body, unsigned int bodyByteLength, string suggestedFilename = "" );

    ofPtrWeak< ofxGenericHTTPResponse > _this;
    
    int _statusCode;
    
    string _MIMEType;
    string _textEncoding;
    string _suggestedFileName;
    void setBody( void* body, unsigned int bodyByteLength );
    typedef unsigned char* BodyType;
    void* _body;
    unsigned int _bodyByteLength;
    
    static bool isParsable( string MIMEType, string textEncoding );
    ofPtr< ofxGenericValueStore > _parsedBody;
};
