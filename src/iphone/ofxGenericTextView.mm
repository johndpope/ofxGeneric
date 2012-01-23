//
//  ofxGenericTextView.mm
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 12/29/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#import "ofxGenericTextView.h"
#import "ofxGenericUtility.h"

void ofxGenericTextView::setText( string newText )
{
    if ( [ _view isKindOfClass:[ UILabel class ] ] )
    {
        UILabel* labelView = ( UILabel* )_view;
        [ labelView setText:ofxStringToNSString( newText ) ];
    }    
}

string ofxGenericTextView::getText()
{
    if ( [ _view isKindOfClass:[ UILabel class ] ] )
    {
        UILabel* labelView = ( UILabel* )_view;
        return ofxNSStringToString( [ labelView text ] );
    }
    return string();
}

void ofxGenericTextView::setTextAlignment( ofxGenericTextHorizontalAlignment alignment )
{
    if ( [ _view isKindOfClass:[ UILabel class ] ] )
    {
        UILabel* labelView = ( UILabel* )_view;
        [ labelView setTextAlignment:ofxGenericTextHorizontalAlignmentToUITextAlignment( alignment ) ];
    }
}

UIView* ofxGenericTextView::createUIView( const CGRect& frame )
{
    UILabel* newView = [ [ UILabel alloc ] initWithFrame:frame ];
    [ newView setBackgroundColor:[ UIColor clearColor ] ];
    [ newView setTextColor:[ UIColor blackColor ] ];
    return newView;
}ofxGenericUIViewCastOperator( ofxGenericTextView, UILabel );

ofxGenericUIViewCastOperator( ofxGenericTextView, UILabel );
