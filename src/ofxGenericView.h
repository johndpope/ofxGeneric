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
#endif

#include <list>

class ofxGenericView
{
public:
    ofxGenericView();
    virtual ~ofxGenericView();
    
    virtual void init( ofPtrWeak< ofxGenericView > setThis, const ofRectangle& setBounds = ofRectangle( 0, 0, 0, 0 ) );

#if TARGET_OS_IPHONE
    UIView* getUIView();
    operator UIView*();
    UIViewController* getUIViewController();
#endif

    ofRectangle getBounds();
    void setBounds( const ofRectangle& setBounds );
    
    ofColor getBackgroundColor();
    void setBackgroundColor( const ofColor& setColor );
    
    void addChildView( ofPtr< ofxGenericView > add );
    void removeChildView( ofPtr< ofxGenericView > remove );
    void removeFromParent();
    void removeChildViews();
    
protected:        
#if TARGET_OS_IPHONE
    virtual UIView* createUIView( const CGRect& frame );
    virtual UIViewController* createUIViewController();
    UIView* _view;
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
