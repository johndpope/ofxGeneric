//
//  ofxGenericSelectorView.cpp
//  ofxGeneric
//
//  Created by Ian Grossberg on 6/23/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericSelectorView.h"

#include "ofxGenericUtility.h"

#import "ofCommon.h"

unsigned int ofxGenericSelectorView::getNumberOfComponents()
{
    if ( _delegate )
    {
        return _delegate.lock()->selectorView_getNumberOfComponents( dynamic_pointer_cast< ofxGenericSelectorView >( _this.lock() ) );
    }
    return 0;
}

unsigned int ofxGenericSelectorView::getNumberOfRowsInComponent( unsigned int component )
{
    if ( _delegate )
    {
        return _delegate.lock()->selectorView_getNumberOfRowsInComponent( dynamic_pointer_cast< ofxGenericSelectorView >( _this.lock() ), component );
    }
    return 0;
}

float ofxGenericSelectorView::getWidthOfComponent( unsigned int component )
{
    if ( _delegate )
    {
        return _delegate.lock()->selectorView_getWidthOfComponent( dynamic_pointer_cast< ofxGenericSelectorView >( _this.lock() ), component );
    }
    return 0.0f;
}

float ofxGenericSelectorView::getHeightOfRowsInComponent( unsigned int component )
{
    if ( _delegate )
    {
        return _delegate.lock()->selectorView_getHeightOfRowsInComponent( dynamic_pointer_cast< ofxGenericSelectorView >( _this.lock() ), component );
    }
    return 0.0f;   
}

ofPtr< ofxGenericView > ofxGenericSelectorView::getViewForRowInComponent( unsigned int component, unsigned int row )
{
    if (  _delegate )
    {
        return _delegate.lock()->selectorView_getViewForRowInComponent( dynamic_pointer_cast< ofxGenericSelectorView >( _this.lock() ), component, row );
    }
    return ofPtr< ofxGenericView >();
}

void ofxGenericSelectorView::setFocusOn()
{
#if TARGET_OS_IPHONE
    UIPickerView* view = *this;
    if ( view )
    {
        [ view becomeFirstResponder ];
    }    
#endif
}

void ofxGenericSelectorView::scrollToRow( unsigned int component, unsigned int row, bool animated )
{
#if TARGET_OS_IPHONE
    UIPickerView* view = *this;
    if ( view )
    {
        [ view selectRow:row inComponent:component animated:( BOOL )animated ];
    }
#endif
}

void ofxGenericSelectorView::selectedRow( unsigned int component, unsigned int row )
{
    if ( _delegate )
    {
        _delegate.lock()->selectorView_selectedRow( dynamic_pointer_cast< ofxGenericSelectorView >( _this.lock() ), component, row );
    }
}

void ofxGenericSelectorView::reloadData()
{
#if TARGET_OS_IPHONE
    UIPickerView* view = *this;
    if ( view )
    {
        [ view reloadAllComponents ];
    }
#endif
}

void ofxGenericSelectorView::setShowSelectionOverlay( bool showSelectionOverlay )
{
#if TARGET_OS_IPHONE
    UIPickerView* view = *this;
    if ( view )
    {
        view.showsSelectionIndicator = ( BOOL )showSelectionOverlay;
    }
#endif
}

bool ofxGenericSelectorView::getShowSelectionOverlay()
{
#if TARGET_OS_IPHONE
    UIPickerView* view = *this;
    if ( view )
    {
        return (bool) view.showsSelectionIndicator;
    }
#endif
    
    return false;
}

#pragma mark Creation

ofPtr< ofxGenericSelectorView > ofxGenericSelectorView::create( const ofRectangle& setFrame, ofPtr< ofxGenericSelectorViewDelegate > delegate )
{
    ofPtr< ofxGenericSelectorView > create( new ofxGenericSelectorView() );
    create->init( create, setFrame, delegate );
    return create;
}

ofxGenericSelectorView::~ofxGenericSelectorView()
{
#if TARGET_OS_IPHONE
    [ _forwarder release ];
#endif
}

#if TARGET_OS_IPHONE
ofxGenericSelectorView::operator UIPickerView*()
{
    if ( [ _view isKindOfClass:[ UIPickerView class ] ] )
    {
        return ( UIPickerView* )_view;
    }
    return nil;
}
#endif

