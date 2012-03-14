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
: data( NULL ), dataByteLength( 0 )
#if TARGET_OS_IPHONE
, _dataSource( nil )
#endif
{

}

void ofxGenericHTTPResponse::init( ofPtrWeak<ofxGenericHTTPResponse> setThis )
{
    _this = setThis;
}

void ofxGenericHTTPResponse::init( ofPtrWeak<ofxGenericHTTPResponse> setThis, string setErrorDescription, string setErrorFailureReason , string setErrorRecoverySuggestions )
{
    init( setThis );
    
    errorDescription = setErrorDescription;
    errorFailureReason = setErrorFailureReason;
    errorRecoverySuggestions = setErrorRecoverySuggestions;
    
    ofxGLog( OF_LOG_ERROR, "HTTPResponse - Error: " + errorDescription + " " + errorFailureReason + " " + errorRecoverySuggestions );
}

void ofxGenericHTTPResponse::init( ofPtrWeak< ofxGenericHTTPResponse > setThis, int setStatusCode, string setMIMEType, string setTextEncoding, void* setData, int setDataByteLength, string setSuggestedFilename )
{
    init( setThis );
    
    statusCode = setStatusCode;
    MIMEType = setMIMEType;
    textEncoding = setTextEncoding;
    data = setData;
    dataByteLength = setDataByteLength;
    suggestedFilename = setSuggestedFilename;

    ofxGLog( OF_LOG_VERBOSE, 
            "HTTPResponse - Status: %d MIMEType: %s Text Encoding: %s Suggested File Name: %s\nBody: %s", 
            statusCode, MIMEType.c_str(), textEncoding.c_str(), suggestedFilename.c_str(), getDataAsString().c_str() );
    
    if ( MIMEType == MIMEType_xml )
    {
        _xml = ofPtr< ofxXmlSettings >( new ofxXmlSettings() );
        TiXmlEncoding encoding = TIXML_DEFAULT_ENCODING;
        if ( textEncoding == "utf-8" )
        {
            encoding = TIXML_ENCODING_UTF8;
        }
        _xml->doc.Parse( getDataAsString().c_str(), NULL, encoding );
        if ( _xml->doc.Error() )
        {
            errorDescription = _xml->doc.ErrorDesc();
        }
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

string ofxGenericHTTPResponse::getDataAsString()
{
    char* dataBuffer = new char[ dataByteLength + 1 ];
    snprintf( dataBuffer, dataByteLength + 1, "%s", data );
    
    string dataString( dataBuffer );
    
    delete [] dataBuffer;
    
    return dataString;
}

ofPtr< ofxXmlSettings >ofxGenericHTTPResponse::getDataAsXML()
{
    return _xml;
}


