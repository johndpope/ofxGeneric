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

UITextAlignment ofxGenericTextHorizontalAlignmentToiOS( ofxGenericTextHorizontalAlignment from )
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

ofxGenericTextHorizontalAlignment iOSToofxGenericTextHorizontalAlignment( UITextAlignment from )
{
    switch ( from )
    {
        case UITextAlignmentLeft:
            return ofxGenericTextHorizontalAlignmentLeft;
        case UITextAlignmentCenter:
            return ofxGenericTextHorizontalAlignmentCenter;
        case UITextAlignmentRight:
            return ofxGenericTextHorizontalAlignmentRight;
    }
    return ofxGenericTextHorizontalAlignmentLeft;
}

UILineBreakMode ofxGenericTextLinebreakModeToiOS( ofxGenericTextLinebreakMode from )
{
    switch ( from )
    {
        case ofxGenericTextLinebreakModeWordWrap:
            return UILineBreakModeWordWrap;
        case ofxGenericTextLinebreakModeCharacterWrap:
            return UILineBreakModeCharacterWrap;
        case ofxGenericTextLinebreakModeClip:
            return UILineBreakModeClip;
        case ofxGenericTextLinebreakModeHeadTruncation:
            return UILineBreakModeHeadTruncation;
        case ofxGenericTextLinebreakModeTailTruncation:
            return UILineBreakModeTailTruncation;
        case ofxGenericTextLinebreakModeMiddleTruncation:
            return UILineBreakModeMiddleTruncation;
    }
    return UILineBreakModeWordWrap;
}

