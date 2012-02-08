//
//  ofxGenericImageView.cpp
//
//  Created by Ian Grossberg on 1/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericImageView.h"
#include "ofxGenericUtility.h"

#if TARGET_OS_IPHONE
UIView* ofxGenericImageView::createUIView( const CGRect& frame )
{
    UIImageView* newView = [ [ UIImageView alloc ] initWithFrame:frame ];
    [ newView setBackgroundColor:[ UIColor clearColor ] ];
    return newView;
}
#endif

// TODO: auto @2x if available
void ofxGenericImageView::setImage( string fileName )
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UIImageView class ] ] )
    {
        UIImageView* view = ( UIImageView* )_view;
        [ view setImage:[ UIImage imageWithContentsOfFile:pathToBundle( ofxStringToNSString( fileName ) )  ] ];
    }
#endif
}
#if TARGET_OS_IPHONE
ofxGenericUIViewCastOperator( ofxGenericImageView, UIImageView );
#endif
