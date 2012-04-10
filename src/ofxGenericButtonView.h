//
//  ofxGenericButtonView.h
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 12/30/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//
// TODO: OF touch event notification

#pragma once

#include "ofxGenericView.h"
#include <vector>

#if TARGET_OS_IPHONE
@class ofxGenericButtonViewForwarder;
#endif
class ofxGenericButtonViewTouchDelegate;

class ofxGenericButtonView : public ofxGenericView
{
public:
    static ofPtr< ofxGenericButtonView > create( const ofRectangle& setFrame = ofRectangle( 0, 0, 0, 0 ), ofPtrWeak< ofxGenericButtonViewTouchDelegate > touchDelegate = ofPtrWeak< ofxGenericButtonViewTouchDelegate >() );
    virtual ~ofxGenericButtonView();
    
    void setText( std::string newText );
    std::string getText();
    //    void setTextColor( const ofColor newColor );
    //    ofColor getTextColor();
    
    // void setImage
    void setBackgroundImage( std::string fileName );
    
    // TODO: scheme so that UIButtonDelegateForwarder is the only one that can call these
    // TODO: scheme so we can pass a reference of this to delegate
    virtual void touchCancel();
    virtual void touchDown();
    virtual void touchDownRepeat();
    virtual void touchDragEnter();
    virtual void touchDragExit();
    virtual void touchDragInside();
    virtual void touchDragOutside();
    virtual void touchUpInside();
    virtual void touchUpOutside();
    
    void setEnabled( bool enabled );
    bool getEnabled();
    
#if TARGET_OS_IPHONE
    operator UIButton*();
#elif TARGET_ANDROID

    static jclass getJNIClassStatic();
	virtual jclass getJNIClass();
    static const char* className;

    static void handleOnClick( int nativeID );
#endif

#if DEBUG
    virtual string toString();
#endif

protected:
    ofxGenericButtonView();
    virtual void init( ofPtrWeak< ofxGenericView > setThis, const ofRectangle& setFrame = ofRectangle( 0, 0, 0, 0 ), ofPtrWeak< ofxGenericButtonViewTouchDelegate > touchDelegate = ofPtrWeak< ofxGenericButtonViewTouchDelegate >() );

    ofPtrWeak< ofxGenericButtonViewTouchDelegate > _touchDelegate;
    
    virtual NativeView createNativeView( const ofRectangle& frame );
#if TARGET_OS_IPHONE
    ofxGenericButtonViewForwarder* _forwarder;
#elif TARGET_ANDROID
    static jclass _jniClass;

    enum ofxGenericButtonViewMethods
     {
     	JNIMethod_SetText = ofxGenericView::Last,
     	JNIMethod_GetText,
     	JNIMethod_setBackgroundImage,
     	JNIMethod_setJNITouchID,
     	JNIMethod_getJNITouchID,
     	Last
     };
     virtual void registerJNIMethods();

     static std::vector< ofxGenericButtonView* > _nativeMap;
#endif

};

class ofxGenericButtonViewTouchDelegate
{
public:
    virtual ~ofxGenericButtonViewTouchDelegate(){};
    
    virtual void button_touchCancel( ofPtr< ofxGenericButtonView > buttonView ){};
    virtual void button_touchDown( ofPtr< ofxGenericButtonView > buttonView ){};
    virtual void button_touchDownRepeat( ofPtr< ofxGenericButtonView > buttonView ){};
    virtual void button_touchDragEnter( ofPtr< ofxGenericButtonView > buttonView ){};
    virtual void button_touchDragExit( ofPtr< ofxGenericButtonView > buttonView ){};
    virtual void button_touchDragInside( ofPtr< ofxGenericButtonView > buttonView ){};
    virtual void button_touchDragOutside( ofPtr< ofxGenericButtonView > buttonView ){};
    virtual void button_touchUpInside( ofPtr< ofxGenericButtonView > buttonView ){};
    virtual void button_touchUpOutside( ofPtr< ofxGenericButtonView > buttonView ){};
};
