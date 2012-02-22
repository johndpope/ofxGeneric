#include "ofxGenericJNI.h"
#include "JNIObject.h"

#include "android/log.h"
#include "ofxAndroidLogChannel.h"

#include "ofxGenericApp.h"
#include "ofxGenericUtility.h"

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

jstring JNICStringToJavaString( const char string[] )
{
    // translate c string to java string
    return JNIObject::getJNIEnv()->NewStringUTF( string );
}

const char* JNIJavaStringToCString( JNIEnv* env, jstring string )
{
    jboolean copy;
    return JNIObject::getJNIEnv()->GetStringUTFChars( string, &copy );
}

const char* JNIGetClassName( jobject object )
{
    jclass jClass = JNIObject::getJNIEnv()->GetObjectClass( object );
    if ( !jClass )
    	return NULL;
    jmethodID method = JNIObject::getJNIEnv()->GetMethodID( jClass, "getName", "()Ljava/lang/String" );
    if ( !method )
    	return NULL;
    jstring name = ( jstring )JNIObject::getJNIEnv()->CallObjectMethod( jClass, method );
    if ( !name )
    	return NULL;

    return JNIJavaStringToCString( JNIObject::getJNIEnv(), name );
}

void JNIHandleException()
{
	jthrowable exception = JNIObject::getJNIEnv()->ExceptionOccurred();
	if ( exception )
	{
		JNIObject::getJNIEnv()->ExceptionDescribe();
		JNIObject::getJNIEnv()->ExceptionClear();

		const char* exceptionClassName = JNIGetClassName( exception );
		if ( !exceptionClassName ) // TODO: fix, doesn't seem to ever be working, will remove need for describe and allow us to mirror with explicit C++ exception classes
		{
	    	// TODO: handle gracefully but don't let go unnoticed!
			JNIObject::getJNIEnv()->ExceptionClear();
			exceptionClassName = "Unknown exception";
		}
		JNIObject::getJNIEnv()->DeleteLocalRef( exception );
		throw JNIException( exceptionClassName );
	}
}

jclass JNIFindClass( const char* className )
{
	jclass classObject = JNIObject::getJNIEnv()->FindClass( className );
	JNIHandleException();
	return classObject;
}

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

string JNIGetBasicTypeSignatureEncoding( const JNIBasicTypeSignature& type )
{
	switch( type )
	{
		// TODO: map external to function so others can use
		case JNIType_bool:
			return "Z";
		case JNIType_byte:
			return "B";
		case JNIType_char:
			return "C";
		case JNIType_short:
			return "S";
		case JNIType_int:
			return "I";
		case JNIType_long:
			return "J";
		case JNIType_float:
			return "F";
		case JNIType_double:
			return "D";
		case JNIType_array:
			return "[";
		case JNIType_void:
			return "V";
		case JNIType_object:
			return "L%s;";
	}
	return "";
}

string JNIGetObjectSignatureEncoding( const char* objectWithPath )
{
	string encoded( "L" );
	encoded += objectWithPath;
	encoded += ";";
	return encoded;
}

string JNIEncodeMethodSignature( int numParameters, JNIBasicTypeSignature returnType, ... )
{
	string encoded( "(" );
	va_list parameterTypePointer;
	va_start( parameterTypePointer, returnType );

    JNIBasicTypeSignature arrayReturnType = JNIType_void;
	const char* returnTypeObjectWithPath = NULL;
    if ( returnType == JNIType_array )
    {
        arrayReturnType = ( JNIBasicTypeSignature )va_arg( parameterTypePointer, int ); //annoying warning JNIBasicTypeSignature );
//    	ofxGLog( OF_LOG_VERBOSE, "JNI Method Sig Encode: return array type %d", ( int )arrayReturnType );
    }
    if ( returnType == JNIType_object || ( returnType == JNIType_array && arrayReturnType == JNIType_object ) )
	{
		returnTypeObjectWithPath = va_arg( parameterTypePointer, char* );
//    	ofxGLog( OF_LOG_VERBOSE, "JNI Method Sig Encode: return object type %s", returnTypeObjectWithPath );
	}

    for( int travParameters = 0; travParameters < numParameters; travParameters ++ )
    {
        JNIBasicTypeSignature parameterType = ( JNIBasicTypeSignature )va_arg( parameterTypePointer, int ); //annoying warning JNIBasicTypeSignature );
        if ( parameterType == JNIType_array )
        {
            encoded += JNIGetBasicTypeSignatureEncoding( JNIType_array );
            parameterType = ( JNIBasicTypeSignature )va_arg( parameterTypePointer, int ); //annoying warning JNIBasicTypeSignature );
//        	ofxGLog( OF_LOG_VERBOSE, "JNI Method Sig Encode: parameter %d array type %d - built: %s", travParameters, ( int )parameterType, encoded.c_str() );
        }
		if( parameterType == JNIType_object )
		{
			const char* objectWithPath = va_arg( parameterTypePointer, char* );
            encoded += JNIGetObjectSignatureEncoding( objectWithPath );
//        	ofxGLog( OF_LOG_VERBOSE, "JNI Method Sig Encode: parameter %d object type %s - built: %s", travParameters, objectWithPath, encoded.c_str() );
		} else
		{
			encoded += JNIGetBasicTypeSignatureEncoding( parameterType );
//        	ofxGLog( OF_LOG_VERBOSE, "JNI Method Sig Encode: parameter %d basic type %d - built: %s", travParameters, ( int )parameterType, encoded.c_str() );
		}
	}
	encoded += ")";

    if ( returnType == JNIType_array )
    {
        encoded += JNIGetBasicTypeSignatureEncoding( JNIType_array );
        returnType = arrayReturnType;
//    	ofxGLog( OF_LOG_VERBOSE, "JNI Method Sig Encode: return array type - built: %s", encoded.c_str() );
    }
	if( returnType == JNIType_object )
	{
		encoded += JNIGetObjectSignatureEncoding( returnTypeObjectWithPath );
//    	ofxGLog( OF_LOG_VERBOSE, "JNI Method Sig Encode: return object type %s - built: %s", returnTypeObjectWithPath, encoded.c_str() );
	} else
	{
		encoded += JNIGetBasicTypeSignatureEncoding( returnType );
//    	ofxGLog( OF_LOG_VERBOSE, "JNI Method Sig Encode: return basic type %d - built: %s", ( int )returnType, encoded.c_str() );
	}

	va_end( parameterTypePointer );

	ofxGLog( OF_LOG_VERBOSE, "JNI Method Sig Encoded: built %s", encoded.c_str() );
	return encoded;
}

