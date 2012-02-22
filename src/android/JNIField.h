/*
 * JNIField.h
 *
 *  Created on: Feb 22, 2012
 *      Author: ian
 */

#pragma once

#include "ofxGenericJNIObject.h"
#include <jni.h>

class JNIField
{
public:
	JNIField( jclass classObject, bool isStatic, string name, string signature, bool ownReference = false );
	JNIField( jclass classObject, bool isStatic, jfieldID id, bool ownReference = false );
	~JNIField();

	jclass getClass();
	bool isStatic();
	jfieldID getID();

	string toString();

	jint getIntField( jobject instance = NULL );
	void setIntField( jint value, jobject instance = NULL );

protected:
	jclass _classObject;
	bool _ownReference;
	bool _isStatic;
	jfieldID _id;

	string _name;
	string _signature;
};
