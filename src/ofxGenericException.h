//
//  ofxGenericException.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 4/18/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"

#include <exception>
#if TARGET_OS_IPHONE
#import <Foundation/Foundation.h>
#endif

class ofxGenericException : public std::exception
{
public:
    ofxGenericException( const char* what ) throw();
    ofxGenericException( std::exception translate ) throw();
#if TARGET_OS_IPHONE
    ofxGenericException( NSException* translate ) throw();
#endif
    virtual ~ofxGenericException() throw();

    virtual const char* what() const throw();    
    
protected:
    void allocAndCopy( char*& to, const char* from );
    void dealloc( char*& from );
    char* _what;
};

///////////////////////////////////////////////////////////////

class ofxGenericExceptionForClass : public ofxGenericException
{
public:
    ofxGenericExceptionForClass( const char* what, const char* className ) throw();
    virtual const char* className() const throw();
    
    virtual ~ofxGenericExceptionForClass() throw();
protected:
    char* _className;
};

///////////////////////////////////////////////////////////////

class ofxGenericExceptionSubclassedSingletonInitializedTwice : public ofxGenericExceptionForClass
{
public:
    ofxGenericExceptionSubclassedSingletonInitializedTwice( const char* className ) throw();
};

///////////////////////////////////////////////////////////////

class ofxGenericExceptionSingletonNotInstantiated : public ofxGenericExceptionForClass
{
public:
    ofxGenericExceptionSingletonNotInstantiated( const char* className ) throw();
};