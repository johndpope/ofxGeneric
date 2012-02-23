/*
 * JNITypes.h
 *
 *  Created on: Feb 22, 2012
 *      Author: ian
 */

#pragma once

#include <exception>
#include <string>
#include <stdio.h>
#include <jni.h>

enum JNIBasicTypeSignature
{
	JNIType_bool,
	JNIType_byte,
	JNIType_char,
	JNIType_short,
	JNIType_int,
	JNIType_long,
	JNIType_float,
	JNIType_double,
	JNIType_array,
	JNIType_void,
	JNIType_object
};

std::string JNIGetBasicTypeSignatureEncoding( const JNIBasicTypeSignature& type );
std::string JNIGetObjectSignatureEncoding( std::string objectWithPath );
std::string JNIEncodeMethodSignature( int numParameters, JNIBasicTypeSignature returnType, ... );

class JNIException : public std::exception
{
public:
	JNIException( std::string message ) : _what( message ) {}
	~JNIException() throw() {}
	const char* what() const throw() { return _what.c_str(); }

private:
	std::string _what;
};

void JNIHandleException();

jstring JNICStringToJavaString( const char string[] );
const char* JNIJavaStringToCString( jstring string );

const char* JNIGetClassName( jobject object );
jclass JNIFindClass( const char* className );

