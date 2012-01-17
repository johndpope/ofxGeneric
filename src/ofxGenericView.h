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
    
    virtual void init( const ofRectangle& setBounds );

    UIView* getUIView();
    UIViewController* getUIViewController();

    ofRectangle getBounds();
    void setBounds( const ofRectangle& setBounds );
    
    ofColor getBackgroundColor();
    void setBackgroundColor( const ofColor& setColor );
    
    void addChildView( ofxGenericView* add );
    void removeChildView( ofxGenericView* remove );
    void removeFromParent();
    void destroyChildViews();
    
protected:        
    virtual UIView* createUIView( const CGRect& frame );
    virtual UIViewController* createUIViewController();

    UIView* _view;
    UIViewController* _viewController;

    std::list< ofxGenericView* > _children;
    ofxGenericView* _parent;
};

