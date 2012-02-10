#include "ofxGenericJNI.h"

#include "ofxGenericConstants.h"

#include "android/log.h"
#include "ofxAndroidLogChannel.h"

#include "ofxGenericApp.h"
#include "ofxGenericUtility.h"

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

// http://docs.oracle.com/javase/1.5.0/docs/guide/jni/spec/types.html#wp276

inline jstring JNICStringToJavaString( const char string[] )
{
    // translate c string to java string
    return ofxGenericApp::jniEnv->NewStringUTF( string );
}

inline const char* JNIJavaStringToCString( JNIEnv* env, jstring string )
{
    jboolean copy;
    return ofxGenericApp::jniEnv->GetStringUTFChars( string, &copy );
}

inline const char* JNIGetClassName( jobject object )
{
    jclass jClass = ofxGenericApp::jniEnv->GetObjectClass( object );
    if ( !jClass )
    	return NULL;
    jmethodID method = ofxGenericApp::jniEnv->GetMethodID( jClass, "getName", "()Ljava/lang/String" );
    if ( !method )
    	return NULL;
    jstring name = ( jstring )ofxGenericApp::jniEnv->CallObjectMethod( jClass, method );
    if ( !name )
    	return NULL;

    return JNIJavaStringToCString( ofxGenericApp::jniEnv, name );
}

inline void JNIHandleException()
{
	jthrowable exception = ofxGenericApp::jniEnv->ExceptionOccurred();
	if ( exception )
	{
		ofxGenericApp::jniEnv->ExceptionDescribe();
		ofxGenericApp::jniEnv->ExceptionClear();

		const char* exceptionClassName = JNIGetClassName( exception );
		if ( !exceptionClassName ) // TODO: fix, doesn't seem to ever be working, will remove need for describe and allow us to mirror with explicit C++ exception classes
		{
	    	// TODO: handle gracefully but don't let go unnoticed!
			ofxGenericApp::jniEnv->ExceptionClear();
			exceptionClassName = "Unknown exception";
		}
		ofxGenericApp::jniEnv->DeleteLocalRef( exception );
		throw JNIException( exceptionClassName );
	}
}

inline jclass JNIFindClass( const char* className )
{
	jclass classObject = ofxGenericApp::jniEnv->FindClass( className );
	JNIHandleException();
	return classObject;
}

inline jmethodID JNIGetMethodID( jclass classObject, bool isStatic, const char* methodName, const char* methodSignature )
{
	jmethodID methodID;
	if ( isStatic )
	{
		methodID = ofxGenericApp::jniEnv->GetStaticMethodID( classObject, methodName, methodSignature );
	} else
	{
		methodID = ofxGenericApp::jniEnv->GetMethodID( classObject, methodName, methodSignature );
	}
	JNIHandleException();
	return methodID;
}

inline jobject JNICallObjectMethod( jclass classObject, bool isStatic, jmethodID methodID, va_list args )
{
	jobject result;
	if ( isStatic )
	{
		result = ofxGenericApp::jniEnv->CallStaticObjectMethodV( classObject, methodID, args );
	} else
	{
		result = ofxGenericApp::jniEnv->CallObjectMethodV( classObject, methodID, args );
	}
	JNIHandleException();
	return result;
}

jobject JNICallObjectMethod( bool isStatic, const char* className, const char* methodName, const char* methodSig, ... )
{
	ofxGLog( OF_LOG_VERBOSE, "JNI: Attempting to call %s::%s%s", className, methodName, methodSig );
	try
	{
		jclass classObject = JNIFindClass( className );
		jmethodID methodID = JNIGetMethodID( classObject, isStatic, methodName, methodSig );

		va_list args;
		va_start( args, methodSig );
		jobject result = JNICallObjectMethod( classObject, isStatic, methodID, args );
		va_end( args );

		return result;
	} catch( std::exception& e )
	{
		ofxGLog( OF_LOG_WARNING, "Exception when calling %s::%s %s - %s", className, methodName, methodSig, e.what() );
	}
	return NULL;
}

inline void JNICallVoidMethod( jclass classObject, bool isStatic, jmethodID methodID, va_list args )
{
	if ( isStatic )
	{
		ofxGenericApp::jniEnv->CallStaticVoidMethodV( classObject, methodID, args );
	} else
	{
		ofxGenericApp::jniEnv->CallVoidMethodV( classObject, methodID, args );
	}
	JNIHandleException();
}

jobject JNICallVoidMethod( bool isStatic, const char* className, const char* methodName, const char* methodSig, ... )
{
	ofxGLog( OF_LOG_VERBOSE, "JNI: Attempting to call %s::%s %s", className, methodName, methodSig );
	try
	{
		jclass classObject = JNIFindClass( className );
		jmethodID methodID = JNIGetMethodID( classObject, isStatic, methodName, methodSig );

		va_list args;
		va_start( args, methodSig );
		JNICallVoidMethod( classObject, isStatic, methodID, args );
		va_end( args );
	} catch( std::exception& e )
	{
		ofxGLog( OF_LOG_WARNING, "Exception when calling %s::%s %s - %s", className, methodName, methodSig, e.what() );
	}
	return NULL;
}
