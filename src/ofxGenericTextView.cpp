//
//  ofxGenericTextView.cpp
//
//  Created by Ian Grossberg on 12/29/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#include "ofxGenericTextView.h"
#include "ofxGenericUtility.h"

ofPtr< ofxGenericTextView > ofxGenericTextView::create( const ofRectangle& setFrame )
{
    ofPtr< ofxGenericTextView > create = ofPtr< ofxGenericTextView >( new ofxGenericTextView() );
    create->init( create, setFrame );
    return create;
}

void ofxGenericTextView::setText( string newText )
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UILabel class ] ] )
    {
        UILabel* labelView = ( UILabel* )_view;
        [ labelView setText:ofxStringToNSString( newText ) ];
        [ labelView sizeToFit ]; // TEMP:
    }
#endif
}

string ofxGenericTextView::getText()
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UILabel class ] ] )
    {
        UILabel* labelView = ( UILabel* )_view;
        return ofxNSStringToString( [ labelView text ] );
    }
    return string();
#endif
}

void ofxGenericTextView::setTextAlignment( ofxGenericTextHorizontalAlignment alignment )
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UILabel class ] ] )
    {
        UILabel* labelView = ( UILabel* )_view;
        [ labelView setTextAlignment:ofxGenericTextHorizontalAlignmentToiOS( alignment ) ];
    }
#endif
}

NativeView ofxGenericTextView::createNativeView( const ofRectangle& frame )
{
#if TARGET_OS_IPHONE
    UILabel* newView = [ [ UILabel alloc ] initWithFrame:ofRectangleToCGRect( frame ) ];
    [ newView setBackgroundColor:[ UIColor clearColor ] ];
    [ newView setTextColor:[ UIColor blackColor ] ];
    return newView;
#endif
}

#if TARGET_OS_IPHONE
ofxGenericUIViewCastOperator( ofxGenericTextView, UILabel );
#endif
