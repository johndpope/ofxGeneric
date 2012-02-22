/*
 * JNIMethod.h
 *
 *  Created on: Feb 22, 2012
 *      Author: ian
 */

#pragma once

#include "ofxGenericJNIObject.h"
#include <jni.h>

class JNIMethod
{
public:
	JNIMethod( jclass classObject, bool isStatic, string name, string signature, bool ownReference = false );
	JNIMethod( jclass classObject, bool isStatic, jmethodID id, bool ownReference = false );
	~JNIMethod();

	jclass getClass();
	bool isStatic();
	jmethodID getID();

	string toString();

	jobject createInstance( jobject instance, ... );
	jint callIntMethod( jobject instance, ... );

protected:
	jclass _classObject;
	bool _ownReference;
	bool _isStatic;
	jmethodID _id;

	string _name;
	string _signature;
};
