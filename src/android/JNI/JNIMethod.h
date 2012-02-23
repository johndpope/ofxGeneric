/*
 * JNIMethod.h
 *
 *  Created on: Feb 22, 2012
 *      Author: ian
 */

#pragma once

#include <jni.h>
#include <string>
#include <stdarg.h>

class JNIMethod
{
public:
	JNIMethod( jclass classObject, bool isStatic, std::string name, std::string signature, bool ownReference = false );
	JNIMethod( jclass classObject, bool isStatic, jmethodID id, bool ownReference = false );
	~JNIMethod();

	jclass getClass();
	bool isStatic();
	jmethodID getID();

	std::string toString();

	jobject createInstance( jobject instance, ... );

	jobject callObjectMethod( jobject instance, va_list args );
	jobject callObjectMethod( jobject instance, ... );
	void callVoidMethod( jobject instance, va_list args );
	void callVoidMethod( jobject instance, ... );
	jint callIntMethod( jobject instance, va_list args );
	jint callIntMethod( jobject instance, ... );

protected:
	jclass _classObject;
	bool _ownReference;
	bool _isStatic;
	jmethodID _id;

	std::string _name;
	std::string _signature;
};
