//
//  ofxGenericSliderView.cpp
//  ofxGeneric
//
//  Created by Ian Grossberg on 4/22/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericSliderView.h"
#include "ofxGenericUtility.h"

#import "ofCommon.h"

#if TARGET_OS_IPHONE

@interface ofxGenericSliderViewForwarder : NSObject
{
@private
    ofPtrWeak< ofxGenericSliderView > _delegate;
}
-( id )initWithDelegate:( ofPtrWeak< ofxGenericSliderView > )delegate;
-( void )valueChanged:( id )sender;
-( void )touchCancel:( id )sender;
-( void )touchDown:( id )sender;
-( void )touchDownRepeat:( id )sender;
-( void )touchDragEnter:( id )sender;
-( void )touchDragExit:( id )sender;
-( void )touchDragInside:( id )sender;
-( void )touchDragOutside:( id )sender;
-( void )touchUpInside:( id )sender;
-( void )touchUpOutside:( id )sender;

@end

#endif

ofPtr< ofxGenericSliderView > ofxGenericSliderView::create( const ofRectangle& setFrame, ofPtrWeak< ofxGenericSliderViewDelegate > delegate )
{
    ofPtr< ofxGenericSliderView > create( new ofxGenericSliderView() );
    create->init( create, setFrame, delegate );
    return create;
}

ofxGenericSliderView::ofxGenericSliderView()
{
}

void ofxGenericSliderView::init( ofPtrWeak< ofxGenericSliderView > setThis, const ofRectangle& setFrame, ofPtrWeak< ofxGenericSliderViewDelegate > delegate )
{
    ofxGenericView::init( setThis, setFrame );
    _delegate = delegate;
}

NativeView ofxGenericSliderView::createNativeView( const ofRectangle& frame )
{
#if TARGET_OS_IPHONE
    UISlider* newView = [ [ UISlider alloc ] initWithFrame:ofxRectangleToCGRect( frame ) ];
    
    _forwarder = [ [ ofxGenericSliderViewForwarder alloc ] initWithDelegate:dynamic_pointer_cast< ofxGenericSliderView >( _this ) ];
    
    [ newView addTarget:_forwarder action:@selector( valueChanged: ) forControlEvents: UIControlEventValueChanged ];
    [ newView addTarget:_forwarder action:@selector( touchCancel: ) forControlEvents: UIControlEventTouchCancel ];
    [ newView addTarget:_forwarder action:@selector( touchDown: ) forControlEvents: UIControlEventTouchDown ];
    [ newView addTarget:_forwarder action:@selector( touchDownRepeat: ) forControlEvents: UIControlEventTouchDownRepeat ];
    [ newView addTarget:_forwarder action:@selector( touchDragEnter: ) forControlEvents: UIControlEventTouchDragEnter ];
    [ newView addTarget:_forwarder action:@selector( touchDragExit: ) forControlEvents: UIControlEventTouchDragExit ];
    [ newView addTarget:_forwarder action:@selector( touchDragInside: ) forControlEvents: UIControlEventTouchDragInside ];
    [ newView addTarget:_forwarder action:@selector( touchDragOutside: ) forControlEvents: UIControlEventTouchDragOutside ];
    [ newView addTarget:_forwarder action:@selector( touchUpInside: ) forControlEvents: UIControlEventTouchUpInside ];
    [ newView addTarget:_forwarder action:@selector( touchUpOutside: ) forControlEvents:UIControlEventTouchUpOutside ];

    return newView;    
#endif
}

float ofxGenericSliderView::getValue()
{    
#if TARGET_OS_IPHONE
    UISlider* view = ( UISlider* )*this;
    if ( view )
    {
        return [ view value ];
    }
#endif
    return 0.0f;
}

void ofxGenericSliderView::setValue( float value, bool animated )
{  
#if TARGET_OS_IPHONE
    UISlider* view = ( UISlider* )*this;
    if ( view )
    {
        [ view setValue:value animated:( BOOL )animated ];
    }
#endif
}

float ofxGenericSliderView::getMinimum()
{    
#if TARGET_OS_IPHONE
    UISlider* view = ( UISlider* )*this;
    if ( view )
    {
        return [ view minimumValue ];
    }
#endif
    return 0.0f;
}

float ofxGenericSliderView::getMaximum()
{    
#if TARGET_OS_IPHONE
    UISlider* view = ( UISlider* )*this;
    if ( view )
    {
        return [ view maximumValue ];
    }
#endif
    return 0.0f;
}

