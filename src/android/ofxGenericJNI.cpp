#include "ofxGenericJNI.h"
#include "JNIObject.h"

#include "android/log.h"
#include "ofxAndroidLogChannel.h"

#include "ofxGenericApp.h"
#include "ofxGenericUtility.h"
#include "JNIUtility.h"

static void noopDeleter( ofBaseLoggerChannel* ){}
void SetupJNI( JNIEnv* env, jobject thiz )
{
	__android_log_print( ANDROID_LOG_VERBOSE, ofxGenericModuleName, "Switching ofLog over to Android" );
	ofLog::setChannel( ofPtr< ofxAndroidLogChannel >( new ofxAndroidLogChannel(), std::ptr_fun( noopDeleter ) ) );

	JavaVM* javaVM = NULL;
	if ( env->GetJavaVM( &javaVM ) == JNI_OK )
	{
		JNIObject::setJavaVM( javaVM );
	}

	ofxGLog( OF_LOG_VERBOSE, "JNI: VM %p Env %p", javaVM, env );
}

extern "C"
{
	void Java_cc_openframeworks_ofxGeneric_Activity_SetupJNI( JNIEnv* env, jobject thiz )
	{
		SetupJNI( env, thiz );
	}
}

// http://docs.oracle.com/javase/1.5.0/docs/guide/jni/spec/types.html#wp276
// http://www.ibm.com/developerworks/java/tutorials/j-jni/resources.html
// http://www.ibm.com/developerworks/java/tutorials/j-jni/section6.html
// http://java.sun.com/docs/books/jni/html/invoke.html
// debug
// http://www.eclipse.org/sequoyah/documentation/native_debug.php
// http://mhandroid.wordpress.com/2011/01/23/using-eclipse-for-android-cc-debugging/#more-23
// http://wiki.eclipse.org/Sequoyah/ndk_guide

jmethodID JNIGetMethodID( jclass classObject, bool isStatic, const char* methodName, const char* methodSignature )
{
	jmethodID methodID;
	if ( isStatic )
	{
		ofxGLog( OF_LOG_VERBOSE, "JNI: Attempting to get method ID for static method ::%s%s", methodName, methodSignature );
		methodID = JNIObject::getJNIEnv()->GetStaticMethodID( classObject, methodName, methodSignature );
	} else
	{
		ofxGLog( OF_LOG_VERBOSE, "JNI: Attempting to get method ID for instance method ::%s%s", methodName, methodSignature );
		methodID = JNIObject::getJNIEnv()->GetMethodID( classObject, methodName, methodSignature );
	}
	JNIHandleException();
	return methodID;
}

jobject JNICallStaticObjectMethod( jclass classObject, jmethodID methodID, va_list args )
{
	jobject	result = JNIObject::getJNIEnv()->CallStaticObjectMethodV( classObject, methodID, args );
	JNIHandleException();
	return result;
}

jobject JNICallStaticObjectMethod( jclass classObject, jmethodID methodID, ... )
{
	va_list args;
	va_start( args, methodID );
	jobject result = JNICallStaticObjectMethod( classObject, methodID, args );
	va_end( args );
	return result;
}

jobject JNICallStaticObjectMethod( const char* className, const char* methodName, const char* methodSig, ... )
{
	ofxGLog( OF_LOG_VERBOSE, "JNI: Attempting to call %s::%s%s", className, methodName, methodSig );
	try
	{
		jclass classObject = JNIFindClass( className );
		jmethodID methodID = JNIGetMethodID( classObject, true, methodName, methodSig );

		va_list args;
		va_start( args, methodSig );
		jobject result = JNICallStaticObjectMethod( classObject, methodID, args );
		va_end( args );

		return result;
	} catch( std::exception& e )
	{
		ofxGLog( OF_LOG_WARNING, "Exception when calling %s::%s%s - %s", className, methodName, methodSig, e.what() );
	}
	return NULL;
}

jobject JNICallObjectMethod( jobject objectInstance, jmethodID methodID, va_list args )
{
	jobject result = JNIObject::getJNIEnv()->CallObjectMethodV( objectInstance, methodID, args );
	JNIHandleException();
	return result;
}

jobject JNICallObjectMethod( jobject objectInstance, jmethodID methodID, ... )
{
	va_list args;
	va_start( args, methodID );
	jobject result = JNIObject::getJNIEnv()->CallObjectMethodV( objectInstance, methodID, args );
	va_end( args );
	JNIHandleException();
	return result;
}

void JNICallStaticVoidMethod( jclass classObject, jmethodID methodID, va_list args )
{
	JNIObject::getJNIEnv()->CallStaticVoidMethodV( classObject, methodID, args );
	JNIHandleException();
}

void JNICallStaticVoidMethod( jclass classObject, jmethodID methodID, ... )
{
	va_list args;
	va_start( args, methodID );
	JNICallStaticVoidMethod( classObject, methodID, args );
	JNIHandleException();
	va_end( args );
}

void JNICallStaticVoidMethod( const char* className, const char* methodName, const char* methodSig, ... )
{
	ofxGLog( OF_LOG_VERBOSE, "JNI: Attempting to call %s::%s%s", className, methodName, methodSig );
	try
	{
		jclass classObject = JNIFindClass( className );
		jmethodID methodID = JNIGetMethodID( classObject, true, methodName, methodSig );

		va_list args;
		va_start( args, methodSig );
		JNICallStaticVoidMethod( classObject, methodID, args );
		va_end( args );
	} catch( std::exception& e )
	{
		ofxGLog( OF_LOG_WARNING, "Exception when calling %s::%s%s - %s", className, methodName, methodSig, e.what() );
	}
}

void JNICallVoidMethod( jobject objectInstance, jmethodID methodID, va_list args )
{
	JNIObject::getJNIEnv()->CallVoidMethodV( objectInstance, methodID, args );
	JNIHandleException();
}

jint JNICallIntMethod( jobject objectInstance, jmethodID methodID, va_list args )
{
	jint result = JNIObject::getJNIEnv()->CallIntMethodV( objectInstance, methodID, args );
	JNIHandleException();
	return result;
}

jint JNICallStaticIntMethod( jclass classObject, jmethodID methodID, va_list args )
{
	jint result = JNIObject::getJNIEnv()->CallStaticIntMethodV( classObject, methodID, args );
	JNIHandleException();
	return result;
}

void JNIDeleteLocalRef( jobject object )
{
	JNIObject::getJNIEnv()->DeleteLocalRef( object );
}

