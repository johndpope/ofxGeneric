//
//  ofxGenericImageView.h
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 1/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericView.h"

class ofxGenericImageView : public ofxGenericView
{
public:
    void setImage( string fileName );
    
#if TARGET_OS_IPHONE
    operator UIImageView*();
#elif TARGET_ANDROID

    static jclass getJNIClassStatic();
	virtual jclass getJNIClass();
    static const char* className;

    enum ofxGenericImageViewMethods
    {
    	JNIMethod_SetImage = ofxGenericView::Last,
    	Last
    };
    virtual void registerJNIMethods();

#endif
    
protected:
    virtual NativeView createNativeView( const ofRectangle& frame );

#if TARGET_ANDROID
    static jclass _jniClass;
#endif
};
