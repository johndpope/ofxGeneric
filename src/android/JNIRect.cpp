/*
 * JNIRect.cpp
 *
 *  Created on: Feb 21, 2012
 *      Author: ian
 */

#include "JNIRect.h"
#include "ofxGenericJNI.h"
#include "JNIMethod.h"
#include "JNIField.h"

jclass JNIRect::_jniClass = NULL;
JNIMethod* JNIRect::_constructor = NULL;

const char* JNIRect::className = "android/graphics/Rect";

JNIRect::JNIRect( int left, int top, int width, int height )
{
	_instance = JNIRect::getConstructor()->createInstance( NULL, left, top, width, height );//JNIObject::createJNIInstance( JNIRect::getConstructor(), left, top, width, height );
	registerMethods();
}

JNIRect::JNIRect( const JNIRect& copy )
{
	_instance = createJNIReference( copy._instance );
	registerMethods();
}


JNIRect::JNIRect( jobject instance )
{
	_instance = createJNIReference( instance );
	registerMethods();
}

JNIRect::~JNIRect()
{
	destroyJNIReference( _instance );
	if ( _constructor != NULL )
	{
		delete _constructor;
		_constructor = NULL;
	}
}

JNIMethod* JNIRect::getConstructor()
{
	if ( !_constructor )
	{
		_constructor = new JNIMethod(
				getJNIClassStatic(),
				false,
				"<init>",
				JNIEncodeMethodSignature( 4, JNIType_void, JNIType_int, JNIType_int, JNIType_int, JNIType_int )
//				JNIEncodeMethodSignature( 4, JNIType_object, JNIRect::className, JNIType_int, JNIType_int, JNIType_int, JNIType_int )
				);
	}
	return _constructor;
}

jclass JNIRect::getJNIClassStatic()
{
	// TODO: handle exception
    if ( !_jniClass )
    	_jniClass = ( jclass )JNIRect::createJNIReferenceStatic( JNIFindClass( className ) );
    return _jniClass;
}

jclass JNIRect::getJNIClass()
{
	return JNIRect::getJNIClassStatic();
}

jobject JNIRect::getJNIInstance()
{
	return _instance;
}

void JNIRect::set( int left, int top, int width, int height )
{
	callJNIVoidMethod( _jniMethods, JNIMethod_Set, left, top, ( left + width ), ( top + height ) );
}

int JNIRect::getLeft()
{
	// TODO: CACHE FIELDS YO
	JNIField left(
			getJNIClass(),
			false,
			"left",
			JNIGetBasicTypeSignatureEncoding( JNIType_int )
			);
	return left.getIntField( getJNIInstance() );
}

int JNIRect::getTop()
{
	// TODO: CACHE FIELDS YO
	JNIField top(
			getJNIClass(),
			false,
			"top",
			JNIGetBasicTypeSignatureEncoding( JNIType_int )
			);
	return top.getIntField( getJNIInstance() );
}

int JNIRect::getWidth()
{
	// TODO: CACHE FIELDS YO
	JNIMethod width(
			getJNIClass(),
			false,
			"width",
			JNIEncodeMethodSignature( 0, JNIType_int )
			);
	return width.callIntMethod( getJNIInstance() );
}

int JNIRect::getHeight()
{
	// TODO: CACHE FIELDS YO
	JNIMethod height(
			getJNIClass(),
			false,
			"height",
			JNIEncodeMethodSignature( 0, JNIType_int )
			);
	return height.callIntMethod( getJNIInstance() );
}

void JNIRect::registerMethods()
{
	registerJNIMethodID( _jniMethods,
			false,
			JNIMethod_Set,
			"set",
			JNIEncodeMethodSignature( 4, JNIType_void, JNIType_int, JNIType_int, JNIType_int, JNIType_int )
			);
}
