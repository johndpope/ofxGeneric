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

ofPtr< ofxGenericHTTPResponse > ofxGenericHTTPResponse::create( int statusCode, string MIMEType, string textEncoding, void* data, unsigned int dataByteLength, string suggestedFileName )
{
    ofPtr< ofxGenericHTTPResponse > create( new ofxGenericHTTPResponse() );
    create->init( create, statusCode, MIMEType, textEncoding, data, dataByteLength, suggestedFileName );
    return create;
}

ofxGenericHTTPResponse::ofxGenericHTTPResponse()
: _statusCode( -1 ), _data( NULL ), _dataByteLength( 0 )
{
}

ofxGenericHTTPResponse::~ofxGenericHTTPResponse()
{
    if ( _data )
    {
        delete [] ( DataType )_data;
        _data = NULL;
        _dataByteLength = 0;
    }
}

void ofxGenericHTTPResponse::init( ofPtrWeak< ofxGenericHTTPResponse > setThis, int statusCode, string MIMEType, string textEncoding, void* data, unsigned int setDataByteLength, string suggestedFileName )
{
    _this = setThis;;
    
    _statusCode = statusCode;
    _MIMEType = MIMEType;
    _textEncoding = textEncoding;
    setData( data, setDataByteLength );
    _suggestedFileName = suggestedFileName;

    if ( isParsable( _MIMEType, _textEncoding ) )
    {
        string dataAsString = getDataAsString();
        if ( !dataAsString.empty() )
        {
            if ( _MIMEType == MIMEType_json )
            {
                _parsedData = ofxGenericValueStore::createFromJSON( dataAsString );
            } else if ( _MIMEType == MIMEType_xml )
            {
//                _parsedData = ofxGenericValueStore::createFromXML( dataAsString );
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

ofPtr< ofxGenericValueStore > ofxGenericHTTPResponse::getParsedData() const
{
    return _parsedData;
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

void ofxGenericHTTPResponse::setData( void* data, unsigned int dataByteLength )
{
    if ( data != NULL & dataByteLength > 0 )
    {
        _dataByteLength = dataByteLength;
        _data = new DataType[ _dataByteLength ];
        memcpy( _data, data, _dataByteLength );
    } else
    {
        _dataByteLength = 0;
        _data = 0;
    }
}

void* ofxGenericHTTPResponse::getData() const
{
    return _data;
}

unsigned int ofxGenericHTTPResponse::getDataByteLength() const
{
    return _dataByteLength;
}

bool ofxGenericHTTPResponse::isParsable( string MIMEType, string textEncoding )
{
    return textEncoding == "utf-8" && ( MIMEType == MIMEType_json || MIMEType == MIMEType_xml );
}

string ofxGenericHTTPResponse::getDataAsString() const
{
    return ofxGToString( _data, _dataByteLength );
}

string ofxGenericHTTPResponse::getErrorName() const
{
    ofPtr< ofxGenericValueStore > parsedData = getParsedData();
    if ( parsedData )
    {
        return parsedData->read( ErrorCacheKey, "" );
    }
    return string();
}

string ofxGenericHTTPResponse::getErrorDescription() const
{
    ofPtr< ofxGenericValueStore > parsedData = getParsedData();
    if ( parsedData )
    {
        return parsedData->read( ErrorDescriptionCacheKey, "" );
    }
    return string();
}

string ofxGenericHTTPResponse::getErrorRecoverySuggestions() const
{
    ofPtr< ofxGenericValueStore > parsedData = getParsedData();
    if ( parsedData )
    {
        return parsedData->read( ErrorRecoverySuggestionsCacheKey, "" );
    }
    return string();
}

string ofxGenericHTTPResponse::toString() const
{
    string result = "HTTPResponse - Status: " + ofxGToString( _statusCode );
    if ( isOk() )
    {
        result += " MIMEType: " + getMIMEType() + " Text Encoding: " + getTextEncoding() + " Suggested File Name: " + getSuggestedFileName();
        
        string bodyAsString = getDataAsString();
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
        string bodyAsString = getDataAsString();
        if ( !bodyAsString.empty() )
        {
            result += " Body:\n " + bodyAsString;
        }
    }
    return result;
}

ofPtr< ofxGenericValueStore > ofxGenericHTTPResponse::createBody( string errorName, string errorDescription, string errorRecoverySuggestions )
{
    ofPtr< ofxGenericValueStore > body = ofxGenericValueStore::create( false );
    body->write( ErrorCacheKey, errorName );
    body->write( ErrorDescriptionCacheKey, errorDescription );
    body->write( ErrorRecoverySuggestionsCacheKey, errorRecoverySuggestions );
    return body;
}
