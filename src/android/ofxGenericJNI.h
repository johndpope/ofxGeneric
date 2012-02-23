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

#include "JNI.h"
