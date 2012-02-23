#include "JNIUtility.h"

#include <stdio.h>
#include <stdarg.h>

#include "JNIObject.h"

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

std::string JNIGetBasicTypeSignatureEncoding( const JNIBasicTypeSignature& type )
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

std::string JNIGetObjectSignatureEncoding( std::string objectWithPath )
{
	std::string encoded( "L" );
	encoded += objectWithPath;
	encoded += ";";
	return encoded;
}

std::string JNIEncodeMethodSignature( int numParameters, JNIBasicTypeSignature returnType, ... )
{
	std::string encoded( "(" );
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

//	ofxGLog( OF_LOG_VERBOSE, "JNI Method Sig Encoded: built %s", encoded.c_str() );
	return encoded;
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

    return JNIUtility::JavaStringToCString( name ).c_str();
}

/*
 * jclass exccls(env->GetObjectClass(exc));
        jclass clscls(env->FindClass("java/lang/Class"));

        jmethodID getName(env->GetMethodID(clscls, "getName", "()Ljava/lang/String;"));
        jstring name(static_cast<jstring>(env->CallObjectMethod(exccls, getName)));
        char const* utfName(env->GetStringUTFChars(name, 0));

        jmethodID getMessage(env->GetMethodID(exccls, "getMessage", "()Ljava/lang/String;"));
        jstring message(static_cast<jstring>(env->CallObjectMethod(exc, getMessage)));
        char const* utfMessage(env->GetStringUTFChars(message, 0));
 */


jclass JNIFindClass( const char* className )
{
	jclass classObject = JNIObject::getJNIEnv()->FindClass( className );
	JNIHandleException();
	return classObject;
}

jstring JNIUtility::CStringToJavaString( std::string from )
{
	jstring result = JNIObject::getJNIEnv()->NewStringUTF( from.c_str() );
	JNIHandleException();
	return result;
}

std::string JNIUtility::JavaStringToCString( jstring from )
{
	jboolean isCopy;
	const char* result = JNIObject::getJNIEnv()->GetStringUTFChars( from, &isCopy );
	JNIHandleException();
	return result;
}
