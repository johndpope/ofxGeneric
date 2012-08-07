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
    static ofPtr< ofxGenericHTTPResponse > create( int statusCode, string MIMEType, string textEncoding, void* data, unsigned int dataByteLength, string suggestedFileName = "" );
    
    int getStatusCode() const;
    
    string getMIMEType() const;
    string getTextEncoding() const;
    string getSuggestedFileName() const;
    void* getData() const;
    unsigned int getDataByteLength() const;
    string getDataAsString() const;
    
    string getErrorName() const;
    string getErrorDescription() const;
    string getErrorRecoverySuggestions() const;
        
    virtual bool isOk() const;
    
    virtual ~ofxGenericHTTPResponse();
    
    string toString() const;
    
    static ofPtr< ofxGenericValueStore > createBody( string errorName, string errorDescription = "", string errorRecoverySuggestions = "" );
    
protected:
    ofxGenericHTTPResponse();
    
    virtual void init( ofPtrWeak< ofxGenericHTTPResponse > setThis, int statusCode, string MIMEType, string textEncoding, void* data, unsigned int dataByteLength, string suggestedFilename = "" );

    ofPtrWeak< ofxGenericHTTPResponse > _this;
    
    int _statusCode;
    
    string _MIMEType;
    string _textEncoding;
    string _suggestedFileName;
    void setData( void* data, unsigned int dataByteLength );
    typedef unsigned char* DataType;
    void* _data;
    unsigned int _dataByteLength;
    
    static bool isParsable( string MIMEType, string textEncoding );
    ofPtr< ofxGenericValueStore > _parsedData;
    ofPtr< ofxGenericValueStore > getParsedData() const;
};
