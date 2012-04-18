//
//  ofxGenericHTTPResponse.h
//  iOS
//
//  Created by Ian Grossberg on 1/23/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"
#include "ofxXmlSettings.h"

class ofxGenericHTTPRequest;
class ofxXmlSettings;

class ofxGenericHTTPResponse
{
    friend class ofxGenericHTTPRequest;
    
public:    
    int getStatusCode();
    
    string getMIMEType();
    string getTextEncoding();
    string getSuggestedFilename();    
    void* getData();
    int getDataByteLength();    
    string getDataAsString();
    ofPtr< ofxXmlSettings > getDataAsXML();
    
    virtual string getErrorDescription();
    string getErrorFailureReason();
    string getErrorRecoverySuggestions();
        
    virtual bool isOk();
    
    virtual ~ofxGenericHTTPResponse();
    
protected:
    ofxGenericHTTPResponse();
    
    virtual void init( ofPtrWeak< ofxGenericHTTPResponse > setThis );
    virtual void init( ofPtrWeak< ofxGenericHTTPResponse > setThis, int statusCode, string MIMEType, string textEncoding, void* data, int dataByteLength, string suggestedFilename = "" );
    virtual void init( ofPtrWeak< ofxGenericHTTPResponse > setThis, string setErrorDescription, string setErrorFailureReason = "", string setErrorRecoverySuggestions = ""  );
#if TARGET_OS_IPHONE
    void retainData( NSData* data );
#endif

    ofPtrWeak< ofxGenericHTTPResponse > _this;
    
    int _statusCode;
    
    string _MIMEType;
    string _textEncoding;
    string _suggestedFilename;
    void* _data;
    int _dataByteLength;
#if TARGET_OS_IPHONE
    NSData* _dataSource;
#endif
    
    string _errorDescription;
    string _errorFailureReason;
    string _errorRecoverySuggestions;
    
    ofPtr< ofxXmlSettings > _xml;
    
    virtual TiXmlElement* findMainElement(){return NULL;};
    virtual TiXmlElement* getMainElement();
    TiXmlElement* _mainElement;
};
