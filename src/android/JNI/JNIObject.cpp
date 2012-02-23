#include "JNIObject.h"
#include "JNIMethod.h"
#include "JNIUtility.h"

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
		// TODO: exceptions
		if ( result == JNI_EDETACHED )
		{
//			ofxGLog( OF_LOG_ERROR, "JNI: JavaVM->GetEnv(...) returned error: current thread is not attached to the VM" );
		} else if ( result == JNI_EVERSION )
		{
//			ofxGLog( OF_LOG_ERROR, "JNI: JavaVM->GetEnv(...) returned error: specified version of JNI (%d) is not supported", requestedVersion );
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
			JNIMethod* deleteMe = ( *deleteRefs ).second;
			( *deleteRefs ).second = NULL;
			delete deleteMe;
		}
	}
	_jniMethods.empty();
}

JNIMethod* JNIObject::getJNIMethod( const std::map< int, JNIMethod* >& methods, int methodEnum )
{
	std::map< int, JNIMethod* >::const_iterator find = methods.find( methodEnum );
	if ( find != methods.end() )
	{
		return ( *find ).second;
	}
	return NULL;
}

void JNIObject::registerJNIMethodID( std::map< int, JNIMethod* >& methods, jclass classObject, bool isStatic, int methodEnum, std::string methodName, std::string methodSignature )
{
	if ( JNIObject::getJNIMethod( methods, methodEnum ) != NULL )
	{
		// TODO: exception?
//		ofxGLog( OF_LOG_VERBOSE, "JNI: Object: Attempting to register in map %p method %s with previously used enum %d, skipping", &methods, ( *find ).second->toString().c_str(), methodEnum );
		return;
	}

	methods[ methodEnum ] = new JNIMethod( classObject, isStatic, methodName, methodSignature );
	//JNIMethod* method = methods[ methodEnum ];

//	ofxGLog( OF_LOG_VERBOSE, "JNI: Object: Registered in map %p method %s with enum %d", &methods, method->toString().c_str(), ( int )methodEnum );
}

void JNIObject::registerJNIMethodID( std::map< int, JNIMethod* >& methods, bool isStatic, int methodEnum, std::string methodName, std::string methodSignature )
{
	JNIObject::registerJNIMethodID( methods, getJNIClass(), isStatic, methodEnum, methodName, methodSignature );
}

jobject JNIObject::callJNIObjectMethod( std::map< int, JNIMethod* >& methods, int methodEnum, ... )
{
	JNIMethod* method = JNIObject::getJNIMethod( methods, methodEnum );
	if ( method == NULL )
	{
		// TODO: exception
//		ofxGLog( OF_LOG_ERROR, "JNI: Object: Could not find in map %p method with enum %d", &methods, ( int )methodEnum );
		return NULL;
	}

	va_list args;
	va_start( args, methodEnum );
	jobject result = method->callObjectMethod( getJNIInstance(), args );
	va_end( args );

	return result;
}

void JNIObject::callJNIVoidMethod( std::map< int, JNIMethod* >& methods, int methodEnum, ... )
{
	JNIMethod* method = JNIObject::getJNIMethod( methods, methodEnum );
	if ( method == NULL )
	{
		// TODO: exception
//		ofxGLog( OF_LOG_ERROR, "JNI: Object: Could not find in map %p method with enum %d", &methods, ( int )methodEnum );
		return;
	}

	va_list args;
	va_start( args, methodEnum );
	method->callVoidMethod( getJNIInstance(), args );
	va_end( args );
}

jint JNIObject::callJNIIntMethod( std::map< int, JNIMethod* >& methods, int methodEnum, ... )
{
	JNIMethod* method = JNIObject::getJNIMethod( methods, methodEnum );
	if ( method == NULL )
	{
		// TODO: exception
//		ofxGLog( OF_LOG_ERROR, "JNI: Object: Could not find in map %p method with enum %d", &methods, ( int )methodEnum );
		return NULL;
	}

	va_list args;
	va_start( args, methodEnum );
	jint result = method->callIntMethod( getJNIInstance(), args );
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
		JNIHandleException();
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
	JNIMethod* constructor = JNIObject::getJNIMethod( methods, methodEnum );
	if ( constructor == NULL )
	{
		// TODO: exception
//		ofxGLog( OF_LOG_ERROR, "JNI: Object: Could not find in map %p constructor with enum %d", &methods, ( int )methodEnum );
		return NULL;
	}

	va_list args;
	va_start( args, methodEnum );
	jobject result = constructor->createInstance( NULL );
	va_end( args );
	return result;
}


