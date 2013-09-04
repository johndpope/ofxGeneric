//
//  ofxGenericSelectorView.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 6/23/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once


#include "ofxGenericView.h"

#if TARGET_OS_IPHONE
@class ofxGenericSelectorViewForwarder;
#endif
class ofxGenericSelectorViewDelegate;

class ofxGenericSelectorView : public ofxGenericView
{
public:
    static ofPtr< ofxGenericSelectorView > create( const ofRectangle& setFrame = ofRectangle(), ofPtr< ofxGenericSelectorViewDelegate > delegate = ofPtr< ofxGenericSelectorViewDelegate >() );
#if TARGET_OS_IPHONE
    operator UIPickerView*();
#endif
    
    virtual ~ofxGenericSelectorView();
    
    virtual unsigned int getNumberOfComponents();
    virtual unsigned int getNumberOfRowsInComponent( unsigned int component );
    
    virtual float getWidthOfComponent( unsigned int component );
    virtual float getHeightOfRowsInComponent( unsigned int component );
    
    virtual ofPtr< ofxGenericView > getViewForRowInComponent( unsigned int component, unsigned int row );

    virtual void setFocusOn();
    
    virtual void scrollToRow( unsigned int component, unsigned int row, bool animated = true );
    
    virtual void selectedRow( unsigned int component, unsigned int row );
    
    virtual void reloadData();
    
    void setShowSelectionOverlay( bool showSelectionOverlay );
    bool getShowSelectionOverlay();
    
protected:
    virtual NativeView createNativeView( const ofRectangle& frame );
#if TARGET_OS_IPHONE
    ofxGenericSelectorViewForwarder* _forwarder;
#endif
    virtual void init( ofPtrWeak< ofxGenericSelectorView > setThis, const ofRectangle& setFrame, ofPtrWeak< ofxGenericSelectorViewDelegate > delegate );
    ofPtrWeak< ofxGenericSelectorViewDelegate > _delegate;
};

class ofxGenericSelectorViewDelegate
{
public:
    virtual ~ofxGenericSelectorViewDelegate() {};

    virtual unsigned int selectorView_getNumberOfComponents( ofPtr< ofxGenericSelectorView > selectorView ) { return 0; };
    virtual unsigned int selectorView_getNumberOfRowsInComponent( ofPtr< ofxGenericSelectorView > selectorView, unsigned int component ) { return 0; };
    
    virtual float selectorView_getWidthOfComponent( ofPtr< ofxGenericSelectorView > selectorView, unsigned int component ) { return 0.0f; };
    virtual float selectorView_getHeightOfRowsInComponent( ofPtr< ofxGenericSelectorView > selectorView, unsigned int component ) { return 0.0f; };
    
    // clients of this class should always create a new row view whenever the delegate method asks for it,
    // rather than always returning the same view instance for the same row ... this is because of changes 
    // in how iOS 7 implements this class (that is, it will request more than one view for the same row and 
    // will present them on the screen simultaneously .. always returning the same view for a row will 
    // therefore cause visual glitches).
    virtual ofPtr< ofxGenericView > selectorView_getViewForRowInComponent( ofPtr< ofxGenericSelectorView > selectorView, unsigned int component, unsigned int row ) { return ofPtr< ofxGenericView >(); };  
    
    virtual void selectorView_selectedRow( ofPtr< ofxGenericSelectorView > selectorView, unsigned int component, unsigned int row ) {};
};
