//
//  Created by Ian Grossberg on 02/01/2012.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include <jni.h>
#include <string>
#include <map>

class JNIMethod;
class JNIField;

class JNIObject
{
public:
	virtual ~JNIObject();

	virtual jclass getJNIClass() = 0;

    static JavaVM* getJavaVM();
    static void setJavaVM( JavaVM* javaVM );
    static JNIEnv* getJNIEnv();

	virtual jobject getJNIInstance() = 0;

protected:
    static JavaVM* _javaVM;

	enum JNIObjectMethods
	{
		Last
	};
	std::map< int, JNIMethod* > _jniMethods; // TODO: should be able to be made static per each class, passed into register and get

	static JNIMethod* getJNIMethod( const std::map< int, JNIMethod* >& methods, int methodEnum );

	static void registerJNIMethodID( std::map< int, JNIMethod* >& methods, jclass classObject, bool isStatic, int methodEnum, std::string methodName, std::string methodSignature );
	void registerJNIMethodID( std::map< int, JNIMethod* >& methods, bool isStatic, int methodEnum, std::string methodName, std::string methodSignature );

	static jobject createJNIInstance( JNIMethod* constructor, ... );
	static jobject createJNIInstance( std::map< int, JNIMethod* >& methods, int methodEnum, ... );

	jobject callJNIObjectMethod( std::map< int, JNIMethod* >& methods, int methodEnum, ... );
	void callJNIVoidMethod( std::map< int, JNIMethod* >& methods, int methodEnum, ... );
	jint callJNIIntMethod( std::map< int, JNIMethod* >& methods, int methodEnum, ... );

	static jobject createJNIReferenceStatic( jobject object );
	jobject createJNIReference( jobject object );
	void destroyJNIReference( jobject& object );
};
