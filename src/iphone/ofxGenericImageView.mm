//
//  ofxGenericImageView.mm
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 1/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#import "ofxGenericImageView.h"
#import "ofxGenericUtility.h"

UIView* ofxGenericImageView::createUIView( const CGRect& frame )
{
    UIImageView* newView = [ [ UIImageView alloc ] initWithFrame:frame ];
    [ newView setBackgroundColor:[ UIColor clearColor ] ];
    return newView;
}

// TODO: auto @2x if available
void ofxGenericImageView::setImage( string fileName )
{
    if ( [ _view isKindOfClass:[ UIImageView class ] ] )
    {
        UIImageView* view = ( UIImageView* )_view;
        [ view setImage:[ UIImage imageWithContentsOfFile:pathToBundle( ofxStringToNSString( fileName ) )  ] ];
    }
}
ofxGenericUIViewCastOperator( ofxGenericImageView, UIImageView );