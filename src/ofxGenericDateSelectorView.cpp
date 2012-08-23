//
//  ofxGenericDateSelectorView.cpp
//  ofxGeneric
//
//  Created by Konstantin Yavichev on 8/22/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericDateSelectorView.h"

#include "ofxGenericDate.h"


#if TARGET_OS_IPHONE
@interface ofxGenericDateSelectorViewForwarder : NSObject
{
@private
    ofxGenericDateSelectorView* _forwardTo;
}

-( void ) setForwardTo:( ofxGenericDateSelectorView* )setForwardTo;

-( void ) valueChanged:( id )sender;

@end;
#endif



ofPtr< ofxGenericDateSelectorView > ofxGenericDateSelectorView::create( const ofRectangle& setFrame )
{
    ofPtr< ofxGenericDateSelectorView > create = ofPtr< ofxGenericDateSelectorView >( new ofxGenericDateSelectorView() );
    create->init( create, setFrame );
    return create;
}

NativeView ofxGenericDateSelectorView::createNativeView( const ofRectangle& frame )
{
#if TARGET_OS_IPHONE
    UIDatePicker* newView = [ [ UIDatePicker alloc ] initWithFrame:ofRectangleToCGRect( frame ) ];
    
    // TODO: Same as the button, doesn't match createUIView design
    _forwarder = [ [ ofxGenericDateSelectorViewForwarder alloc ] init ];
    [ _forwarder setForwardTo:this ];
    
    [ newView addTarget:_forwarder action:@selector( valueChanged: ) forControlEvents: UIControlEventValueChanged ];
    
    return newView;
#endif
}

void ofxGenericDateSelectorView::setDelegate( ofPtrWeak< ofxGenericDateSelectorViewDelegate > delegate )
{
    _delegate = delegate;
}

void ofxGenericDateSelectorView::setMode( ofxGenericDateSelectorMode mode )
{
#if TARGET_OS_IPHONE
    if ( [ getNativeView() isKindOfClass:[ UIDatePicker class ] ] )
    {
        UIDatePicker* asDatePicker = ( UIDatePicker* )getNativeView();
        
        asDatePicker.datePickerMode = ofxGenericDateSelectorModeToiOS( mode );
    }
#endif
}

ofxGenericDateSelectorMode ofxGenericDateSelectorView::getMode()
{
    ofxGenericDateSelectorMode retval = ofxGenericDateSelectorModeTime;
    
#if TARGET_OS_IPHONE
    if ( [ getNativeView() isKindOfClass:[ UIDatePicker class ] ] )
    {
        UIDatePicker* asDatePicker = ( UIDatePicker* )getNativeView();
        
        retval = iOSToofxGenericDateSelectorMode( asDatePicker.datePickerMode );
    }
#endif
    
    return retval;
}

void ofxGenericDateSelectorView::setDate( ofPtr< ofxGenericDate > value, bool animate )
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

ofPtr< ofxGenericDate > ofxGenericDateSelectorView::getDate()
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


void ofxGenericDateSelectorView::valueChanged()
{
    if ( _delegate )
    {
        ( _delegate.lock() )->selector_valueChanged( dynamic_pointer_cast< ofxGenericDateSelectorView >( _this.lock() ) );
    }
}



#if TARGET_OS_IPHONE
@implementation ofxGenericDateSelectorViewForwarder

-( void )setForwardTo:(ofxGenericDateSelectorView *)setForwardTo
{
    _forwardTo = setForwardTo;
}

-( void ) valueChanged:( id )sender
{
    _forwardTo->valueChanged();
}

@end
#endif
