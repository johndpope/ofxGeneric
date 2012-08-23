//
//  ofxGenericDatePickerView.h
//  ofxGeneric
//
//  Created by Konstantin Yavichev on 8/22/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericView.h"

class ofxGenericDate;

#if TARGET_OS_IPHONE
@class ofxGenericDatePickerViewForwarder;
#endif
class ofxGenericDatePickerViewDelegate;



class ofxGenericDatePickerView : public ofxGenericView
{
public:
    static ofPtr< ofxGenericDatePickerView > create( const ofRectangle& setFrame = ofRectangle( 0, 0, 0, 0 ) );
    
    void setDelegate( ofPtrWeak< ofxGenericDatePickerViewDelegate > delegate );

    void setMode( ofxGenericDatePickerMode mode );
    ofxGenericDatePickerMode getMode();
    
    void setDate( ofPtr< ofxGenericDate > value, bool animate = false );
    ofPtr< ofxGenericDate > getDate();
    
    virtual void valueChanged();
    
protected:
    ofxGenericDatePickerView() {};
    
    virtual NativeView createNativeView( const ofRectangle& frame );
    
    ofPtrWeak< ofxGenericDatePickerViewDelegate > _delegate;
#if TARGET_OS_IPHONE
    ofxGenericDatePickerViewForwarder* _forwarder;
#endif
};



class ofxGenericDatePickerViewDelegate
{
public:
    virtual ~ofxGenericDatePickerViewDelegate() {};
    
    virtual void picker_valueChanged( ofPtr< ofxGenericDatePickerView > datePickerView ) {};
};
