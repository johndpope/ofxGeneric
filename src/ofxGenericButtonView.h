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
#if TARGET_ANDROID
#include <vector>
#endif

#if TARGET_OS_IPHONE
@class ofxGenericButtonViewForwarder;
#endif
class ofxGenericButtonViewDelegate;
class ofxGenericTextView;
class ofxGenericImage;

class ofxGenericButtonView : public ofxGenericView
{
public:
    static ofPtr< ofxGenericButtonView > create( ofxGenericButtonType buttonType = ofxGenericButtonTypeRoundedRect, const ofRectangle& setFrame = ofRectangle( 0, 0, 0, 0 ), ofPtrWeak< ofxGenericButtonViewDelegate > touchDelegate = ofPtrWeak< ofxGenericButtonViewDelegate >() );
    
    virtual ~ofxGenericButtonView();
    
    virtual void setText( std::string newText );
    virtual std::string getText();
    
    // void setImage
    void setBackgroundImage( std::string fileName );
    void setBackgroundImage( ofPtr< ofxGenericImage > image );
    void setDownBackgroundImage( std::string fileName );
    void setDownBackgroundImage( ofPtr< ofxGenericImage > image );

    virtual void setTextAlignment( ofxGenericTextHorizontalAlignment alignment );
    virtual ofxGenericTextHorizontalAlignment getTextAlignment();
    
    virtual void setContentHorizontalAlignment( ofxGenericContentHorizontalAlignment alignment );
    virtual ofxGenericContentHorizontalAlignment getContentHorizontalAlignment();
    
    virtual void setTextColor ( const ofColor& setColor );
    virtual ofColor getTextColor ();
    
    virtual void setDownTextColor ( const ofColor& setColor );
    virtual ofColor getDownTextColor ();
    
    void setLineBreakMode ( ofxGenericTextLinebreakMode mode );
    ofxGenericTextLinebreakMode getLineBreakMode();
    
    void setNumberOfLines ( int n );
    int getNumberOfLines ();
    
    virtual float getMinimumFontSize();
    virtual void setMinimumFontSize( float s );
    
    virtual void setFont ( string name, float size );
    virtual float getFontSize();
    virtual string getFontName();
    
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
    
    void setDelegate( ofPtrWeak< ofxGenericButtonViewDelegate > delegate );
    ofPtrWeak< ofxGenericButtonViewDelegate > getDelegate();

    virtual string toString();
    virtual ofPtr< ofxGenericValueStore > dumpViewGraphAsValueStore();
    virtual ofPtr< ofxGenericValueStore > toValueStore();
    
    virtual void copyProperties( ofPtr< ofxGenericView > from );

    ofRectangle getTextArea();
    
protected:
    ofxGenericButtonView();
    virtual void init( ofPtrWeak< ofxGenericView > setThis, ofxGenericButtonType buttonType = ofxGenericButtonTypeRoundedRect, const ofRectangle& setFrame = ofRectangle( 0, 0, 0, 0 ), ofPtrWeak< ofxGenericButtonViewDelegate > touchDelegate = ofPtrWeak< ofxGenericButtonViewDelegate >() );
    ofPtr< ofxGenericTextView > _textView;
    ofPtrWeak< ofxGenericButtonViewDelegate > _touchDelegate;
    
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

    ofPtr< ofxGenericImage > _backgroundImage;
    ofPtr< ofxGenericImage > _downBackgroundImage;
};

class ofxGenericButtonViewDelegate
{
public:
    virtual ~ofxGenericButtonViewDelegate(){};
    
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
