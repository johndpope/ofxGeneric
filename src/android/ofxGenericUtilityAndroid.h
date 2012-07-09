//
//  ofxGenericUtilityAndroid.h
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 12/29/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"

#include "JNIRect.h"

JNIRect ofRectangleToJNIRect( const ofRectangle& from );
ofRectangle JNIRectToofRectangle( JNIRect& from );

jint ofColorToJNIColor( const ofColor& from );
ofColor JNIColorToofColor( const jint& from );