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
@class ofxUIGenericViewControllerForwarder;
#endif

class ofxGenericView
#if TARGET_ANDROID
: public JNIObject
#endif
{
public:
    static ofPtr< ofxGenericView > create( const ofRectangle& setFrame = ofRectangle( 0, 0, 0, 0 ), NativeView nativeView = NativeNull );
    virtual ~ofxGenericView();
    
    NativeView getNativeView();
    operator NativeView();
#if TARGET_OS_IPHONE

    ofxUIGenericViewControllerForwarder* getUIViewController();

#elif TARGET_ANDROID

    static jclass getJNIClassStatic();
	virtual jclass getJNIClass();
    static const char* className;

    jobject getJNIInstance();
#endif

    ofRectangle getFrame();
    ofRectangle getFrame( const ofPoint& setTopLeft );
    virtual void setFrame( const ofRectangle& setFrame );
    
    virtual ofColor getBackgroundColor();
    virtual void setBackgroundColor( const ofColor& setColor );
    
    virtual void addChildView( ofPtr< ofxGenericView > add );
    virtual void addChildViewBefore( ofPtr< ofxGenericView > add, ofPtr< ofxGenericView > before );
    virtual void removeChildView( ofPtr< ofxGenericView > remove );
    void removeFromParent();
    void removeChildViews();
    
    ofPtrWeak< ofxGenericView > getParent();
    
    virtual void willLoad();
    virtual void didLoad();
    
    virtual void willAppear();
    virtual void didAppear();
    virtual void willDisappear();
    virtual void didDisappear();
    
    virtual void hitInView( ofPoint location );
    
    void setAlpha( float alpha );
    float getAlpha();
    
    void setVisible( bool visible );
    bool getVisible();
    
    void setClipSubviews( bool clip );
    bool getClipSubviews();
    
    void setAutoresizingMask( int autoresizing );
    int getAutoresizingMask( );
    
    void setNextResponder( ofPtrWeak< ofxGenericView > view );
    ofPtr< ofxGenericView > getNextResponder();
    
    virtual void setViewDelegate( ofPtrWeak< ofxGenericViewDelegate > delegate );
    
    virtual bool containsPoint( const ofPoint& point );
    virtual bool containsPoint( const ofPoint& point, ofPtr< ofxGenericView > pointRelativeTo );
    ofPoint convertFrom( const ofPoint& point, ofPtr< ofxGenericView > view );
    ofRectangle convertFrom( const ofRectangle& rectangle, ofPtr< ofxGenericView > view );
    
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

    virtual void addGestureRecognizerSwipe( ofxGenericGestureTypeSwipe type );
    virtual void addGestureRecognizerTap( int tapCount, int fingerCount );
    virtual void addGestureRecognizerHold( float minimumPressDuration, unsigned int fingerCount, float allowableMovement );
    virtual void addGestureRecognizerPan( unsigned int minimumFingerCount, unsigned int maximumFingerCount );
    
#if TARGET_OS_IPHONE
    virtual void gesturePerformedSwipe( UISwipeGestureRecognizer* recognizer );
    virtual void gesturePerformedTap( UITapGestureRecognizer* recognizer );
    virtual void gesturePerformedHold( UILongPressGestureRecognizer* recognizer );
    virtual void gesturePerformedPan( UIPanGestureRecognizer* recognizer );
#endif
    virtual void gesturePerformedSwipe( ofxGenericGestureTypeSwipe type, ofPoint location );
    virtual void gesturePerformedTap( int tapCount, int fingerCount, ofPoint location );
    virtual void gesturePerformedHold( float minimumPressDuration, unsigned int fingerCount, float allowableMovement, ofPoint location );
    virtual void gesturePerformedPan( unsigned int fingerCount, ofPoint distance );

    virtual string dumpViewGraph( int depth );
    virtual string toString();
    
    static void replaceViewWithView( ofPtr< ofxGenericView > replace, ofPtr< ofxGenericView > with );
    
protected:        
    ofxGenericView();
    virtual void init( ofPtrWeak< ofxGenericView > setThis, const ofRectangle& setBounds = ofRectangle( 0, 0, 0, 0 ), NativeView nativeView = NativeNull );
    
#if TARGET_OS_IPHONE
    virtual UIView* allocNativeView( const ofRectangle& frame );
#elif TARGET_ANDROID
    virtual jobject allocNativeView( const ofRectangle& frame );
#endif
    virtual NativeView createNativeView( const ofRectangle& frame );
    NativeView _view;
    
#if TARGET_OS_IPHONE

    virtual ofxUIGenericViewControllerForwarder* createUIViewController();
    ofxUIGenericViewControllerForwarder* _viewController;

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
    
private:
    void addChildViewPre( ofPtr< ofxGenericView > add );
    void addChildViewPost( ofPtr< ofxGenericView > add );
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
    
    virtual void gesturePerformedSwipe( ofPtr< ofxGenericView > view, ofxGenericGestureTypeSwipe type, ofPoint location ) {};
    virtual void gesturePerformedTap( ofPtr< ofxGenericView > view, int tapCount, int fingerCount, ofPoint location ) {};
    virtual void gesturePerformedHold( ofPtr< ofxGenericView > view, float minimumPressDuration, unsigned int fingerCount, float allowableMovement, ofPoint location ) {};
    virtual void gesturePerformedPan( ofPtr< ofxGenericView > view, unsigned int fingerCount, ofPoint distance ) {};
    
    virtual void hitInView( ofPoint location ) {};
};

#if TARGET_OS_IPHONE

@interface ofxUIGenericViewControllerForwarder : UIViewController
{
@protected
    ofPtrWeak< ofxGenericView > _forwardTo;
    NSMutableDictionary* _activeTouches;
}
-( id )initWithForwardTo:( ofPtrWeak< ofxGenericView > )forwardTo;

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
