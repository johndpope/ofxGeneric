//
//  ofxGenericView.h
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 12/29/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"
#include "ofxGenericConstants.h"

#include <list>

class ofxGenericViewDelegate;
#if TARGET_OS_IPHONE
@class ofxUIGenericViewController;
#endif

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
    virtual void setFrame( const ofRectangle& setFrame );
    
    virtual ofColor getBackgroundColor();
    virtual void setBackgroundColor( const ofColor& setColor );
    
    void addChildView( ofPtr< ofxGenericView > add );
    void addChildView( ofPtr< ofxGenericView > add, ofPtr< ofxGenericView > before );
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
    
    void setClipSubviews( bool clip );
    bool getClipSubviews();
    
    void setAutoresizingMask( int autoresizing );
    
    void setNextResponder( ofPtrWeak< ofxGenericView > view );
    ofPtr< ofxGenericView > getNextResponder();
    
    virtual void setViewDelegate( ofPtrWeak< ofxGenericViewDelegate > delegate );
    
    static void beginAnimation( string animationId, ofPtr< ofxGenericViewDelegate > delegate = ofPtr< ofxGenericViewDelegate >() );
    static void commitAnimation();
    
//    + (void)setAnimationWillStartSelector:(SEL)selector;                // default = NULL. -animationWillStart:(NSString *)animationID context:(void *)context
//    + (void)setAnimationDidStopSelector:(SEL)selector;                  // default = NULL. -animationDidStop:(NSString *)animationID finished:(NSNumber *)finished context:(void *)context
    static void setAnimationDuration( double seconds = 0.2 );
    static void setAnimationDelay( double seconds = 0.0 );
    static void setAnimationCurve( ofxGenericViewAnimationCurve curve );
  
    static void setAnimationTransition( ofxGenericViewAnimationTransition transition, ofPtr< ofxGenericView > forView );
//    + (void)setAnimationTransition:(UIViewAnimationTransition)transition forView:(UIView *)view cache:(BOOL)cache;  // current limitation - only one per begin/commit block
    
//    + (void)setAnimationsEnabled:(BOOL)enabled;                         // ignore any attribute changes while set.
//    + (BOOL)areAnimationsEnabled;

    
    virtual string dumpViewGraph( int depth );
    virtual string toString();
    
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

    void setIsAttachedToRoot( bool attached );
    bool _isAttachedToRoot;

    std::list< ofPtr< ofxGenericView > > _children;
    ofPtrWeak< ofxGenericView > _parent;
    ofPtrWeak < ofxGenericViewDelegate > _viewDelegate;
    
    friend class ofxAppGenericWindow;
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
    
    virtual void animationWillStart( string animationId ) {};
    virtual void animationDidStop( string animationId ) {};
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
