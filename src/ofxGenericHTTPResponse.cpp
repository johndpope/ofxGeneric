//
//  ofxGenericHTTPResponse.cpp
//
//  Created by Ian Grossberg on 1/23/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericHTTPResponse.h"
#include "ofxGenericUtility.h"

#include "ofxGenericValueStore.h"
#include "ofxXMLSettings.h"

#define ErrorCacheKey "error"
#define ErrorDescriptionCacheKey "error_description"
#define ErrorRecoverySuggestionsCacheKey "error_recovery"

ofPtr< ofxGenericHTTPResponse > ofxGenericHTTPResponse::create(
                                                               int statusCode,
                                                               string MIMEType,
                                                               string textEncoding,
                                                               void* body,
                                                               unsigned int bodyByteLength,
                                                               string suggestedFileName
                                                               )
{
    ofPtr< ofxGenericHTTPResponse > create( new ofxGenericHTTPResponse() );
    create->init(
                 create,
                 statusCode,
                 MIMEType,
                 textEncoding,
                 body,
                 bodyByteLength,
                 suggestedFileName
                 );
    return create;
}

ofxGenericHTTPResponse::ofxGenericHTTPResponse()
: _statusCode( -1 ), _body( NULL ), _bodyByteLength( 0 )
{
}

ofxGenericHTTPResponse::~ofxGenericHTTPResponse()
{
    if ( _body )
    {
        delete [] ( BodyType )_body;
        _body = NULL;
        _bodyByteLength = 0;
    }
}

void ofxGenericHTTPResponse::init(
                                  ofPtrWeak< ofxGenericHTTPResponse > setThis,
                                  int statusCode,
                                  string MIMEType,
                                  string textEncoding,
                                  void* body,
                                  unsigned int bodyByteLength,
                                  string suggestedFileName
                                  )
{
    _this = setThis;
    
    _statusCode = statusCode;
    _MIMEType = MIMEType;
    _textEncoding = textEncoding;
    setBody( body, bodyByteLength );
    _suggestedFileName = suggestedFileName;

    if ( isParsable( _MIMEType, _textEncoding ) )
    {
        string bodyAsString = getBodyAsString();
        if ( !bodyAsString.empty() )
        {
            if ( _MIMEType == MIMEType_json )
            {
                _parsedBody = ofxGenericValueStore::createFromJSON( bodyAsString );
            } else if ( _MIMEType == MIMEType_xml )
            {
                _parsedBody = ofxGenericValueStore::createFromXML( bodyAsString );
            }
        }
    }

    if ( isOk() )
    {
        ofxGLogVerbose( toString() );
    } else
    {
        ofxGLogNotice( toString() );
    }
}

ofPtr< ofxGenericValueStore > ofxGenericHTTPResponse::getParsedBody() const
{
    return _parsedBody;
}

bool ofxGenericHTTPResponse::isOk() const
{
    return _statusCode >= 200 && _statusCode <= 299;
}

int ofxGenericHTTPResponse::getStatusCode() const
{
    return _statusCode;
}

string ofxGenericHTTPResponse::getMIMEType() const
{
    return _MIMEType;
}

string ofxGenericHTTPResponse::getTextEncoding() const
{
    return _textEncoding;
}

string ofxGenericHTTPResponse::getSuggestedFileName() const
{
    return _suggestedFileName;
}

void ofxGenericHTTPResponse::setBody( void* body, unsigned int bodyByteLength )
{
    if ( body != NULL & bodyByteLength > 0 )
    {
        _bodyByteLength = bodyByteLength;
        _body = new BodyType[ _bodyByteLength ];
        memcpy( _body, body, _bodyByteLength );
    } else
    {
        _bodyByteLength = 0;
        _body = 0;
    }
}

void* ofxGenericHTTPResponse::getBody() const
{
    return _body;
}

unsigned int ofxGenericHTTPResponse::getBodyByteLength() const
{
    return _bodyByteLength;
}

bool ofxGenericHTTPResponse::isParsable( string MIMEType, string textEncoding )
{
    return textEncoding == "utf-8" && ( MIMEType == MIMEType_json || MIMEType == MIMEType_xml );
}

string ofxGenericHTTPResponse::getBodyAsString() const
{
    return ofxGToString( _body, _bodyByteLength );
}

string ofxGenericHTTPResponse::getErrorName() const
{
    ofPtr< ofxGenericValueStore > parsedData = getParsedBody();
    if ( parsedData )
    {
        return parsedData->read( ErrorCacheKey, "" );
    }
    return string();
}

string ofxGenericHTTPResponse::getErrorDescription() const
{
    ofPtr< ofxGenericValueStore > parsedBody = getParsedBody();
    if ( parsedBody )
    {
        return parsedBody->read( ErrorDescriptionCacheKey, "" );
    }
    return string();
}

string ofxGenericHTTPResponse::getErrorRecoverySuggestions() const
{
    ofPtr< ofxGenericValueStore > parsedBody = getParsedBody();
    if ( parsedBody )
    {
        return parsedBody->read( ErrorRecoverySuggestionsCacheKey, "" );
    }
    return string();
}

string ofxGenericHTTPResponse::toString() const
{
    string result = "HTTPResponse - Status: " + ofxGToString( _statusCode );
    if ( isOk() )
    {
        result += " MIMEType: " + getMIMEType() + " Text Encoding: " + getTextEncoding() + " Suggested File Name: " + getSuggestedFileName();
        
        string bodyAsString = getBodyAsString();
        if ( !bodyAsString.empty() )
        {
            result += " Body:\n " + bodyAsString;
        }
    } else
    {
        result += " Error ";
        if ( !getErrorName().empty() )
        {
            result += " Name: " + getErrorName();
        }
        if ( !getErrorDescription().empty() )
        {
            result += " Description: " + getErrorDescription();
        }
        if ( !getErrorRecoverySuggestions().empty() )
        {
            result += " Suggestions: " + getErrorRecoverySuggestions();
        }
        string bodyAsString = getBodyAsString();
        if ( !bodyAsString.empty() )
        {
            result += " Body:\n " + bodyAsString;
        }
    }
    return result;
}

ofPtr< ofxGenericValueStore > ofxGenericHTTPResponse::createErrorBody( string errorName, string errorDescription, string errorRecoverySuggestions )
{
    ofPtr< ofxGenericValueStore > body = ofxGenericValueStore::create( false );
    body->write( ErrorCacheKey, errorName );
    body->write( ErrorDescriptionCacheKey, errorDescription );
    body->write( ErrorRecoverySuggestionsCacheKey, errorRecoverySuggestions );
    return body;
}
