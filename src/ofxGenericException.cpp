//
//  ofxGenericException.mm
//  ofxGeneric
//
//  Created by Ian Grossberg on 4/18/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericException.h"
#include "ofxGenericUtility.h"

#if TARGET_OS_IPHONE
ofxGenericException::ofxGenericException( NSException* translate ) throw()
{
    if ( translate )
    {
        NSString* nsWhat = [ NSString stringWithFormat:@"%@ - %@ - %@ - %@ - %@", translate.name, translate.reason, translate.userInfo, translate.callStackReturnAddresses, translate.callStackSymbols ];
        _what = ofxNSStringToString( nsWhat );
    } else
    {
        _what = "Unknown exception";
    }
}
#endif

ofxGenericException::~ofxGenericException() throw()
{
}

const char* ofxGenericException::what() const throw()
{
    return _what.c_str();
}