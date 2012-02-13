//
//  ofxGenericView.h
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 12/29/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofMain.h"

#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>
typedef UIView* NativeView;
#endif

#if TARGET_ANDROID
#include <jni.h>
typedef jobject NativeView;
#endif

#include <list>

class ofxGenericView
{
public:
    ofxGenericView();
    virtual ~ofxGenericView();
    
    virtual void init( ofPtrWeak< ofxGenericView > setThis, const ofRectangle& setBounds = ofRectangle( 0, 0, 0, 0 ) );

    NativeView getNativeView();
    operator NativeView();
#if TARGET_OS_IPHONE
    UIViewController* getUIViewController();
#endif

    ofRectangle getFrame();
    void setFrame( const ofRectangle& setFrame );
    
    ofColor getBackgroundColor();
    void setBackgroundColor( const ofColor& setColor );
    
    void addChildView( ofPtr< ofxGenericView > add );
    void removeChildView( ofPtr< ofxGenericView > remove );
    void removeFromParent();
    void removeChildViews();
    
protected:        
    virtual NativeView* createNativeView( const ofRectangle& frame );
    NativeView _view;
#if TARGET_OS_IPHONE
    virtual UIViewController* createUIViewController();
    UIViewController* _viewController;
#endif
    
    ofPtrWeak< ofxGenericView > _this;

    std::list< ofPtr< ofxGenericView > > _children;
    ofPtrWeak< ofxGenericView > _parent;
    ofPtr< ofxGenericView > getChildViewofPtr( ofxGenericView* forView );
};

#if TARGET_OS_IPHONE
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
