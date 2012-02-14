#include "ofxGenericConstants.h"
#include "ofMain.h"

#include <jni.h>

#include <exception>

class JNIException : public std::exception
{
public:
	JNIException( string message ) : _what( message ) {}
	~JNIException() throw() {}
	const char* what() const throw() { return _what.c_str(); }

private:
	string _what;
};

jstring JNICStringToJavaString( const char string[] );
const char* JNIJavaStringToCString( jstring string );

void JNIHandleException();

const char* JNIGetClassName( jobject object );
jclass JNIFindClass( const char* className );

jmethodID JNIGetMethodID( jclass classObject, bool isStatic, const char* methodName, const char* methodSignature );

jobject JNICallObjectMethod( jclass classObject, bool isStatic, jmethodID methodID, va_list args );
jobject JNICallObjectMethod( jclass classObject, bool isStatic, jmethodID methodID, ... );
jobject JNICallObjectMethod( bool isStatic, const char* className, const char* methodName, const char* methodSig, ... );

void JNICallVoidMethod( jclass classObject, bool isStatic, jmethodID methodID, va_list args );
jobject JNICallVoidMethod( bool isStatic, const char* className, const char* methodName, const char* methodSig, ... );

void JNIDeleteLocalRef( jobject object );


enum JNIBasicTypeSignature
{
	JNIType_bool,
	JNIType_byte,
	JNIType_char,
	JNIType_short,
	JNIType_int,
	JNIType_long,
	JNIType_float,
	JNIType_double,
	JNIType_array,
	JNIType_void,
	JNIType_object
};

const char* JNIGetBasicTypeSignatureEncoding( const JNIBasicTypeSignature& type );
const char* JNIGetObjectSignatureEncoding( const char* objectWithPath );
const char* JNIEncodeMethodSignature( int numParameters, JNIBasicTypeSignature returnType, ... );
