//
//  ofxGenericImageView.h
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 1/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericView.h"

class ofxGenericImage;

class ofxGenericImageView : public ofxGenericView
{
public:
    static ofPtr< ofxGenericImageView > create( const ofRectangle& setFrame = ofRectangle( 0, 0, 0, 0 ), string fileName = string() );
    
    virtual void setImage( string fileName );
    virtual void setImage( ofPtr< ofImage > image );
    virtual void setImage ( ofPtr< ofxGenericImage > image );
    
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
    virtual void init( ofPtrWeak< ofxGenericImageView > setThis, const ofRectangle& setFrame, string fileName = string() );
    ofxGenericImageView();
    virtual NativeView createNativeView( const ofRectangle& frame );

    
    
    ofPtr< ofxGenericImage > _image;
    
#if TARGET_ANDROID
    static jclass _jniClass;
#endif
};
