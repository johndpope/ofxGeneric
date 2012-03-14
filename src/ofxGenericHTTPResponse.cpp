//
//  ofxGenericHTTPResponse.cpp
//
//  Created by Ian Grossberg on 1/23/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericHTTPResponse.h"
#include "ofxGenericUtility.h"

#include "ofxXmlSettings.h"

#define MIMEType_xml "application/xml"

ofPtr< ofxGenericHTTPResponse > ofxGenericHTTPResponse::create()
{
    ofPtr< ofxGenericHTTPResponse > create = ofPtr< ofxGenericHTTPResponse >( new ofxGenericHTTPResponse() );
    create->init( create );
    return create;
}

ofPtr< ofxGenericHTTPResponse > ofxGenericHTTPResponse::create( string setErrorDescription, string setErrorFailureReason, string setErrorRecoverySuggestions )
{
    ofPtr< ofxGenericHTTPResponse > create = ofPtr< ofxGenericHTTPResponse >( new ofxGenericHTTPResponse() );
    create->init( 
                 create, 
                 setErrorDescription,
                 setErrorFailureReason,
                 setErrorRecoverySuggestions
                 );
    return create;
}

ofPtr< ofxGenericHTTPResponse > ofxGenericHTTPResponse::create( int statusCode, string MIMEType, string textEncoding, void* data, int dataByteLength, string suggestedFilename )
{
    ofPtr< ofxGenericHTTPResponse > create = ofPtr< ofxGenericHTTPResponse >( new ofxGenericHTTPResponse() );
    create->init( 
                 create, 
                 statusCode,
                 MIMEType,
                 textEncoding,
                 data,
                 dataByteLength,
                 suggestedFilename
                 );
    return create;
}

#if TARGET_OS_IPHONE
ofPtr< ofxGenericHTTPResponse > ofxGenericHTTPResponse::create( NSError* error )
{    
    return ofxGenericHTTPResponse::create(
                                          ofxNSStringToString( [ error localizedDescription ] ), 
                                          ofxNSStringToString( [ error localizedFailureReason ] ),
                                          ofxNSStringToString( [ error localizedRecoverySuggestion ] )
                                          );
}

ofPtr< ofxGenericHTTPResponse > ofxGenericHTTPResponse::create( NSURLResponse* response, NSData* data )
{
    int statusCode;
    if ( [ response isKindOfClass:[ NSHTTPURLResponse class ] ] )
    {
        NSHTTPURLResponse* httpResponse = ( NSHTTPURLResponse* )response;
        statusCode = [ httpResponse statusCode ];
    } else
    {
        statusCode = -1;
    }
    
    ofPtr< ofxGenericHTTPResponse > create = ofxGenericHTTPResponse::create( 
                                                                            statusCode,
                                                                            ofxNSStringToString( [ response MIMEType ] ),
                                                                            ofxNSStringToString( [ response textEncodingName ] ),
                                                                            ( void* )[ data bytes ],
                                                                            [ data length ],
                                                                            ofxNSStringToString( [ response suggestedFilename ] ) 
                                                                            );
    create->_dataSource = [ data retain ];
    return create;
}
#endif

ofxGenericHTTPResponse::ofxGenericHTTPResponse()
: _data( NULL ), _dataByteLength( 0 )
#if TARGET_OS_IPHONE
, _dataSource( nil )
#endif
{

}

void ofxGenericHTTPResponse::init( ofPtrWeak<ofxGenericHTTPResponse> setThis )
{
    _this = setThis;
}

void ofxGenericHTTPResponse::init( ofPtrWeak<ofxGenericHTTPResponse> setThis, string errorDescription, string errorFailureReason , string errorRecoverySuggestions )
{
    init( setThis );
    
    _errorDescription = errorDescription;
    _errorFailureReason = errorFailureReason;
    _errorRecoverySuggestions = errorRecoverySuggestions;
    
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

int ofxGenericHTTPResponse::getStatusCode()
{
    return _statusCode;
}

string ofxGenericHTTPResponse::getMIMEType()
{
    return _MIMEType;
}

string ofxGenericHTTPResponse::getTextEncoding()
{
    return _textEncoding;
}

string ofxGenericHTTPResponse::getSuggestedFilename()
{
    return _suggestedFilename;
}

void* ofxGenericHTTPResponse::getData()
{
    return _data;
}

int ofxGenericHTTPResponse::getDataByteLength()
{
    return _dataByteLength;
}

string ofxGenericHTTPResponse::getDataAsString()
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
    if ( !_errorDescription.empty() )
    {
        return _errorDescription;
    } else if ( _xml && _xml->doc.Error() )
    {
        return _xml->doc.ErrorDesc();
    }
    return string();
}

string ofxGenericHTTPResponse::getErrorFailureReason()
{
    return _errorFailureReason;
}

string ofxGenericHTTPResponse::getErrorRecoverySuggestions()
{
    return _errorRecoverySuggestions;
}
