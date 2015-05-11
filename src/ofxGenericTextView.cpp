//
//  ofxGenericTextView.cpp
//
//  Created by Ian Grossberg on 12/29/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#include "ofxGenericTextView.h"

#include "ofxGenericUtility.h"
#include "ofxGenericFontManager.h"
#include "ofxGenericFont.h"

#import "ofCommon.h"

#if TARGET_OS_IPHONE
static Class nativeClass = Nil;
#endif

ofPtr< ofxGenericTextView > ofxGenericTextView::create( const ofRectangle& setFrame, NativeView nativeView )
{
    ofPtr< ofxGenericTextView > create = ofPtr< ofxGenericTextView >( new ofxGenericTextView() );
    create->init( create, setFrame, nativeView );
    return create;
}

#if TARGET_OS_IPHONE
void ofxGenericTextView::setNativeViewClass( Class nativeViewClass )
{
    if ([nativeViewClass isSubclassOfClass:[UILabel class]])
    {
        nativeClass = nativeViewClass;
    }
}
#endif

NativeView ofxGenericTextView::createNativeView( const ofRectangle& frame )
{
#if TARGET_OS_IPHONE
    Class labelClass = nativeClass ? nativeClass : [UILabel class];
    UILabel* newView = [ [ labelClass alloc ] initWithFrame:ofxRectangleToCGRect( frame ) ];
    [ newView setBackgroundColor:[ UIColor clearColor ] ];
    [ newView setTextColor:[ UIColor blackColor ] ];
    [ newView setLineBreakMode:ofxGenericTextLinebreakModeToiOS( ofxGenericTextLinebreakModeWordWrap ) ];
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

ofxGenericTextView::operator string()
{
    return getText();
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
        [ labelView setTextColor:ofxColorToUIColor( setColor ) ];
    }
#endif
}

ofColor ofxGenericTextView::getTextColor ()
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UILabel class ] ] )
    {
        UILabel* labelView = ( UILabel* )_view;
        return ofxUIColorToofColor( labelView.textColor );
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

void ofxGenericTextView::setFont( string name, float size )
{
    ofPtr< ofxGenericFont > font = ofxGenericFontManager::getInstance().getLocalizedFont(name, size);
    setFont( font );
}

void ofxGenericTextView::setFont( ofPtr< ofxGenericFont > font )
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UILabel class ] ] )
    {
        UILabel* labelView = ( UILabel* )_view;
        [ labelView setFont:font->getNativeFont() ];
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

float ofxGenericTextView::getMinimumFontSize()
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UILabel class ] ] )
    {
        return ( float )((UILabel *)_view).minimumFontSize;
    }
#endif
    return 0;
}

void ofxGenericTextView::setMinimumFontSize( float s )
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UILabel class ] ] )
    {
        ((UILabel *)_view).minimumFontSize = ( CGFloat )s;
    }
#endif
}

void ofxGenericTextView::copyProperties( ofPtr< ofxGenericTextView > from )
{
    ofxGenericView::copyProperties( from );
    
    if ( from )
    {
        setText( from->getText() );
        setTextAlignment( from->getTextAlignment() );
        setTextColor( from->getTextColor() );
        setLineBreakMode( from->getLineBreakMode() );
        setNumberOfLines( from->getNumberOfLines() );
        setMinimumFontSize( from->getMinimumFontSize() );
        setFont( from->getFontName(), from->getFontSize() );
        setAutosizeFontToFitText( from->getAutosizeFontToFitText() );
    }
}

#if TARGET_OS_IPHONE
ofxGenericUIViewCastOperator( ofxGenericTextView, UILabel );
#endif

string ofxGenericTextView::toString()
{
    string result = ofxGenericView::toString();
    
    string text = getText();
    if ( !text.empty() )
    {
        result += " " + text;
    }
    return result;
}

ofPtr< ofxGenericValueStore > ofxGenericTextView::toValueStore()
{
    ofPtr< ofxGenericValueStore > result = ofxGenericView::toValueStore();
    if ( result )
    {
        if ( !getText().empty() )
        {
            result->write( "text", getText() );
        }
    }
    return result;
}
