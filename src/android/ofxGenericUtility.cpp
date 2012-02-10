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
