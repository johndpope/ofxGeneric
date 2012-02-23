/*
 * JNIMethod.cpp
 *
 *  Created on: Feb 22, 2012
 *      Author: ian
 */

#include "JNIMethod.h"
#include "JNIObject.h"
#include "JNIUtility.h"

static const bool lateMethodID = false;

JNIMethod::JNIMethod( jclass classObject, bool isStatic, std::string name, std::string signature, bool ownReference )
{
	_ownReference = ownReference;
	if( _ownReference )
	{
		_classObject = ( jclass )JNIObject::getJNIEnv()->NewGlobalRef( classObject );
	} else
	{
		_classObject = classObject;
	}
	_isStatic = isStatic;

	_name = name;
	_signature = signature;
	_id = NULL;

	if ( !lateMethodID )
	{
		getID();
	}
}

JNIMethod::JNIMethod( jclass classObject, bool isStatic, jmethodID id, bool ownReference )
{
	if( _ownReference )
	{
		_classObject = ( jclass )JNIObject::getJNIEnv()->NewGlobalRef( classObject );
	} else
	{
		_classObject = classObject;
	}
	_isStatic = isStatic;
	_id = id;
}

JNIMethod::~JNIMethod()
{
	if ( _classObject && _ownReference )
	{
		JNIObject::getJNIEnv()->DeleteGlobalRef( _classObject );
		_classObject = NULL;
	}
}

jclass JNIMethod::getClass()
{
	return _classObject;
}

bool JNIMethod::isStatic()
{
	return _isStatic;
}

jmethodID JNIMethod::getID()
{
	if ( _id == NULL )
	{
		if ( isStatic() )
		{
			_id = JNIObject::getJNIEnv()->GetStaticMethodID( getClass(), _name.c_str(), _signature.c_str() );
		} else
		{
			_id = JNIObject::getJNIEnv()->GetMethodID( getClass(), _name.c_str(), _signature.c_str() );
		}
		JNIHandleException();
	}

	return _id;
}

std::string JNIMethod::toString()
{
	return _name + _signature;
}

/*jobject callObjectMethod( jobject instance, va_list args );
jobject callObjectMethod( jobject instance, ... );
void callVoidMethod( jobject instance, va_list args );
void callVoidMethod( jobject instance, ... );
jint callIntMethod( jobject instance, va_list args );
jint callIntMethod( jobject instance, ... );
*/
jobject JNIMethod::callObjectMethod( jobject instance, va_list args )
{
	jobject result = NULL;
	if ( isStatic() )
	{
		result = JNIObject::getJNIEnv()->CallStaticObjectMethodV( getClass(), getID(), args );
	} else
	{
		result = JNIObject::getJNIEnv()->CallObjectMethodV( instance, getID(), args );
	}

	JNIHandleException();

	return result;

}

jobject JNIMethod::callObjectMethod( jobject instance, ... )
{
	jobject result = NULL;

	va_list args;
	va_start( args, instance );
	result = callObjectMethod( instance, args );
	va_end( args );

	return result;
}


void JNIMethod::callVoidMethod( jobject instance, va_list args )
{
	if ( isStatic() )
	{
		JNIObject::getJNIEnv()->CallStaticVoidMethodV( getClass(), getID(), args );
	} else
	{
		JNIObject::getJNIEnv()->CallVoidMethodV( instance, getID(), args );
	}

	JNIHandleException();
}

void JNIMethod::callVoidMethod( jobject instance, ... )
{
	va_list args;
	va_start( args, instance );
	callVoidMethod( instance, args );
	va_end( args );
}


jint JNIMethod::callIntMethod( jobject instance, va_list args )
{
	jint result = NULL;
	if ( isStatic() )
	{
		result = JNIObject::getJNIEnv()->CallStaticIntMethodV( getClass(), getID(), args );
	} else
	{
		result = JNIObject::getJNIEnv()->CallIntMethodV( instance, getID(), args );
	}

	JNIHandleException();

	return result;

}

jint JNIMethod::callIntMethod( jobject instance, ... )
{
	jint result = NULL;

	va_list args;
	va_start( args, instance );
	result = callIntMethod( instance, args );
	va_end( args );

	return result;
}

jobject JNIMethod::createInstance( jobject instance, ... )
{
	va_list args;
	va_start( args, instance );
	jobject result = JNIObject::getJNIEnv()->NewObjectV( getClass(), getID(), args );
	va_end( args );

	JNIHandleException();

	return result;
}
