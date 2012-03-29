//
//  ofxGenericView.h
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 12/29/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "ofxGenericConstants.h"
#if TARGET_ANDROID
#include "JNIObject.h"
#endif

#include <list>

#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>

@class ofxUIGenericViewController;
#endif

class ofxGenericViewDelegate;

class ofxGenericView
#if TARGET_ANDROID
: public JNIObject
#endif
{
public:
    static ofPtr< ofxGenericView > create( const ofRectangle& setFrame = ofRectangle( 0, 0, 0, 0 ) );
    virtual ~ofxGenericView();
    
    NativeView getNativeView();
    operator NativeView();
#if TARGET_OS_IPHONE

    ofxUIGenericViewController* getUIViewController();

#elif TARGET_ANDROID

    static jclass getJNIClassStatic();
	virtual jclass getJNIClass();
    static const char* className;

    jobject getJNIInstance();
#endif

    ofRectangle getFrame();
    void setFrame( const ofRectangle& setFrame );
    
    ofColor getBackgroundColor();
    void setBackgroundColor( const ofColor& setColor );
    
    void addChildView( ofPtr< ofxGenericView > add );
    void removeChildView( ofPtr< ofxGenericView > remove );
    void removeFromParent();
    void removeChildViews();
    
    ofPtrWeak< ofxGenericView > getParent();
    
    virtual void willLoad();
    virtual void didLoad();
    
    virtual void willAppear();
    virtual void didAppear();
    virtual void willDisappear();
    virtual void didDisappear();
    
    void setAlpha( float alpha );
    float getAlpha();
    
    void setVisible( bool visible );
    bool getVisible();
    
    void setNextResponder( ofPtrWeak< ofxGenericView > view );
    ofPtr< ofxGenericView > getNextResponder();
    
    // TEMP HACK:
    void replaceChildrenWith( ofPtr< ofxGenericView > view );
    
    virtual void setViewDelegate( ofPtrWeak< ofxGenericViewDelegate > delegate );
    
protected:        
    ofxGenericView();
    virtual void init( ofPtrWeak< ofxGenericView > smartPointer, const ofRectangle& setBounds = ofRectangle( 0, 0, 0, 0 ) );
    
    virtual NativeView createNativeView( const ofRectangle& frame );
    NativeView _view;

#if TARGET_OS_IPHONE

    virtual ofxUIGenericViewController* createUIViewController();
    ofxUIGenericViewController* _viewController;

#elif TARGET_ANDROID

    static jclass _jniClass;

    enum ofxGenericViewMethods
    {
    	JNIMethod_constructor = JNIObject::Last,
    	JNIMethod_Init,
    	JNIMethod_SetFrame,
    	JNIMethod_GetFrame,
    	JNIMethod_GetBackgroundColor,
    	JNIMethod_SetBackgroundColor,
    	JNIMethod_AddChildView,
    	JNIMethod_RemoveChildView,
    	Last
    };
    virtual void registerJNIMethods();
#endif
    
    ofPtrWeak< ofxGenericView > _this;
    
    ofPtrWeak< ofxGenericView > _nextResponder;

    std::list< ofPtr< ofxGenericView > > _children;
    ofPtrWeak< ofxGenericView > _parent;
    ofPtr< ofxGenericView > getChildViewofPtr( ofxGenericView* forView );
    ofPtrWeak < ofxGenericViewDelegate > _viewDelegate;
};

class ofxGenericViewDelegate
{
public:
    virtual ~ofxGenericViewDelegate() {};
    
    virtual void willLoad( ofPtr< ofxGenericView > view ) {};
    virtual void didLoad( ofPtr< ofxGenericView > view ) {};
    
    virtual void willAppear( ofPtr< ofxGenericView > view ) {};
    virtual void didAppear( ofPtr< ofxGenericView > view ) {};
    virtual void willDisappear( ofPtr< ofxGenericView > view ) {};
    virtual void didDisappear( ofPtr< ofxGenericView > view ) {};
};

#if TARGET_OS_IPHONE

@interface ofxUIGenericViewController : UIViewController
{
@protected
    ofPtrWeak< ofxGenericView > _delegate;
    NSMutableDictionary* _activeTouches;
}
-( id )initWithDelegate:( ofPtrWeak< ofxGenericView > ) delegate;

@end

#define ofxGenericUIViewCastOperator( ofxType, UIViewType ) \
ofxType::operator UIViewType*() \
{ \
    if ( [ _view isKindOfClass:[ UIViewType class ] ] ) \
    { \
        return ( UIViewType* )_view; \
    } \
    return nil; \
}
#endif
