//
//  ofxGenericUtility.mm
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 12/30/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#include "ofxGenericUtility.h"



NSString* pathToBundle( NSString* resourceFileName )
{
    NSString* bundlePath = [ [ NSBundle mainBundle ] resourcePath ];
    if ( resourceFileName != nil )
    {
        return [ NSString stringWithFormat:@"%@/%@", bundlePath, resourceFileName ];
    }
    return [ NSString stringWithFormat:@"%@/", bundlePath ];
}

CGFloat getWindowScale()
{
    if ( [ [ UIScreen mainScreen ] respondsToSelector:@selector( scale ) ] ) 
    {
        return [ [ UIScreen mainScreen ] scale ];
    }
    return 1;
}

CGRect ofRectangleToCGRect( const ofRectangle& from )
{
    return CGRectMake( from.x, from.y, from.width, from.height );
}

ofRectangle CGRectToofRectangle( const CGRect& from )
{
    return ofRectangle( from.origin.x, from.origin.y, from.size.width, from.size.height );
}

UITextAlignment ofxGenericTextHorizontalAlignmentToUITextAlignment( ofxGenericTextHorizontalAlignment from )
{
    switch ( from )
    {
        case ofxGenericTextHorizontalAlignmentLeft:
            return UITextAlignmentLeft;
        case ofxGenericTextHorizontalAlignmentCenter:
            return UITextAlignmentCenter;
        case ofxGenericTextHorizontalAlignmentRight:
            return UITextAlignmentRight;
    }
    return UITextAlignmentLeft;
}
UITableViewCellSeparatorStyle ofxGenericTableViewSeparatorStyleToiOS( ofxGenericTableViewSeparatorStyle from )
{
    switch ( from )
    {
        case ofxGenericTableViewSeparatorStyleNone:
            return UITableViewCellSeparatorStyleNone;
        case ofxGenericTableViewSeparatorStyleSingleLine:
            return UITableViewCellSeparatorStyleSingleLine;
        case ofxGenericTableViewSeparatorStyleSingleLineEtched:
            return UITableViewCellSeparatorStyleSingleLineEtched;
    }
    return UITableViewCellSeparatorStyleSingleLine;    
}
