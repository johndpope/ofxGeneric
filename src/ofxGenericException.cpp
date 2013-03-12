//
//  ofxGenericException.mm
//  ofxGeneric
//
//  Created by Ian Grossberg on 4/18/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericException.h"
#include "ofxGenericUtility.h"
#include <execinfo.h>

ofxGenericException::ofxGenericException( const char* what ) throw()
: _what( NULL )
{    
    captureStackWithWhat( what );
}

ofxGenericException::ofxGenericException( std::exception translate ) throw()
: _what( NULL )
{
    captureStackWithWhat( translate.what() );
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
}
#endif

ofxGenericException::~ofxGenericException() throw()
{
    dealloc( _what );
}

void ofxGenericException::allocAndCopy( char*& to, const char* from ) throw()
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

void ofxGenericException::dealloc( char*& from ) throw()
{
    if ( from )
    {
        delete [] from;
        from = NULL;
    }
}

void ofxGenericException::captureStackWithWhat( const char* what ) throw()
{
    void* callstack[ 128 ];
    int frames = backtrace( callstack, 128 );
    char** strs = backtrace_symbols( callstack, frames );
    
    char buffer[ 10000 ];
    int offset = snprintf( buffer, 10000, "%s\n", what );
    for ( int i = 0; i < frames && offset < 10000; ++i ) 
    {
        offset += snprintf( buffer + offset, 10000 - offset, "%s\n", strs[ i ] );
    }
    free( strs );
    allocAndCopy( _what, buffer );
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

ofxGenericExceptionMemberNotImplement::ofxGenericExceptionMemberNotImplement( const char* className, const char* memberName ) throw()
: ofxGenericExceptionForClass( "not implemented", className ), _memberName( NULL )
{
    allocAndCopy( _memberName, memberName );
}

const char* ofxGenericExceptionMemberNotImplement::memberName() const throw()
{
    return _memberName;
}

ofxGenericExceptionMemberNotImplement::~ofxGenericExceptionMemberNotImplement() throw()
{
    dealloc( _memberName );
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