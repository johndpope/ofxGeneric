//
//  ofxGenericDatePickerView.cpp
//  ofxGeneric
//
//  Created by Konstantin Yavichev on 8/22/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericDatePickerView.h"

#include "ofxGenericDate.h"


#if TARGET_OS_IPHONE
@interface ofxGenericDatePickerViewForwarder : NSObject
{
@private
    ofxGenericDatePickerView* _forwardTo;
}

-( void ) setForwardTo:( ofxGenericDatePickerView* )setForwardTo;

-( void ) valueChanged:( id )sender;

@end;
#endif



ofPtr< ofxGenericDatePickerView > ofxGenericDatePickerView::create( const ofRectangle& setFrame )
{
    ofPtr< ofxGenericDatePickerView > create = ofPtr< ofxGenericDatePickerView >( new ofxGenericDatePickerView() );
    create->init( create, setFrame );
    return create;
}

NativeView ofxGenericDatePickerView::createNativeView( const ofRectangle& frame )
{
#if TARGET_OS_IPHONE
    UIDatePicker* newView = [ [ UIDatePicker alloc ] initWithFrame:ofRectangleToCGRect( frame ) ];
    
    // TODO: Same as the button, doesn't match createUIView design
    _forwarder = [ [ ofxGenericDatePickerViewForwarder alloc ] init ];
    [ _forwarder setForwardTo:this ];
    
    [ newView addTarget:_forwarder action:@selector( valueChanged: ) forControlEvents: UIControlEventValueChanged ];
    
    return newView;
#endif
}

void ofxGenericDatePickerView::setDelegate( ofPtrWeak< ofxGenericDatePickerViewDelegate > delegate )
{
    _delegate = delegate;
}

void ofxGenericDatePickerView::setMode( ofxGenericDatePickerMode mode )
{
#if TARGET_OS_IPHONE
    if ( [ getNativeView() isKindOfClass:[ UIDatePicker class ] ] )
    {
        UIDatePicker* asDatePicker = ( UIDatePicker* )getNativeView();
        
        asDatePicker.datePickerMode = ofxGenericDatePickerModeToiOS( mode );
    }
#endif
}

ofxGenericDatePickerMode ofxGenericDatePickerView::getMode()
{
    ofxGenericDatePickerMode retval = ofxGenericDatePickerModeTime;
    
#if TARGET_OS_IPHONE
    if ( [ getNativeView() isKindOfClass:[ UIDatePicker class ] ] )
    {
        UIDatePicker* asDatePicker = ( UIDatePicker* )getNativeView();
        
        retval = iOSToofxGenericDatePickerMode( asDatePicker.datePickerMode );
    }
#endif
    
    return retval;
}

void ofxGenericDatePickerView::setDate( ofPtr< ofxGenericDate > value, bool animate )
{
    if( value )
    {
#if TARGET_OS_IPHONE
        if ( [ getNativeView() isKindOfClass:[ UIDatePicker class ] ] )
        {
            UIDatePicker* asDatePicker = ( UIDatePicker* )getNativeView();
            
            [asDatePicker setDate:value->convertToNSDate() animated:animate];
        }
#endif
    }
}

ofPtr< ofxGenericDate > ofxGenericDatePickerView::getDate()
{    
#if TARGET_OS_IPHONE
    if ( [ getNativeView() isKindOfClass:[ UIDatePicker class ] ] )
    {
        UIDatePicker* asDatePicker = ( UIDatePicker* )getNativeView();
        
        return ofxGenericDate::createFromNSDate( asDatePicker.date );
    }
#endif
    
    return ofPtr< ofxGenericDate >();
}


void ofxGenericDatePickerView::valueChanged()
{
    if ( _delegate )
    {
        ( _delegate.lock() )->picker_valueChanged( dynamic_pointer_cast< ofxGenericDatePickerView >( _this.lock() ) );
    }
}



#if TARGET_OS_IPHONE
@implementation ofxGenericDatePickerViewForwarder

-( void )setForwardTo:(ofxGenericDatePickerView *)setForwardTo
{
    _forwardTo = setForwardTo;
}

-( void ) valueChanged:( id )sender
{
    _forwardTo->valueChanged();
}

@end
#endif
