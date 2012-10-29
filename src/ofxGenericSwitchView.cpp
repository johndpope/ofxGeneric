//
//  ofxGenericSwitchView.cpp
//  ofxGeneric
//
//  Created by Ian Grossberg on 10/28/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericSwitchView.h"

#if TARGET_OS_IPHONE
@interface ofxGenericSwitchViewForwarder : NSObject
{
@private
    ofPtrWeak< ofxGenericSwitchView > _forwardTo;
}
-( id )initWithForwardTo:( ofPtrWeak< ofxGenericSwitchView > )forwardTo;

-( void )uicontrolEventValueChanged:( id )sender;
@end
#endif

ofPtr< ofxGenericSwitchView > ofxGenericSwitchView::create( const ofRectangle& setFrame, ofPtrWeak< ofxGenericSwitchViewDelegate > delegate )
{
    ofPtr< ofxGenericSwitchView > create( new ofxGenericSwitchView() );
    create->init( create, setFrame, delegate );
    return create;
}

ofxGenericSwitchView::ofxGenericSwitchView()
{
}

NativeView ofxGenericSwitchView::createNativeView( const ofRectangle& frame )
{
#if TARGET_OS_IPHONE
    UISwitch* newView = [ [ UISwitch alloc ] initWithFrame:ofRectangleToCGRect( frame ) ];
    [ newView setBackgroundColor:[ UIColor clearColor ] ];
    _forwarder = [ [ ofxGenericSwitchViewForwarder alloc ] initWithForwardTo:dynamic_pointer_cast< ofxGenericSwitchView >( _this ) ];
    [ newView addTarget:_forwarder action:@selector( uicontrolEventValueChanged: ) forControlEvents:UIControlEventValueChanged ];
    return newView;
#endif
}

void ofxGenericSwitchView::init( ofPtrWeak< ofxGenericSwitchView > setThis, const ofRectangle& setFrame, ofPtrWeak< ofxGenericSwitchViewDelegate > delegate )
{
    setDelegate( delegate );
    ofxGenericView::init( setThis, setFrame );
}

ofxGenericSwitchView::~ofxGenericSwitchView()
{
#if TARGET_OS_IPHONE
    if ( _forwarder )
    {
        [ _forwarder release ];
        _forwarder = nil;
    }
#endif
}

void ofxGenericSwitchView::setDelegate( ofPtrWeak< ofxGenericSwitchViewDelegate > delegate )
{
    _delegate = delegate;
}

ofPtrWeak< ofxGenericSwitchViewDelegate > ofxGenericSwitchView::getDelegate()
{
    return _delegate;
}

void ofxGenericSwitchView::setSwitched( bool value, bool animate )
{
#if TARGET_IS_IPHONE
    if ( [ getNativeView() isKindOfClass:[ UISwitch class ] ] )
    {
        UISwitch* switchView = ( UISwitch* )getNativeView();
        [ switchView setOn:( BOOL )value animated:( BOOL )animate ];
    }
#endif
}

bool ofxGenericSwitchView::isSwitched()
{
    bool result = false;
#if TARGET_OS_IPHONE
    if ( [ getNativeView() isKindOfClass:[ UISwitch class ] ] )
    {
        UISwitch* switchView = ( UISwitch* )getNativeView();
        result = ( bool )[ switchView isOn ];
    }
#endif
    return result;
}

void ofxGenericSwitchView::valueChanged()
{
    ofPtr< ofxGenericSwitchViewDelegate > delegate = _delegate.lock();
    if ( delegate )
    {
        delegate->switch_valueChanged( dynamic_pointer_cast< ofxGenericSwitchView >(  _this ) );
    }
}

void ofxGenericSwitchView::setTintColor( const ofColor& color )
{
#if TARGET_OS_IPHONE
    if ( [ getNativeView() isKindOfClass:[ UISwitch class ] ] )
    {
        UISwitch* switchView = ( UISwitch* )getNativeView();
        [ switchView setTintColor:ofColorToUIColor( color ) ];
    }
#endif
}

ofColor ofxGenericSwitchView::getTintColor()
{
    ofColor result;
#if TARGET_OS_IPHONE
    if ( [ getNativeView() isKindOfClass:[ UISwitch class ] ] )
    {
        UISwitch* switchView = ( UISwitch* )getNativeView();
        result = UIColorToofColor( [ switchView tintColor ] );
    }
#endif
    return result;
}

#if TARGET_OS_IPHONE
@implementation ofxGenericSwitchViewForwarder

-( id )initWithForwardTo:( ofPtrWeak< ofxGenericSwitchView > )forwardTo;
{
    self = [ super init ];
    if ( self )
    {
        _forwardTo = forwardTo;
    }
    return self;
}

-( void )uicontrolEventValueChanged:( id )sender
{
    ofPtr< ofxGenericSwitchView > forwardTo = _forwardTo.lock();
    if ( forwardTo )
    {
        forwardTo->valueChanged();
    }
}

@end
#endif