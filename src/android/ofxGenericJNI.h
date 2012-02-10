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

inline jstring JNICStringToJavaString( const char string[] );
inline const char* JNIJavaStringToCString( jstring string );

inline void JNIHandleException();

inline const char* JNIGetClassName( jobject object );
inline jclass JNIFindClass( const char* className );

inline jmethodID JNIGetMethodID( jclass classObject, bool isStatic, const char* methodName, const char* methodSignature );

inline jobject JNICallObjectMethod( jclass classObject, bool isStatic, jmethodID methodID, va_list args );
jobject JNICallObjectMethod( const char* className, const char* methodName, const char* methodSig, ... );

inline void JNICallVoidMethod( jclass classObject, bool isStatic, jmethodID methodID, va_list args );
jobject JNICallVoidMethod( bool isStatic, const char* className, const char* methodName, const char* methodSig, ... );
