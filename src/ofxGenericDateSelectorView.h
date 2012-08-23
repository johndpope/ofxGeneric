//
//  ofxGenericDateSelectorView.h
//  ofxGeneric
//
//  Created by Konstantin Yavichev on 8/22/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericView.h"

class ofxGenericDate;

#if TARGET_OS_IPHONE
@class ofxGenericDateSelectorViewForwarder;
#endif
class ofxGenericDateSelectorViewDelegate;


class ofxGenericDateSelectorView : public ofxGenericView
{
public:
    static ofPtr< ofxGenericDateSelectorView > create( const ofRectangle& setFrame = ofRectangle( 0, 0, 0, 0 ), ofPtrWeak< ofxGenericDateSelectorViewDelegate > delegate = ofPtrWeak< ofxGenericDateSelectorViewDelegate >() );
    
    void setDelegate( ofPtrWeak< ofxGenericDateSelectorViewDelegate > delegate );

    void setMode( ofxGenericDateSelectorMode mode );
    ofxGenericDateSelectorMode getMode();
    
    void setDate( ofPtr< ofxGenericDate > value, bool animate = false );
    ofPtr< ofxGenericDate > getDate();
    
    virtual void valueChanged();
    
    virtual ~ofxGenericDateSelectorView();
protected:
    ofxGenericDateSelectorView();
    
    virtual NativeView createNativeView( const ofRectangle& frame );
    
    ofPtrWeak< ofxGenericDateSelectorViewDelegate > _delegate;
#if TARGET_OS_IPHONE
    ofxGenericDateSelectorViewForwarder* _forwarder;
#endif
};


class ofxGenericDateSelectorViewDelegate
{
public:
    virtual ~ofxGenericDateSelectorViewDelegate() {};
    
    virtual void selector_valueChanged( ofPtr< ofxGenericDateSelectorView > dateSelectorView ) {};
};
