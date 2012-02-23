/*
 * JNIField.h
 *
 *  Created on: Feb 22, 2012
 *      Author: ian
 */

#pragma once

#include <jni.h>
#include <string>

class JNIField
{
public:
	JNIField( jclass classObject, bool isStatic, std::string name, std::string signature, bool ownReference = false );
	JNIField( jclass classObject, bool isStatic, jfieldID id, bool ownReference = false );
	~JNIField();

	jclass getClass();
	bool isStatic();
	jfieldID getID();

	std::string toString();

	jint getIntField( jobject instance = NULL );
	void setIntField( jint value, jobject instance = NULL );

protected:
	jclass _classObject;
	bool _ownReference;
	bool _isStatic;
	jfieldID _id;

	std::string _name;
	std::string _signature;
};
