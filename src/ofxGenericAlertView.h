//
//  ofxGenericAlertView.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 4/18/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"

class ofxGenericAlertViewDelegate;
#if TARGET_OS_IPHONE
@class ofxGenericAlertViewForwarder;
#endif

class ofxGenericAlertView
{
public:
    static ofPtr< ofxGenericAlertView > create( const string& title = string(),
                                                const string& message = string(),
                                                const string& cacnelButtonLabel = string(),
                                                ofPtrWeak< ofxGenericAlertViewDelegate > delegate = ofPtrWeak< ofxGenericAlertViewDelegate >() );

    void setTitle( string title );
    void setMessage( string message );
    void addOtherButton( string buttonTitle );
    
    void show();
    
    virtual ~ofxGenericAlertView();
    
    virtual void clickedButtonAtIndex( int buttonIndex );
    virtual void cancelled();
protected:
    ofxGenericAlertView();
    virtual void init( ofPtrWeak< ofxGenericAlertView > setThis,
                       const string& title,
                       const string& message,
                       const string& cancelButtonLabel,
                       ofPtrWeak< ofxGenericAlertViewDelegate > delegate );
    ofPtrWeak< ofxGenericAlertView > _this;

    virtual void createNativeView(const string& cancelButtonLabel);
#if TARGET_OS_IPHONE
    UIAlertView* _view;
#endif    
    
#if TARGET_OS_IPHONE
    ofxGenericAlertViewForwarder* _forwarder;
#endif
    ofPtrWeak< ofxGenericAlertViewDelegate > _delegate;
};

class ofxGenericAlertViewDelegate
{
public:
    virtual ~ofxGenericAlertViewDelegate() {};
    
    virtual void alertView_clickedButtonAtIndex( int buttonIndex ) {};
    virtual void alertView_cancelled() {};
};