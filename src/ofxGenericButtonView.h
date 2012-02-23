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
@class UIButtonDelegateForwarder;
#endif
class ofxGenericButtonViewTouchDelegate;

class ofxGenericButtonView : public ofxGenericView
{
public:
    ofxGenericButtonView();
    virtual ~ofxGenericButtonView();
    
    virtual void init( ofPtrWeak< ofxGenericView > setThis, const ofRectangle& setBounds = ofRectangle( 0, 0, 0, 0 ), ofPtrWeak< ofxGenericButtonViewTouchDelegate > touchDelegate = ofPtrWeak< ofxGenericButtonViewTouchDelegate >() );
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
    
#if TARGET_OS_IPHONE
    operator UIButton*();
#elif TARGET_ANDROID

    static jclass getJNIClassStatic();
	virtual jclass getJNIClass();
    static const char* className;

    static void handleOnClick( int nativeID );
#endif
    
protected:
    ofPtrWeak< ofxGenericButtonViewTouchDelegate > _touchDelegate;
    
    virtual NativeView createNativeView( const ofRectangle& frame );
#if TARGET_OS_IPHONE
    UIButtonDelegateForwarder* _eventHandler;
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
    
    virtual void button_touchCancel(){};
    virtual void button_touchDown(){};
    virtual void button_touchDownRepeat(){};
    virtual void button_touchDragEnter(){};
    virtual void button_touchDragExit(){};
    virtual void button_touchDragInside(){};
    virtual void button_touchDragOutside(){};
    virtual void button_touchUpInside(){};
    virtual void button_touchUpOutside(){};
};

#if TARGET_OS_IPHONE
@interface UIButtonDelegateForwarder : NSObject 
{
@private
    ofxGenericButtonView* _delegate;
}
-( void )setDelegate:( ofxGenericButtonView* )setDelegate;

-( void )touchCancel:( id )sender;
-( void )touchDown:( id )sender;
-( void )touchDownRepeat:( id )sender;
-( void )touchDragEnter:( id )sender;
-( void )touchDragExit:( id )sender;
-( void )touchDragInside:( id )sender;
-( void )touchDragOutside:( id )sender;
-( void )touchUpInside:( id )sender;
-( void )touchUpOutside:( id )sender;

@end
#elif TARGET_ANDROID

#endif
