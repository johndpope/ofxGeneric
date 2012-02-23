/*
 * JNIField.cpp
 *
 *  Created on: Feb 22, 2012
 *      Author: ian
 */

#include "JNIField.h"
#include "JNIObject.h"
#include "JNIUtility.h"

static const bool lateFieldID = false;

JNIField::JNIField( jclass classObject, bool isStatic, std::string name, std::string signature, bool ownReference )
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

	if ( !lateFieldID )
	{
		getID();
	}
}

JNIField::JNIField( jclass classObject, bool isStatic, jfieldID id, bool ownReference )
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

JNIField::~JNIField()
{
	if ( _classObject && _ownReference )
	{
		JNIObject::getJNIEnv()->DeleteGlobalRef( _classObject );
		_classObject = NULL;
	}
}

jclass JNIField::getClass()
{
	return _classObject;
}

bool JNIField::isStatic()
{
	return _isStatic;
}

jfieldID JNIField::getID()
{
	if ( _id == NULL )
	{
		if ( isStatic() )
		{
			_id = JNIObject::getJNIEnv()->GetStaticFieldID( getClass(), _name.c_str(), _signature.c_str() );
		} else
		{
			_id = JNIObject::getJNIEnv()->GetFieldID( getClass(), _name.c_str(), _signature.c_str() );
		}
		JNIHandleException();
	}
	return _id;
}

std::string JNIField::toString()
{
	return _name + _signature;
}

jint JNIField::getIntField( jobject instance )
{
	jint result = NULL;
	if ( isStatic() )
	{
		result = JNIObject::getJNIEnv()->GetStaticIntField( getClass(), getID() );
	} else
	{
		result = JNIObject::getJNIEnv()->GetIntField( instance, getID() );
	}
	JNIHandleException();

	return result;
}

void JNIField::setIntField( jint value, jobject instance )
{
	if ( isStatic() )
	{
		JNIObject::getJNIEnv()->SetStaticIntField( getClass(), getID(), value );
	} else
	{
		JNIObject::getJNIEnv()->SetIntField( instance, getID(), value );
	}
	JNIHandleException();
}
