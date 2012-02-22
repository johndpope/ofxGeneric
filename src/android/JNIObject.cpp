#include "JNIObject.h"

#include "JNIMethod.h"
#include "ofxGenericApp.h"

JavaVM* JNIObject::_javaVM = NULL;

JavaVM* JNIObject::getJavaVM()
{
	return _javaVM;
}

void JNIObject::setJavaVM( JavaVM* javaVM )
{
	_javaVM = javaVM;
}

JNIEnv* JNIObject::getJNIEnv()
{
	JNIEnv* env = NULL;
	if ( _javaVM )
	{
		jint requestedVersion = JNI_VERSION_1_4;
		jint result = _javaVM->GetEnv( ( void** )&env, requestedVersion );
		if ( result == JNI_EDETACHED )
		{
			ofxGLog( OF_LOG_ERROR, "JNI: JavaVM->GetEnv(...) returned error: current thread is not attached to the VM" );
		} else if ( result == JNI_EVERSION )
		{
			ofxGLog( OF_LOG_ERROR, "JNI: JavaVM->GetEnv(...) returned error: specified version of JNI (%d) is not supported", requestedVersion );
		}
	}
	return env;
}

JNIObject::~JNIObject()
{
	for( std::map< int, JNIMethod* >::iterator deleteRefs = _jniMethods.begin(); deleteRefs != _jniMethods.end(); deleteRefs ++ )
	{
		if ( ( *deleteRefs ).second )
		{
			deleteThreadSafe( ( *deleteRefs ).second, JNIMethod* );
		}
	}
	_jniMethods.empty();
}

void JNIObject::registerJNIMethodID( std::map< int, JNIMethod* >& methods, jclass classObject, bool isStatic, int methodEnum, string methodName, string methodSignature )
{
	std::map< int, JNIMethod* >::iterator find = methods.find( methodEnum );
	if ( find != methods.end() )
	{
		ofxGLog( OF_LOG_VERBOSE, "JNI: Object: Attempting to register in map %p method %s with previously used enum %d, skipping", &methods, ( *find ).second->toString().c_str(), methodEnum );
		return;
	}

	methods[ methodEnum ] = new JNIMethod( classObject, isStatic, methodName, methodSignature );
	JNIMethod* method = methods[ methodEnum ];

	ofxGLog( OF_LOG_VERBOSE, "JNI: Object: Registered in map %p method %s with enum %d", &methods, method->toString().c_str(), ( int )methodEnum );
}

void JNIObject::registerJNIMethodID( std::map< int, JNIMethod* >& methods, bool isStatic, int methodEnum, string methodName, string methodSignature )
{
	JNIObject::registerJNIMethodID( methods, getJNIClass(), isStatic, methodEnum, methodName, methodSignature );
}

jobject JNIObject::callJNIObjectMethod( std::map< int, JNIMethod* >& methods, int methodEnum, ... )
{
	std::map< int, JNIMethod* >::const_iterator methodIt = methods.find( methodEnum );
	if ( methodIt == methods.end() )
	{
		ofxGLog( OF_LOG_ERROR, "JNI: Object: Could not find in map %p method with enum %d", &methods, ( int )methodEnum );
		return NULL;
	}

	JNIMethod* method = ( *methodIt ).second;

	va_list args;
	va_start( args, methodEnum );
	jobject result = NULL;
	jmethodID methodID = method->getID();
	if ( method->isStatic() )
	{
		jclass classObject = method->getClass();
		ofxGLog( OF_LOG_VERBOSE, "JNI: Object: Calling static method TODO::%s ", method->toString().c_str() );
		result = JNICallStaticObjectMethod( classObject, methodID, args );
	} else
	{
		jobject instance = getJNIInstance();
		ofxGLog( OF_LOG_VERBOSE, "JNI: Object: Calling instance method TODO::%s ", method->toString().c_str() );
		result = JNICallObjectMethod( instance, methodID, args );
	}
	va_end( args );

	return result;
}

void JNIObject::callJNIVoidMethod( std::map< int, JNIMethod* >& methods, int methodEnum, ... )
{
	std::map< int, JNIMethod* >::const_iterator methodIt = methods.find( methodEnum );
	if ( methodIt == methods.end() )
	{
		ofxGLog( OF_LOG_ERROR, "JNI: Object: Could not find in map %p method with enum %d", &methods, ( int )methodEnum );
		return;
	}

	JNIMethod* method = ( *methodIt ).second;

	va_list args;
	va_start( args, methodEnum );
	if ( method->isStatic() )
	{
		ofxGLog( OF_LOG_VERBOSE, "JNI: Object: Calling static method TODO::%s ", method->toString().c_str() );
		JNICallStaticVoidMethod( getJNIClass(), method->getID(), args );
	} else
	{
		ofxGLog( OF_LOG_VERBOSE, "JNI: Object: Calling instance method TODO::%s ", method->toString().c_str() );
		JNICallVoidMethod( getJNIInstance(), method->getID(), args );
	}
	va_end( args );
}

jint JNIObject::callJNIIntMethod( std::map< int, JNIMethod* >& methods, int methodEnum, ... )
{
	std::map< int, JNIMethod* >::const_iterator methodIt = methods.find( methodEnum );
	if ( methodIt == methods.end() )
	{
		ofxGLog( OF_LOG_ERROR, "JNI: Object: Could not find in map %p method with enum %d", &methods, ( int )methodEnum );
		return NULL;
	}

	JNIMethod* method = ( *methodIt ).second;

	va_list args;
	va_start( args, methodEnum );
	jint result;
	jmethodID methodID = method->getID();
	if ( method->isStatic() )
	{
		jclass classObject = method->getClass();
		ofxGLog( OF_LOG_VERBOSE, "JNI: Object: Calling static int method TODO::%s ", method->toString().c_str() );
		result = JNICallStaticIntMethod( classObject, methodID, args );
	} else
	{
		jobject instance = getJNIInstance();
		ofxGLog( OF_LOG_VERBOSE, "JNI: Object: Calling instance int method TODO::%s ", method->toString().c_str() );
		result = JNICallIntMethod( instance, methodID, args );
	}
	va_end( args );

	return result;
}

jobject JNIObject::createJNIReferenceStatic( jobject object )
{
	jobject reference = getJNIEnv()->NewGlobalRef( object );
	JNIHandleException();
	return reference;
}

jobject JNIObject::createJNIReference( jobject object )
{
	return JNIObject::createJNIReferenceStatic( object );
}

void JNIObject::destroyJNIReference( jobject& object )
{
	if ( object )
	{
		JNIObject::getJNIEnv()->DeleteGlobalRef( object );
		object = NULL;
	}
}

jobject JNIObject::createJNIInstance( JNIMethod* constructor, ... )
{
	va_list args;
	va_start( args, constructor );
	jobject result = constructor->createInstance( NULL );
	va_end( args );
	return result;
}

jobject JNIObject::createJNIInstance( std::map< int, JNIMethod* >& methods, int methodEnum, ... )
{
	std::map< int, JNIMethod* >::const_iterator methodIt = methods.find( methodEnum );
	if ( methodIt == methods.end() )
	{
		ofxGLog( OF_LOG_ERROR, "JNI: Object: Could not find in map %p constructor with enum %d", &methods, ( int )methodEnum );
		return NULL;
	}

	JNIMethod* constructor = ( *methodIt ).second;
	va_list args;
	va_start( args, methodEnum );
	jobject result = constructor->createInstance( NULL );
	va_end( args );
	return result;
}


