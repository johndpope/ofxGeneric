//
//  ofxAppGenericWindow.h
//
//  Created by Ian Grossberg on 02/01/2012.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericConstants.h"
#include "ofMain.h"

#include <jni.h>

#include <map>
#include <exception>

jstring JNICStringToJavaString( const char string[] );
const char* JNIJavaStringToCString( jstring string );

void JNIHandleException();

const char* JNIGetClassName( jobject object );
jclass JNIFindClass( const char* className );

jmethodID JNIGetMethodID( jclass classObject, bool isStatic, const char* methodName, const char* methodSignature );

jobject JNICallStaticObjectMethod( jclass classObject, jmethodID methodID, va_list args );
jobject JNICallStaticObjectMethod( jclass classObject, jmethodID methodID, ... );
jobject JNICallStaticObjectMethod( const char* className, const char* methodName, const char* methodSig, ... );
jobject JNICallObjectMethod( jobject objectInstance, jmethodID methodID, va_list args );
jobject JNICallObjectMethod( jobject objectInstance, jmethodID methodID, ... );
void JNICallStaticVoidMethod( jclass classObject, jmethodID methodID, va_list args );
void JNICallStaticVoidMethod( jclass classObject, jmethodID methodID, ... );
void JNICallStaticVoidMethod( const char* className, const char* methodName, const char* methodSig, ... );
void JNICallVoidMethod( jobject objectInstance, jmethodID methodID, va_list args );
jint JNICallIntMethod( jobject objectInstance, jmethodID methodID, va_list args );
jint JNICallStaticIntMethod( jclass classObject, jmethodID methodID, va_list args );

void JNIDeleteLocalRef( jobject object );

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

string JNIGetBasicTypeSignatureEncoding( const JNIBasicTypeSignature& type );
string JNIGetObjectSignatureEncoding( const char* objectWithPath );
string JNIEncodeMethodSignature( int numParameters, JNIBasicTypeSignature returnType, ... );

class JNIException : public std::exception
{
public:
	JNIException( string message ) : _what( message ) {}
	~JNIException() throw() {}
	const char* what() const throw() { return _what.c_str(); }

private:
	string _what;
};
