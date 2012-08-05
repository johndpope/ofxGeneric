//
//  ofxGenericHTTPResponse.cpp
//
//  Created by Ian Grossberg on 1/23/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericHTTPResponse.h"
#include "ofxGenericUtility.h"

#define MIMEType_xml "application/xml"

#if TARGET_OS_IPHONE
void ofxGenericHTTPResponse::retainData( NSData* data )
{
    if ( data != _dataSource )
    {
        if ( _dataSource )
        {
            release( _dataSource );
        }
        _dataSource = [ data retain ];
    }
}
#endif

ofxGenericHTTPResponse::ofxGenericHTTPResponse()
: _data( NULL ), _dataByteLength( 0 ), _mainElement( NULL )
#if TARGET_OS_IPHONE
, _dataSource( nil )
#endif
{

}

void ofxGenericHTTPResponse::init( ofPtrWeak<ofxGenericHTTPResponse> setThis )
{
    _this = setThis;
    _mainElement = findMainElement();
}

void ofxGenericHTTPResponse::init( ofPtrWeak<ofxGenericHTTPResponse> setThis, string errorDescription, string errorFailureReason , string errorRecoverySuggestions )
{
    init( setThis );
    
    _errorDescription = errorDescription;
    _errorFailureReason = errorFailureReason;
    _errorRecoverySuggestions = errorRecoverySuggestions;
    _mainElement = findMainElement();
    
    ofxGLog( OF_LOG_ERROR, "HTTPResponse - Error: " + _errorDescription + " " + _errorFailureReason + " " + _errorRecoverySuggestions );
}

void ofxGenericHTTPResponse::init( ofPtrWeak< ofxGenericHTTPResponse > setThis, int setStatusCode, string setMIMEType, string setTextEncoding, void* setData, int setDataByteLength, string setSuggestedFilename )
{
    init( setThis );
    
    _statusCode = setStatusCode;
    _MIMEType = setMIMEType;
    _textEncoding = setTextEncoding;
    _data = setData;
    _dataByteLength = setDataByteLength;
    _suggestedFilename = setSuggestedFilename;

    ofxGLog( OF_LOG_VERBOSE, 
            "HTTPResponse - Status: %d MIMEType: %s Text Encoding: %s Suggested File Name: %s\nBody: %s", 
            _statusCode, _MIMEType.c_str(), _textEncoding.c_str(), _suggestedFilename.c_str(), getDataAsString().c_str() );
    
    if ( _MIMEType == MIMEType_xml )
    {
        _xml = ofPtr< ofxXmlSettings >( new ofxXmlSettings() );
        TiXmlEncoding encoding = TIXML_DEFAULT_ENCODING;
        if ( _textEncoding == "utf-8" )
        {
            encoding = TIXML_ENCODING_UTF8;
        }
        _xml->doc.Parse( getDataAsString().c_str(), NULL, encoding );
    }
    _mainElement = findMainElement();
}

bool ofxGenericHTTPResponse::isOk()
{
    if ( _xml )
    {
        if ( _xml->tagExists( "response" ) )
        {
            return ( bool )_xml->getAttribute( "response", "success", 0 );
        }
    }
    return false;
}

ofxGenericHTTPResponse::~ofxGenericHTTPResponse()
{
#if TARGET_OS_IPHONE
    release( _dataSource );
#endif
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

string ofxGenericHTTPResponse::getSuggestedFilename() const
{
    return _suggestedFilename;
}

void* ofxGenericHTTPResponse::getData() const
{
    return _data;
}

int ofxGenericHTTPResponse::getDataByteLength() const
{
    return _dataByteLength;
}

string ofxGenericHTTPResponse::getDataAsString() const
{
    char* dataBuffer = new char[ _dataByteLength + 1 ];
    snprintf( dataBuffer, _dataByteLength + 1, "%s", _data );
    
    string dataString( dataBuffer );
    
    delete [] dataBuffer;
    
    return dataString;
}

ofPtr< ofxXmlSettings >ofxGenericHTTPResponse::getDataAsXML()
{
    return _xml;
}

string ofxGenericHTTPResponse::getErrorDescription()
{
    //this will be set early by HTTP errors, in that case we won't continue
    if ( _errorDescription.empty() )
    {
        //if there was an XML parsing error, then report that
        if ( _xml && _xml->doc.Error() )
        {
            _errorDescription = _xml->doc.ErrorDesc();
        }
        //otherwise, look through the XML to see if there are any errors within
        else
        {
            TiXmlElement* mainElement = getMainElement();
            if ( mainElement )
            {
                TiXmlElement* errors = mainElement->FirstChildElement( "errors" );
                
                if ( errors )
                {
                    TiXmlElement* error = errors->FirstChildElement( "error" );
                    if ( error )
                    {
                        _errorDescription = error->GetText();
                    }
                }
            }
        }
        
        //if we still haven't set an error, report an unknown one
        if ( _errorDescription.empty() )
        {
            _errorDescription = string("Unknown Error");
        }
    }
    
    return _errorDescription;
}

string ofxGenericHTTPResponse::getErrorFailureReason()
{
    return _errorFailureReason;
}

string ofxGenericHTTPResponse::getErrorRecoverySuggestions()
{
    return _errorRecoverySuggestions;
}

TiXmlElement* ofxGenericHTTPResponse::getMainElement()
{
    return _mainElement;
}