#if TARGET_OS_IPHONE
@interface ofxGenericSelectorViewForwarder : NSObject< UIPickerViewDelegate, UIPickerViewDataSource >
{
@private
    ofPtrWeak< ofxGenericSelectorView > _forwardTo;
    
    vector< ofPtr< ofxGenericView > > _rowViewsCache; // we put row views in this cache to ensure they don't get deleted ... clients of this class should always create a new row view whenever the delegate method asks for it, rather than always returning the same view instance for the same row ... this is because of changes in how iOS 7 implements this class (that is, it will request more than one view for the same row and will present them on the screen simultaneously .. always returning the same view for a row will therefore cause visual glitches).
}
-( id )initWithForwardTo:( ofPtrWeak< ofxGenericSelectorView > )forwardTo;

@end
#endif

NativeView ofxGenericSelectorView::createNativeView( const ofRectangle& frame )
{
#if TARGET_OS_IPHONE
    UIPickerView* newView = [ [ UIPickerView alloc ] initWithFrame:ofxRectangleToCGRect( frame ) ];
    _forwarder = [ [ ofxGenericSelectorViewForwarder alloc ] initWithForwardTo:dynamic_pointer_cast< ofxGenericSelectorView >( _this ) ];
    [ newView setDelegate:_forwarder ];
    [ newView setDataSource:_forwarder ];
    newView.showsSelectionIndicator = YES;
    return newView;
#endif
}

void ofxGenericSelectorView::init( ofPtrWeak< ofxGenericSelectorView > setThis, const ofRectangle& setFrame, ofPtrWeak< ofxGenericSelectorViewDelegate > delegate )
{
    _delegate = delegate;
    ofxGenericView::init( setThis, setFrame );
}

#pragma mark Native Forwarder

#if TARGET_OS_IPHONE

@implementation ofxGenericSelectorViewForwarder

-( id )initWithForwardTo:( ofPtrWeak< ofxGenericSelectorView > ) forwardTo;
{
    self = [ super init ];
    if ( self )
    {
        _forwardTo = forwardTo;
    }
    return self;
}

// UIPickerViewDataSource
// TODO: deal with unsigned int to NSInteger issues
-( NSInteger )numberOfComponentsInPickerView:( UIPickerView* )pickerView
{
    if ( _forwardTo )
    {
        return _forwardTo.lock()->getNumberOfComponents();
    }
    return 0;
}

-( NSInteger )pickerView:( UIPickerView* )pickerView numberOfRowsInComponent:( NSInteger )component
{
    if ( _forwardTo )
    {
        return _forwardTo.lock()->getNumberOfRowsInComponent( component );
    }
    return 0;
}

// UIPickerViewDelegate
-( CGFloat )pickerView:( UIPickerView* )pickerView widthForComponent:( NSInteger )component
{
    if ( _forwardTo )
    {
        return ( CGFloat )_forwardTo.lock()->getWidthOfComponent( component );
    }
    return 0.0f;
}

-( CGFloat )pickerView:( UIPickerView* )pickerView rowHeightForComponent:( NSInteger )component
{
    if ( _forwardTo )
    {
        return ( CGFloat )_forwardTo.lock()->getHeightOfRowsInComponent( component );
    }
    return 0.0f;
}

/*-( NSString* )pickerView:( UIPickerView* )pickerView titleForRow:( NSInteger )row forComponent:( NSInteger )component
{
}
*/

-( UIView* )pickerView:( UIPickerView* )pickerView viewForRow:( NSInteger )row forComponent:( NSInteger )component reusingView:( UIView* )view
{
    if ( _forwardTo )
    {
        ofPtr< ofxGenericView > viewForRow = _forwardTo.lock()->getViewForRowInComponent( component, row );
        if ( viewForRow )
        {
            _rowViewsCache.push_back( viewForRow );
            return viewForRow->getNativeView();
        }
    }
    return nil;
}

-( void )pickerView:( UIPickerView* )pickerView didSelectRow:( NSInteger )row inComponent:( NSInteger )component
{
    if ( _forwardTo )
    {
        _forwardTo.lock()->selectedRow( component, row );
    }
}

@end

#endif
