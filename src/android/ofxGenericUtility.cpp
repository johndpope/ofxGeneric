/*
 * ofxGenericUtility.cpp
 *
 *  Created on: Feb 8, 2012
 *      Author: ian
 */

#include "ofxGenericUtility.h"
#include "ofLog.h"
#include "ofxAndroidLogChannel.h"
#include "ofxGenericConstants.h"
#include "ofxGenericApp.h"

// http://www.ibm.com/developerworks/java/tutorials/j-jni/section6.html

inline jstring JNICStringToJavaString( JNIEnv* env,const char string[] )
{
    // translate c string to java string
    return env->NewStringUTF( string );
}

inline const char* JNIJavaStringToCString( JNIEnv* env, jstring string )
{
    jboolean copy;
    return env->GetStringUTFChars( string, &copy );
}

inline const char* JNIGetClassName( JNIEnv* env, jobject object )
{
    jclass jClass = env->GetObjectClass( object );
    if ( !jClass )
    	return NULL;
    jmethodID method = env->GetMethodID( jClass, "getName", "()Ljava/lang/String" );
    if ( !method )
    	return NULL;
    jstring name = ( jstring )env->CallObjectMethod( jClass, method );
    if ( !name )
    	return NULL;

    return JNIJavaStringToCString( env, name );
}

inline void JNIHandleException( JNIEnv* env )
{
	jthrowable exception = env->ExceptionOccurred();
	if ( exception )
	{
        env->ExceptionDescribe();
		env->ExceptionClear();

		const char* exceptionClassName = JNIGetClassName( env, exception );
		if ( !exceptionClassName ) // TODO: fix, doesn't seem to ever be working, will remove need for describe
		{
	    	// TODO: handle gracefully but don't let go unnoticed!
			env->ExceptionClear();
			exceptionClassName = "Unknown exception";
		}
		env->DeleteLocalRef( exception );
		throw JNIException( exceptionClassName );
	}
}

inline jclass JNIFindClass( JNIEnv* env, const char* className )
{
	jclass classObject = env->FindClass( className );
	JNIHandleException( env );
	return classObject;
}

inline jmethodID JNIGetStaticMethodID( JNIEnv* env, jclass classObject, const char* methodName, const char* methodSignature )
{
	jmethodID methodID = env->GetStaticMethodID( classObject, methodName, methodSignature );
	JNIHandleException( env );
	return methodID;
}

jobject JNICallClassStaticObjectMethod( const char* className, const char* methodName, const char* methodSig )
{
	ofxGLog( OF_LOG_VERBOSE, "JNI: Attempting to call %s::%s %s", className, methodName, methodSig );
	try
	{
		JNIEnv* env = ofxGenericApp::jniEnv;
		jclass classObject = JNIFindClass( env, className );
		ofxGLog( OF_LOG_VERBOSE, "Found class" );
		jmethodID methodID = JNIGetStaticMethodID( env, classObject, methodName, methodSig );
    	ofxGLog( OF_LOG_VERBOSE, "Found method" );
		jobject result = env->CallStaticObjectMethod( classObject, methodID );
		env->DeleteLocalRef( classObject );
		return result;
	} catch( std::exception& e )
	{
		ofxGLog( OF_LOG_WARNING, "Exception when calling %s::%s %s - %s", className, methodName, methodSig, e.what() );
	}
	return NULL;
}

#include "android/log.h"
static void noopDeleter( ofBaseLoggerChannel* ){}
void SetupJNI( JNIEnv* env, jobject thiz )
{
	__android_log_print( ANDROID_LOG_VERBOSE, ofxGenericModuleName, "Switching ofLog over to Android" );
	ofLog::setChannel( ofPtr< ofxAndroidLogChannel >( new ofxAndroidLogChannel(), std::ptr_fun( noopDeleter ) ) );

	ofxGenericApp::jniEnv = env;
	ofxGLog( OF_LOG_VERBOSE, "In SetupJNI(): %d", env );
}

extern "C"
{
	void Java_cc_openframeworks_ofxGeneric_Activity_SetupJNI( JNIEnv* env, jobject thiz )
	{
		SetupJNI( env, thiz );
	}
}
