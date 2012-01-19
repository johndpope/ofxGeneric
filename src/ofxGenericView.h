//
//  ofxGenericView.h
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 12/29/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofMain.h"

#import <UIKit/UIKit.h>

#import <list>

class ofxGenericView
{
public:
    ofxGenericView();
    virtual ~ofxGenericView();
    
    virtual void init( ofPtrWeak< ofxGenericView > setThis, const ofRectangle& setBounds );

    UIView* getUIView();
    UIViewController* getUIViewController();

    ofRectangle getBounds();
    void setBounds( const ofRectangle& setBounds );
    
    ofColor getBackgroundColor();
    void setBackgroundColor( const ofColor& setColor );
    
    void addChildView( ofPtr< ofxGenericView > add );
    void removeChildView( ofPtr< ofxGenericView > remove );
    void removeFromParent();
    void removeChildViews();
    
protected:        
    virtual UIView* createUIView( const CGRect& frame );
    virtual UIViewController* createUIViewController();
    UIView* _view;
    UIViewController* _viewController;
    
    ofPtrWeak< ofxGenericView > _this;

    std::list< ofPtr< ofxGenericView > > _children;
    ofPtrWeak< ofxGenericView > _parent;
    ofPtr< ofxGenericView > getChildViewofPtr( ofxGenericView* forView );
};

