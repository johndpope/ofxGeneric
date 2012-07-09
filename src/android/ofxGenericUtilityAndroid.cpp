/*
 * ofxGenericUtilityAndroid.cpp
 *
 *  Created on: Feb 8, 2012
 *      Author: ian
 */

#include "ofxGenericUtility.h"

JNIRect ofRectangleToJNIRect( const ofRectangle& from )
{
	return JNIRect( from.x, from.y, from.width, from.height );
}

ofRectangle JNIRectToofRectangle( JNIRect& from )
{
	ofRectangle to;
	to.set( from.getLeft(), from.getTop(), from.getWidth(), from.getHeight() );
	return to;
}

jint ofColorToJNIColor( const ofColor& from )
{
    return ( from.a << 24 ) | ( from.r << 16 ) | ( from.g << 8 ) | from.b;
}

ofColor JNIColorToofColor( const jint& from )
{
	return ofColor(
			( float )( ( from >> 16 ) & 0xFF ) / 255.0f,
			( float )( ( from >> 8 ) & 0xFF ) / 255.0f,
			( float )( from & 0xFF ) / 255.0f,
			( float )( ( from >> 24 ) & 0xFF ) / 255.0f
			);
}


