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

NativeView ofxGenericTextView::createNativeView( const ofRectangle& frame )
{
#if TARGET_OS_IPHONE
    UILabel* newView = [ [ UILabel alloc ] initWithFrame:ofRectangleToCGRect( frame ) ];
    [ newView setBackgroundColor:[ UIColor clearColor ] ];
    [ newView setTextColor:[ UIColor blackColor ] ];
    [ newView setLineBreakMode:UILineBreakModeWordWrap ];
    [ newView setNumberOfLines:0 ];
    return newView;
#endif
}

void ofxGenericTextView::setText( string newText )
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UILabel class ] ] )
    {
        UILabel* labelView = ( UILabel* )_view;
        [ labelView setText:ofxStringToNSString( newText ) ];
        if ( getAutosizeFontToFitText() && getNumberOfLines() != 1 )
        {
            autosizeFontForMultiline();
        }
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

ofxGenericTextHorizontalAlignment ofxGenericTextView::getTextAlignment()
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UILabel class ] ] )
    {
        UILabel* labelView = ( UILabel* )_view;
        return iOSToofxGenericTextHorizontalAlignment( labelView.textAlignment );
    }
#endif
    return ofxGenericTextHorizontalAlignmentLeft;
}

void ofxGenericTextView::setTextColor ( const ofColor& setColor )
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UILabel class ] ] )
    {
        UILabel* labelView = ( UILabel* )_view;
        [ labelView setTextColor:ofColorToUIColor( setColor ) ];
    }
#endif
}

ofColor ofxGenericTextView::getTextColor ()
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UILabel class ] ] )
    {
        UILabel* labelView = ( UILabel* )_view;
        return UIColorToofColor( labelView.textColor );
    }
#endif
    return ofColor(0, 0, 0);
}

void ofxGenericTextView::setLineBreakMode ( ofxGenericTextLinebreakMode mode )
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UILabel class ] ] )
    {
        UILabel* labelView = ( UILabel* )_view;
        [ labelView setLineBreakMode: ofxGenericTextLinebreakModeToiOS( mode ) ];
    }
#endif
}

ofxGenericTextLinebreakMode ofxGenericTextView::getLineBreakMode()
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UILabel class ] ] )
    {
        UILabel* labelView = ( UILabel* )_view;
        return iOSToofxGenericTextLinebreakMode( labelView.lineBreakMode );
    }
#endif
    return ofxGenericTextLinebreakModeWordWrap;
}

void ofxGenericTextView::setNumberOfLines ( int n )
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UILabel class ] ] )
    {
        UILabel* labelView = ( UILabel* )_view;
        [ labelView setNumberOfLines: n ];
    }
#endif
}

int ofxGenericTextView::getNumberOfLines ()
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UILabel class ] ] )
    {
        UILabel* labelView = ( UILabel* )_view;
        return labelView.numberOfLines;
    }
#endif
    return 0;
}

void ofxGenericTextView::setFont ( string name, float size )
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UILabel class ] ] )
    {
        UILabel* labelView = ( UILabel* )_view;
        [ labelView setFont: [UIFont fontWithName:[NSString stringWithCString:name.c_str() encoding:NSASCIIStringEncoding] size:size] ];
    }
#endif
}

float ofxGenericTextView::getFontSize()
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UILabel class ] ] )
    {
        UILabel* labelView = ( UILabel* )_view;
        return labelView.font.pointSize;
    }
#endif
    return 12.0f;
}

string ofxGenericTextView::getFontName()
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UILabel class ] ] )
    {
        UILabel* labelView = ( UILabel* )_view;
        return [labelView.font.fontName cStringUsingEncoding:NSASCIIStringEncoding];
    }
#endif
    return "Helvetica";
}


void ofxGenericTextView::setAutosizeFontToFitText( bool autosize )
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UILabel class ] ] )
    {
        UILabel* labelView = ( UILabel* )_view;
        [ labelView setAdjustsFontSizeToFitWidth:( BOOL )autosize ];
        if ( autosize && getNumberOfLines() != 1 )
        {
            autosizeFontForMultiline();
        }
    }
#endif
}

bool ofxGenericTextView::getAutosizeFontToFitText()
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UILabel class ] ] )
    {
        UILabel* labelView = ( UILabel* )_view;
        return ( bool )labelView.adjustsFontSizeToFitWidth;
    }
#endif
    return false;
}

#if TARGET_OS_IPHONE
void ofxGenericTextView::autosizeFontForMultiline()
{
    float newFontSize = ofxGFontSizeForText( getText(), getFontName(), getFontSize(), ofPoint( getFrame().width, getFrame().height ) );
    if ( newFontSize != getFontSize() )
    {
        setFont( getFontName(), newFontSize );
    }
}
#endif

                                
#if TARGET_OS_IPHONE
ofxGenericUIViewCastOperator( ofxGenericTextView, UILabel );
#endif