void ofxGenericSliderView::setRange( float minimum, float maximum )
{    
#if TARGET_OS_IPHONE
    UISlider* view = ( UISlider* )*this;
    if ( view )
    {
        [ view setMinimumValue:minimum ];
        [ view setMaximumValue:maximum ];
    }
#endif
}

#if TARGET_OS_IPHONE
ofxGenericUIViewCastOperator( ofxGenericSliderView, UISlider );
#endif

void ofxGenericSliderView::valueChanged()
{
    if ( _delegate )
    {
        _delegate.lock()->slider_valueChanged( dynamic_pointer_cast< ofxGenericSliderView >( _this.lock() ) );
    }
}

void ofxGenericSliderView::touchCancel()
{
    if ( _delegate )
    {
        _delegate.lock()->slider_touchCancel( dynamic_pointer_cast< ofxGenericSliderView >( _this.lock() ) );
    }
}

void ofxGenericSliderView::touchDown()
{
    if ( _delegate )
    {
        _delegate.lock()->slider_touchDown( dynamic_pointer_cast< ofxGenericSliderView >( _this.lock() ) );
    }
}

void ofxGenericSliderView::touchDownRepeat()
{
    if ( _delegate )
    {
        _delegate.lock()->slider_touchDownRepeat( dynamic_pointer_cast< ofxGenericSliderView >( _this.lock() ) );
    }
}

void ofxGenericSliderView::touchDragEnter()
{
    if ( _delegate )
    {
        _delegate.lock()->slider_touchDragEnter( dynamic_pointer_cast< ofxGenericSliderView >( _this.lock() ) );
    }
}

void ofxGenericSliderView::touchDragExit()
{
    if ( _delegate )
    {
        _delegate.lock()->slider_touchDragExit( dynamic_pointer_cast< ofxGenericSliderView >( _this.lock() ) );
    }
}

void ofxGenericSliderView::touchDragInside()
{
    if ( _delegate )
    {
        _delegate.lock()->slider_touchDragInside( dynamic_pointer_cast< ofxGenericSliderView >( _this.lock() ) );
    }
}

void ofxGenericSliderView::touchDragOutside()
{
    if ( _delegate )
    {
        _delegate.lock()->slider_touchDragOutside( dynamic_pointer_cast< ofxGenericSliderView >( _this.lock() ) );
    }
}

void ofxGenericSliderView::touchUpInside()
{
    if ( _delegate )
    {
        _delegate.lock()->slider_touchUpInside( dynamic_pointer_cast< ofxGenericSliderView >( _this.lock() ) );
    }
}

void ofxGenericSliderView::touchUpOutside()
{
    if ( _delegate )
    {
        _delegate.lock()->slider_touchUpOutside( dynamic_pointer_cast< ofxGenericSliderView >( _this.lock() ) );
    }
}

#if TARGET_OS_IPHONE

@implementation ofxGenericSliderViewForwarder
-( id )initWithDelegate:( ofPtrWeak< ofxGenericSliderView > )delegate
{
    self = [ super init ];
    if ( self )
    {
        _delegate = delegate;
    }
    return self;
}

-( void )valueChanged:( id )sender
{
    if ( _delegate )
    {
        _delegate.lock()->valueChanged();
    }
}

-( void )touchCancel:( id )sender
{    
    if ( _delegate )
    {
        _delegate.lock()->touchCancel();
    }
}

-( void )touchDown:( id )sender
{
    if ( _delegate )
    {
        _delegate.lock()->touchDown();
    }
}

-( void )touchDownRepeat:( id )sender
{
    if ( _delegate )
    {
        _delegate.lock()->touchDownRepeat();
    }
}

-( void )touchDragEnter:( id )sender
{
    if ( _delegate )
    {
        _delegate.lock()->touchDragEnter();
    }
}

-( void )touchDragExit:( id )sender
{
    if ( _delegate )
    {
        _delegate.lock()->touchDragExit();
    }
}

-( void )touchDragInside:( id )sender
{
    if ( _delegate )
    {
        _delegate.lock()->touchDragInside();
    }
}

-( void )touchDragOutside:( id )sender
{
    if ( _delegate )
    {
        _delegate.lock()->touchDragOutside();
    }
}

-( void )touchUpInside:( id )sender
{
    if ( _delegate )
    {
        _delegate.lock()->touchUpInside();
    }
}

-( void )touchUpOutside:( id )sender
{
    if ( _delegate )
    {
        _delegate.lock()->touchUpOutside();
    }
}

@end

#endif