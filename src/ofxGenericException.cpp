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
    allocAndCopy( _what, what );
    ofxGLog( OF_LOG_VERBOSE, "ofxGenericException thrown: %s", _what );
}

ofxGenericException::ofxGenericException( std::exception translate ) throw()
: _what( NULL )
{
    allocAndCopy( _what, translate.what() );
    ofxGLog( OF_LOG_VERBOSE, "ofxGenericException thrown: %s", _what );
}

#if TARGET_OS_IPHONE
ofxGenericException::ofxGenericException( NSException* translate ) throw()
: _what( NULL )
{
    if ( translate )
    {
        NSString* nsWhat = [ NSString stringWithFormat:@"%@ - %@ - %@ - %@ - %@", translate.name, translate.reason, translate.userInfo, translate.callStackReturnAddresses, translate.callStackSymbols ];
        allocAndCopy( _what, [ nsWhat UTF8String ] );
    } else
    {
        allocAndCopy( _what, "Unknown exception" );
    }
    ofxGLog( OF_LOG_VERBOSE, "ofxGenericException thrown: %s", _what );
}
#endif

ofxGenericException::~ofxGenericException() throw()
{
    dealloc( _what );
}

void ofxGenericException::allocAndCopy( char*& to, const char* from )
{
    if ( from )
    {
        int length = strlen( from ) + 1;
        to = new char[ length ];
        strcpy( to, from );
    } else 
    {
        to = NULL;
    }
}

void ofxGenericException::dealloc( char*& from )
{
    if ( from )
    {
        delete [] from;
        from = NULL;
    }
}

const char* ofxGenericException::what() const throw()
{
    return _what;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

ofxGenericExceptionForClass::ofxGenericExceptionForClass( const char* what, const char* className ) throw()
: ofxGenericException( what ), _className( NULL )
{
    allocAndCopy( _className, className );
}

const char* ofxGenericExceptionForClass::className() const throw()
{
    return _className;
}

ofxGenericExceptionForClass::~ofxGenericExceptionForClass() throw()
{
    dealloc( _className );
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