ofxGenericTextLinebreakMode iOSToofxGenericTextLinebreakMode( UILineBreakMode from )
{
    switch ( from )
    {
        case UILineBreakModeWordWrap:
            return ofxGenericTextLinebreakModeWordWrap;
        case UILineBreakModeCharacterWrap:
            return ofxGenericTextLinebreakModeCharacterWrap;
        case UILineBreakModeClip:
            return ofxGenericTextLinebreakModeClip;
        case UILineBreakModeHeadTruncation:
            return ofxGenericTextLinebreakModeHeadTruncation;
        case UILineBreakModeTailTruncation:
            return ofxGenericTextLinebreakModeTailTruncation;
        case UILineBreakModeMiddleTruncation:
            return ofxGenericTextLinebreakModeMiddleTruncation;
    }
    return ofxGenericTextLinebreakModeWordWrap;
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

UIReturnKeyType ofxGenericKeyboardReturnKeyToiOS( ofxGenericKeyboardReturnKey from )
{
    switch( from )
    {
        case ofxGenericKeyboardReturnKeyDefault:
            return UIReturnKeyDefault;
        case ofxGenericKeyboardReturnKeyGo:
            return UIReturnKeyGo;
            //    ofxGenericKeyboardReturnKeyGoogle, //UIReturnKeyGoogle,
            //    ofxGenericKeyboardReturnKeyJoin, UIReturnKeyJoin,
        case ofxGenericKeyboardReturnKeyNext:
            return UIReturnKeyNext;
            //    ofxGenericKeyboardReturnKeyRoute, //UIReturnKeyRoute,
        case ofxGenericKeyboardReturnKeySearch:
            return UIReturnKeySearch;
        case ofxGenericKeyboardReturnKeySend:
            return UIReturnKeySend;
            //    ofxGenericKeyboardReturnKeyYahoo, UIReturnKeyYahoo,
        case ofxGenericKeyboardReturnKeyDone:
            return UIReturnKeyDone;
            //ofxGenericKeyboardReturnKeyEmergencyCall //UIReturnKeyEmergencyCall,
    }
    return UIReturnKeyDefault;
}

ofxGenericKeyboardReturnKey iOSToofxGenericKeyboardReturnKey( UIReturnKeyType from )
{
    switch( from )
    {
        case UIReturnKeyDefault:
            return ofxGenericKeyboardReturnKeyDefault;
        case UIReturnKeyGo:
            return ofxGenericKeyboardReturnKeyGo;
            //    ofxGenericKeyboardReturnKeyGoogle, //UIReturnKeyGoogle,
            //    ofxGenericKeyboardReturnKeyJoin, UIReturnKeyJoin,
        case UIReturnKeyNext:
            return ofxGenericKeyboardReturnKeyNext;
            //    ofxGenericKeyboardReturnKeyRoute, //UIReturnKeyRoute,
        case UIReturnKeySearch:
            return ofxGenericKeyboardReturnKeySearch;
        case UIReturnKeySend:
            return ofxGenericKeyboardReturnKeySend;
            //    ofxGenericKeyboardReturnKeyYahoo, UIReturnKeyYahoo,
        case UIReturnKeyDone:
            return ofxGenericKeyboardReturnKeyDone;
            //ofxGenericKeyboardReturnKeyEmergencyCall //UIReturnKeyEmergencyCall,
        default:
            return ofxGenericKeyboardReturnKeyDefault;
    }
    return ofxGenericKeyboardReturnKeyDefault;
}

UIKeyboardType ofxGenericKeyboardTypeToiOS( ofxGenericKeyboardType from )
{
    switch ( from )
    {
        case ofxGenericKeyboardTypeDefault:
            return UIKeyboardTypeDefault;                // Default type for the current input method.
        case ofxGenericKeyboardTypeASCIICapable:
            return UIKeyboardTypeASCIICapable;           // Displays a keyboard which can enter ASCII characters, non-ASCII keyboards remain active
        case ofxGenericKeyboardTypeNumbersAndPunctuation:
            return UIKeyboardTypeNumbersAndPunctuation;  // Numbers and assorted punctuation.
        case ofxGenericKeyboardTypeURL:
            return UIKeyboardTypeURL;                    // A type optimized for URL entry (shows . / .com prominently).
        case ofxGenericKeyboardTypeNumberPad:
            return UIKeyboardTypeNumberPad;              // A number pad (0-9). Suitable for PIN entry.
        case ofxGenericKeyboardTypePhonePad:
            return UIKeyboardTypePhonePad;               // A phone pad (1-9, *, 0, #, with letters under the numbers).
        case ofxGenericKeyboardTypeNamePhonePad:
            return UIKeyboardTypeNamePhonePad;           // A type optimized for entering a person's name or phone number.
        case ofxGenericKeyboardTypeEmailAddress:
            return UIKeyboardTypeEmailAddress;           // A type optimized for multiple email address entry (shows space @ . prominently).
#if __IPHONE_4_1 <= __IPHONE_OS_VERSION_MAX_ALLOWED
        case ofxGenericKeyboardTypeDecimalPad:
            return UIKeyboardTypeDecimalPad;             // A number pad with a decimal point.
#endif
#if __IPHONE_5_0 <= __IPHONE_OS_VERSION_MAX_ALLOWED
        case ofxGenericKeyboardTypeTwitter:
            return UIKeyboardTypeTwitter;                // A type optimized for twitter text entry (easy access to @ #)
#endif
    }
}

ofxGenericKeyboardType iOSToofxGenericKeyboardType( UIKeyboardType from )
{
    switch ( from )
    {
        case UIKeyboardTypeDefault:                // Default type for the current input method.
            return ofxGenericKeyboardTypeDefault;
        case UIKeyboardTypeASCIICapable:           // Displays a keyboard which can enter ASCII characters, non-ASCII keyboards remain active
            return ofxGenericKeyboardTypeASCIICapable;
        case UIKeyboardTypeNumbersAndPunctuation:  // Numbers and assorted punctuation.
            return ofxGenericKeyboardTypeNumbersAndPunctuation;
        case UIKeyboardTypeURL:                    // A type optimized for URL entry (shows . / .com prominently).
            return ofxGenericKeyboardTypeURL;
        case UIKeyboardTypeNumberPad:              // A number pad (0-9). Suitable for PIN entry.
            return ofxGenericKeyboardTypeNumberPad;
        case UIKeyboardTypePhonePad:               // A phone pad (1-9, *, 0, #, with letters under the numbers).
            return ofxGenericKeyboardTypePhonePad;
        case UIKeyboardTypeNamePhonePad:           // A type optimized for entering a person's name or phone number.
            return ofxGenericKeyboardTypeNamePhonePad;
        case UIKeyboardTypeEmailAddress:           // A type optimized for multiple email address entry (shows space @ . prominently).
            return ofxGenericKeyboardTypeEmailAddress;
#if __IPHONE_4_1 <= __IPHONE_OS_VERSION_MAX_ALLOWED
        case UIKeyboardTypeDecimalPad:             // A number pad with a decimal point.
            return ofxGenericKeyboardTypeDecimalPad;
#endif
#if __IPHONE_5_0 <= __IPHONE_OS_VERSION_MAX_ALLOWED
        case UIKeyboardTypeTwitter:                // A type optimized for twitter text entry (easy access to @ #)
            return ofxGenericKeyboardTypeTwitter;
#endif
    }
}

UITextAutocapitalizationType ofxGenericTextAutoCapitalizationToiOS( ofxGenericTextAutoCapitalization from )
{
    switch ( from )
    {
        case ofxGenericTextAutoCapitalizationNone:
            return UITextAutocapitalizationTypeNone;
        case ofxGenericTextAutoCapitalizationWords:
            return UITextAutocapitalizationTypeWords;
        case ofxGenericTextAutoCapitalizationSentences:
            return UITextAutocapitalizationTypeSentences;
        case ofxGenericTextAutoCapitalizationAllCharacters:
            return UITextAutocapitalizationTypeAllCharacters;
    }
    return UITextAutocapitalizationTypeNone;
}

ofxGenericTextAutoCapitalization iOSToofxGenericTextAutoCapitalization( UITextAutocapitalizationType from )
{
    switch ( from )
    {
        case UITextAutocapitalizationTypeNone:
            return ofxGenericTextAutoCapitalizationNone;
        case UITextAutocapitalizationTypeWords:
            return ofxGenericTextAutoCapitalizationWords;
        case UITextAutocapitalizationTypeSentences:
            return ofxGenericTextAutoCapitalizationSentences;
        case UITextAutocapitalizationTypeAllCharacters:
            return ofxGenericTextAutoCapitalizationAllCharacters;
    }
    return ofxGenericTextAutoCapitalizationNone;
}

UIDeviceOrientation ofOrientationToiOS( ofOrientation from )
{
    switch ( from ) 
    {
        case OF_ORIENTATION_UNKNOWN:
            return UIDeviceOrientationUnknown;
        case OF_ORIENTATION_DEFAULT:
            return UIDeviceOrientationPortrait;
        case OF_ORIENTATION_180:
            return UIDeviceOrientationPortraitUpsideDown;
        case OF_ORIENTATION_90_RIGHT:
            return UIDeviceOrientationLandscapeLeft;
        case OF_ORIENTATION_90_LEFT:
            return UIDeviceOrientationLandscapeRight;
            // TODO:
//        case ofxGenericOrientationFaceUp:
//            return UIDeviceOrientationFaceUp;
//        case ofxGenericOrientationFaceDown:
//            return UIDeviceOrientationFaceDown;
    }
    return UIDeviceOrientationUnknown;
}

ofOrientation iOSToofOrientation( UIDeviceOrientation from )
{
    switch ( from ) 
    {
        case UIDeviceOrientationUnknown:
            return OF_ORIENTATION_UNKNOWN;
        case UIDeviceOrientationPortrait:
            return OF_ORIENTATION_DEFAULT;
        case UIDeviceOrientationPortraitUpsideDown:
            return OF_ORIENTATION_180;
        case UIDeviceOrientationLandscapeLeft:
            return OF_ORIENTATION_90_RIGHT;
        case UIDeviceOrientationLandscapeRight:
            return OF_ORIENTATION_90_LEFT;
// TODO:
//        case UIDeviceOrientationFaceUp:
//            return ofxGenericOrientationFaceUp;
//        case UIDeviceOrientationFaceDown:
//            return ofxGenericOrientationFaceDown;
        default:
            return OF_ORIENTATION_UNKNOWN;
    }
    return OF_ORIENTATION_UNKNOWN;
}

UIInterfaceOrientation ofInterfaceOrientationToiOS( ofOrientation from )
{
    switch ( from ) 
    {
        case OF_ORIENTATION_UNKNOWN:
            return UIInterfaceOrientationPortrait;
        case OF_ORIENTATION_DEFAULT:
            return UIInterfaceOrientationPortrait;
        case OF_ORIENTATION_180:
            return UIInterfaceOrientationPortraitUpsideDown;
        case OF_ORIENTATION_90_RIGHT:
            return UIInterfaceOrientationLandscapeLeft;
        case OF_ORIENTATION_90_LEFT:
            return UIInterfaceOrientationLandscapeRight;
    }
    return UIInterfaceOrientationPortrait;    
}

ofOrientation iOSToofOrientation( UIInterfaceOrientation from )
{
    switch ( from ) 
    {
        case UIInterfaceOrientationPortrait:
            return OF_ORIENTATION_DEFAULT;
        case UIInterfaceOrientationPortraitUpsideDown:
            return OF_ORIENTATION_180;
        case UIInterfaceOrientationLandscapeLeft:
            return OF_ORIENTATION_90_RIGHT;
        case UIInterfaceOrientationLandscapeRight:
            return OF_ORIENTATION_90_LEFT;
        default:
            return OF_ORIENTATION_DEFAULT;
    }
    return OF_ORIENTATION_DEFAULT;    
}

UIViewAnimationCurve ofxGenericViewAnimationCurveToiOS( ofxGenericViewAnimationCurve from )
{
    switch( from )
    {
        case ofxViewAnimationCurveEaseInOut:
            return UIViewAnimationCurveEaseInOut;
        case ofxViewAnimationCurveEaseIn:
            return UIViewAnimationCurveEaseIn;
        case ofxViewAnimationCurveEaseOut:
            return UIViewAnimationCurveEaseOut;
        case ofxViewAnimationCurveLinear:
            return UIViewAnimationCurveLinear;
    }
    return UIViewAnimationCurveLinear;
}

UIViewAnimationTransition ofxGenericViewAnimationTransitionToiOS( ofxGenericViewAnimationTransition from )
{
    switch ( from ) 
    {
        case ofxGenericViewAnimationTransitionNone:
            return UIViewAnimationTransitionNone;
        case ofxGenericViewAnimationTransitionFlipFromLeft:
            return UIViewAnimationTransitionFlipFromLeft;
        case ofxGenericViewAnimationTransitionFlipFromRight:
            return UIViewAnimationTransitionFlipFromRight;
        case ofxGenericViewAnimationTransitionCurlUp:
            return UIViewAnimationTransitionCurlUp;
        case ofxGenericViewAnimationTransitionCurlDown:
            return UIViewAnimationTransitionCurlDown;
    }
    return UIViewAnimationTransitionNone;
}

UIViewAutoresizing ofxGenericViewAutoresizingToiOS( ofxGenericViewAutoresizing from )
{
    UIViewAutoresizing to = 0;
    if ( from & ofxGenericViewAutoresizingLeftMargin )
    {
        to |= UIViewAutoresizingFlexibleLeftMargin;
    }
    if ( from & ofxGenericViewAutoresizingFlexibleWidth )
    {
        to |= UIViewAutoresizingFlexibleWidth;
    }
    if ( from & ofxGenericViewAutoresizingRightMargin )
    {
        to |= UIViewAutoresizingFlexibleRightMargin;
    }
    if ( from & ofxGenericViewAutoresizingTopMargin )
    {
        to |= UIViewAutoresizingFlexibleTopMargin;
    }
    if ( from & ofxGenericViewAutoresizingFlexibleHeight )
    {
        to |= UIViewAutoresizingFlexibleHeight;
    }
    if ( from & ofxGenericViewAutoresizingBottomMargin )
    {
        to |= UIViewAutoresizingFlexibleBottomMargin;
    }
    return to;
}

ofxGenericViewAutoresizing iOSToofxGenericViewAutoresizing( UIViewAutoresizing from )
{
    int to = 0;
    if ( from & UIViewAutoresizingFlexibleLeftMargin )
    {
        to |= ofxGenericViewAutoresizingLeftMargin;
    }
    if ( from & UIViewAutoresizingFlexibleWidth )
    {
        to |= ofxGenericViewAutoresizingFlexibleWidth;
    }
    if ( from & UIViewAutoresizingFlexibleRightMargin )
    {
        to |= ofxGenericViewAutoresizingRightMargin;
    }
    if ( from & UIViewAutoresizingFlexibleTopMargin )
    {
        to |= ofxGenericViewAutoresizingTopMargin;
    }
    if ( from & UIViewAutoresizingFlexibleHeight )
    {
        to |= ofxGenericViewAutoresizingFlexibleHeight;
    }
    if ( from & UIViewAutoresizingFlexibleBottomMargin )
    {
        to |= ofxGenericViewAutoresizingBottomMargin;
    }
    return ( ofxGenericViewAutoresizing )to;
}

UITextBorderStyle ofxGenericTextViewBorderStyleToiOS( ofxGenericTextViewBorderStyle from )
{
    switch ( from )
    {
        case ofxGenericTextViewBorderNone:
            return UITextBorderStyleNone;
        case ofxGenericTextViewBorderLine:
            return UITextBorderStyleLine;
        case ofxGenericTextViewBorderBezel:
            return UITextBorderStyleBezel;
        case ofxGenericTextViewBorderRoundedRect:
            return UITextBorderStyleRoundedRect;
    }
    return UITextBorderStyleNone;
}

ofxGenericTextViewBorderStyle iOSToofxGenericTextViewBorderStyle( UITextBorderStyle from )
{
    switch( from )
    {
        case UITextBorderStyleNone:
            return ofxGenericTextViewBorderNone;
        case UITextBorderStyleLine:
            return ofxGenericTextViewBorderLine;
        case UITextBorderStyleBezel:
            return ofxGenericTextViewBorderBezel;
        case UITextBorderStyleRoundedRect:
            return ofxGenericTextViewBorderRoundedRect;
    }
    return ofxGenericTextViewBorderNone;
}

UISwipeGestureRecognizerDirection ofxGenericGestureTypeSwipeToiOS( ofxGenericGestureTypeSwipe from )
{
    int to = 0;
    if ( from & ofxGenericGestureTypeSwipeLeft )
    {
        to |= UISwipeGestureRecognizerDirectionLeft;
    }
    if ( from & ofxGenericGestureTypeSwipeRight )
    {
        to |= UISwipeGestureRecognizerDirectionRight;
    }
    if ( from & ofxGenericGestureTypeSwipeUp )
    {
        to |= UISwipeGestureRecognizerDirectionUp;
    }
    if ( from & ofxGenericGestureTypeSwipeDown )
    {
        to |= UISwipeGestureRecognizerDirectionDown;
    }
    return ( UISwipeGestureRecognizerDirection )to;
}

ofxGenericGestureTypeSwipe iOSToofxGenericGestureTypeSwipe( UISwipeGestureRecognizerDirection from )
{
    int to = 0;
    if ( from & UISwipeGestureRecognizerDirectionLeft )
    {
        to |= ofxGenericGestureTypeSwipeLeft;
    }
    if ( from & UISwipeGestureRecognizerDirectionRight )
    {
        to |= ofxGenericGestureTypeSwipeRight;
    }
    if ( from & UISwipeGestureRecognizerDirectionUp )
    {
        to |= ofxGenericGestureTypeSwipeUp;
    }
    if ( from & UISwipeGestureRecognizerDirectionDown )
    {
        to |= ofxGenericGestureTypeSwipeDown;
    }
    return ( ofxGenericGestureTypeSwipe )to;
}

float ofxGFontSizeForText( string text, string fontName, float startingFontSize, const ofPoint& constrainedSize )
{
    // http://stackoverflow.com/questions/4382976/multiline-uilabel-with-adjustsfontsizetofitwidth
    CGFloat fontSize = startingFontSize;
    NSString* nsFontName = [ NSString stringWithCString:fontName.c_str() encoding:NSUTF8StringEncoding ];
    UIFont* font = [ UIFont fontWithName:nsFontName size:fontSize ];
    
    NSString* nsText = [ NSString stringWithCString:text.c_str() encoding:NSUTF8StringEncoding ];
    CGFloat height = [ nsText sizeWithFont:font constrainedToSize:CGSizeMake( constrainedSize.x, FLT_MAX ) lineBreakMode:UILineBreakModeWordWrap ].height;
    UIFont *newFont = font;
    
    //Reduce font size while too large, break if no height (empty string)
    while ( height > constrainedSize.y && height != 0 ) 
    {   
        fontSize--;  
        newFont = [ UIFont fontWithName:nsFontName size:fontSize ];   
        height = [ nsText sizeWithFont:newFont constrainedToSize:CGSizeMake( constrainedSize.x, FLT_MAX ) lineBreakMode:UILineBreakModeWordWrap ].height;
    };
    
    // Loop through words in string and resize to fit
    for ( NSString* word in [ nsText componentsSeparatedByCharactersInSet:[ NSCharacterSet whitespaceAndNewlineCharacterSet ] ] ) 
    {
        CGFloat width = [ word sizeWithFont:newFont ].width;
        while ( width > constrainedSize.x && width != 0 ) 
        {
            fontSize--;
            newFont = [ UIFont fontWithName:nsFontName size:fontSize ];   
            width = [ word sizeWithFont:newFont ].width;
        }
    }
    return ( float )fontSize;   
}