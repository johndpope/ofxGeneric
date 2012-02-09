/*
 * ofxGenericUtility.cpp
 *
 *  Created on: Feb 8, 2012
 *      Author: ian
 */

#include <jni.h>
#include "ofLog.h"
#include "ofxAndroidLogChannel.h"
#include "android/log.h"

extern "C"
{
	static void noopDeleter( ofBaseLoggerChannel* ){}
	void Java_cc_openframeworks_ofxGeneric_Activity_SetupLog( JNIEnv* env, jobject thiz )
	{
		__android_log_print( ANDROID_LOG_VERBOSE, "OF", "Switching ofLog over to Android" );
		ofLog::setChannel( ofPtr< ofxAndroidLogChannel >( new ofxAndroidLogChannel(), std::ptr_fun( noopDeleter ) ) );
	}
}
