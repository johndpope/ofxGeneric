//
//  ofxGenericException.mm
//  ofxGeneric
//
//  Created by Ian Grossberg on 4/18/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericException.h"
#include "ofxGenericUtility.h"

ofxGenericException::ofxGenericException( const char* what ) throw()
: _what( NULL )
{    
    setWhat( what );
}

ofxGenericException::ofxGenericException( std::exception translate ) throw()
: _what( NULL )
{
    setWhat( translate.what() );
}

#if TARGET_OS_IPHONE
ofxGenericException::ofxGenericException( NSException* translate ) throw()
: _what( NULL )
{
    if ( translate )
    {
        NSString* nsWhat = [ NSString stringWithFormat:@"%@ - %@ - %@ - %@ - %@", translate.name, translate.reason, translate.userInfo, translate.callStackReturnAddresses, translate.callStackSymbols ];
        setWhat( [ nsWhat UTF8String ] );
    } else
    {
        setWhat( "Unknown exception" );
    }
}
#endif

ofxGenericException::~ofxGenericException() throw()
{
    if ( _what )
    {
        delete [] _what;
        _what = NULL;
    }
}

void ofxGenericException::setWhat( const char* what )
{
    if ( _what )
    {
        delete [] _what;
    }
    _what = new char[ strlen( what ) + 1 ];
    strcpy( _what, what );
}

const char* ofxGenericException::what() const throw()
{
    return _what;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

ofxGenericExceptionForClass::ofxGenericExceptionForClass( const char* what, const char* className ) throw()
: ofxGenericException( what ), _className( NULL )
{
    if ( className )
    {   
        _className = new char[ strlen( className ) + 1 ];
        strcpy( _className, className );
    }    
}

const char* ofxGenericExceptionForClass::className() const throw()
{
    return _className;
}

ofxGenericExceptionForClass::~ofxGenericExceptionForClass() throw()
{
    if ( _className )
    {
        delete [] _className;
        _className = NULL;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

ofxGenericExceptionSubclassedSingletonInitializedTwice::ofxGenericExceptionSubclassedSingletonInitializedTwice( const char* className ) throw()
: ofxGenericExceptionForClass( "Subclassed singleton initialization was called twice", className )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

ofxGenericExceptionSingletonNotInstantiated::ofxGenericExceptionSingletonNotInstantiated( const char* className ) throw()
: ofxGenericExceptionForClass( "Singleton was not instantiated", className )
{
}