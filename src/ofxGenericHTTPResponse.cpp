//
//  ofxGenericHTTPResponse.cpp
//
//  Created by Ian Grossberg on 1/23/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericHTTPResponse.h"
#include "ofxGenericUtility.h"


ofPtr< ofxGenericHTTPResponse > ofxGenericHTTPResponse::create()
{
    ofPtr< ofxGenericHTTPResponse > create = ofPtr< ofxGenericHTTPResponse >( new ofxGenericHTTPResponse() );
    create->_this = create;
    return create;
}

ofPtr< ofxGenericHTTPResponse > ofxGenericHTTPResponse::create( NSError* error )
{    
    ofPtr< ofxGenericHTTPResponse > create = ofPtr< ofxGenericHTTPResponse >( new ofxGenericHTTPResponse( error ) );
    create->_this = create;
    return create;
}

ofPtr< ofxGenericHTTPResponse > ofxGenericHTTPResponse::create( NSURLResponse* response, NSData* data )
{
    ofPtr< ofxGenericHTTPResponse > create = ofPtr< ofxGenericHTTPResponse >( new ofxGenericHTTPResponse( response, data ) );
    create->_this = create;
    return create;
}

ofxGenericHTTPResponse::ofxGenericHTTPResponse()
: data( NULL ), dataByteLength( 0 )
#if TARGET_OS_IPHONE
, _dataSource( nil )
#endif
{

}

#if TARGET_OS_IPHONE
ofxGenericHTTPResponse::ofxGenericHTTPResponse( NSError* error )
: data( NULL ), dataByteLength( 0 )
, _dataSource( nil )
{
    errorDescription = ofxNSStringToString( [ error localizedDescription ] );
    errorFailureReason = ofxNSStringToString( [ error localizedFailureReason ] );
    errorRecoverySuggestions = ofxNSStringToString( [ error localizedRecoverySuggestion ] );
}

ofxGenericHTTPResponse::ofxGenericHTTPResponse( NSURLResponse* response, NSData* setData )
: data( NULL ), dataByteLength( 0 )
, _dataSource( nil )
{
    if ( [ response isKindOfClass:[ NSHTTPURLResponse class ] ] )
    {
        NSHTTPURLResponse* httpResponse = ( NSHTTPURLResponse* )response;
        statusCode = [ httpResponse statusCode ];
    } else
    {
        statusCode = -1;
    }
    MIMEType = ofxNSStringToString( [ response MIMEType ] );
    textEncoding = ofxNSStringToString( [ response textEncodingName ] );
    suggestedFilename = ofxNSStringToString( [ response suggestedFilename ] );

    data = ( void* )[ setData bytes ];
    dataByteLength = [ setData length ];
    _dataSource = [ setData retain ];
}
#endif

ofxGenericHTTPResponse::~ofxGenericHTTPResponse()
{
#if TARGET_OS_IPHONE
    release( _dataSource );
#endif
}
