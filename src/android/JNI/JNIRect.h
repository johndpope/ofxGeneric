/*
 * JNIRect.h
 *
 *  Created on: Feb 21, 2012
 *      Author: Ian
 */

#pragma once

#include "JNIObject.h"

// TODO: generally profile (code and development performance) interfacing object vs work in java, basic type only interface
// TODO: caching?
class JNIRect : public JNIObject
{
public:
	JNIRect( int left, int top, int width, int height );
	JNIRect( jobject instance );
	JNIRect( const JNIRect& copy );
	virtual ~JNIRect();

    static jclass getJNIClassStatic();
	virtual jclass getJNIClass();

	void set( int left, int top, int width, int height );
	int getLeft();
	int getTop();
	int getWidth();
	int getHeight();

    jobject getJNIInstance();

	static const char* className;

protected:
    static jclass _jniClass;
    static JNIMethod* getConstructor(); // TODO: come up with inheritable scheme
	static JNIMethod* _constructor;

	jobject _instance;
    enum JNIRectMethods
    {
    	JNIMethod_Set = JNIObject::Last,
    	JNIMethod_GetLeft,
    	JNIMethod_GetTop,
    	JNIMethod_GetWidth,
    	JNIMethod_GetHeight,
    	Last
    };

    void registerMethods();
};
