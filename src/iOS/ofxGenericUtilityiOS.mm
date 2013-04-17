//
//  ofxGenericUtilityiOS.mm
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 12/30/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#include "ofxGenericUtilityiOS.h"

#include "ofxGenericUtility.h"
#include "ofUtils.h"

string ofxGToString( NSData* data )
{
    if ( !data )
    {
        return "";
    }
    return ofxGToString( [ data bytes ], ( unsigned int )[ data length ] );
}

string ofxGToString( NSString* string )
{
    if ( !string )
    {
        return "";
    }
    return [ string cStringUsingEncoding:NSUTF8StringEncoding ];
}

CGFloat getWindowScale()
{
    if ( [ [ UIScreen mainScreen ] respondsToSelector:@selector( scale ) ] ) 
    {
        return [ [ UIScreen mainScreen ] scale ];
    }
    return 1;
}

NSTextAlignment ofxGenericTextHorizontalAlignmentToiOS( ofxGenericTextHorizontalAlignment from )
{
    switch ( from )
    {
        case ofxGenericTextHorizontalAlignmentLeft:
            return NSTextAlignmentLeft;
        case ofxGenericTextHorizontalAlignmentCenter:
            return NSTextAlignmentCenter;
        case ofxGenericTextHorizontalAlignmentRight:
            return NSTextAlignmentRight;
    }
    return NSTextAlignmentLeft;
}

ofxGenericTextHorizontalAlignment iOSToofxGenericTextHorizontalAlignment( NSTextAlignment from )
{
    switch ( from )
    {
        case NSTextAlignmentLeft:
            return ofxGenericTextHorizontalAlignmentLeft;
        case NSTextAlignmentCenter:
            return ofxGenericTextHorizontalAlignmentCenter;
        case NSTextAlignmentRight:
            return ofxGenericTextHorizontalAlignmentRight;
    }
    return ofxGenericTextHorizontalAlignmentLeft;
}

NSLineBreakMode ofxGenericTextLinebreakModeToiOS( ofxGenericTextLinebreakMode from )
{
    switch ( from )
    {
        case ofxGenericTextLinebreakModeWordWrap:
            return NSLineBreakByWordWrapping;
        case ofxGenericTextLinebreakModeCharacterWrap:
            return NSLineBreakByCharWrapping;
        case ofxGenericTextLinebreakModeClip:
            return NSLineBreakByClipping;
        case ofxGenericTextLinebreakModeHeadTruncation:
            return NSLineBreakByTruncatingHead;
        case ofxGenericTextLinebreakModeTailTruncation:
            return NSLineBreakByTruncatingTail;
        case ofxGenericTextLinebreakModeMiddleTruncation:
            return NSLineBreakByTruncatingMiddle;
    }
    return NSLineBreakByWordWrapping;
}

ofxGenericTextLinebreakMode iOSToofxGenericTextLinebreakMode( NSLineBreakMode from )
{
    switch ( from )
    {
        case NSLineBreakByWordWrapping:
            return ofxGenericTextLinebreakModeWordWrap;
        case NSLineBreakByCharWrapping:
            return ofxGenericTextLinebreakModeCharacterWrap;
        case NSLineBreakByClipping:
            return ofxGenericTextLinebreakModeClip;
        case NSLineBreakByTruncatingHead:
            return ofxGenericTextLinebreakModeHeadTruncation;
        case NSLineBreakByTruncatingTail:
            return ofxGenericTextLinebreakModeTailTruncation;
        case NSLineBreakByTruncatingMiddle:
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
        case ofxGenericTableViewSeparatorStyleSizedPadding:
            return UITableViewCellSeparatorStyleNone;
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

UIActivityIndicatorViewStyle ofxGenericActivityViewStyleToiOS( ofxGenericActivityViewStyle style)
{
    switch (style) {
        case ofxGenericActivityViewStyleWhiteLarge:
            return UIActivityIndicatorViewStyleWhiteLarge;
            
        case ofxGenericActivityViewStyleWhite:
            return UIActivityIndicatorViewStyleWhite;
            
        case ofxGenericActivityViewStyleGray:
            return UIActivityIndicatorViewStyleGray;
            
        default:
            return UIActivityIndicatorViewStyleWhite;
    }
}

ofxGenericActivityViewStyle iOSToofxGenericActivityViewStyle( UIActivityIndicatorViewStyle style )
{
    switch (style)
    {
        case UIActivityIndicatorViewStyleWhiteLarge:
            return ofxGenericActivityViewStyleWhiteLarge;
            
        case UIActivityIndicatorViewStyleWhite:
            return ofxGenericActivityViewStyleWhite;
            
        case UIActivityIndicatorViewStyleGray:
            return ofxGenericActivityViewStyleGray;
            
        default:
            return ofxGenericActivityViewStyleWhite;
    }
    
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
    return ofxGenericKeyboardTypeDefault;
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

ofOrientation iOSDeviceToofOrientation( UIDeviceOrientation from )
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
        case ofxGenericViewAnimationCurveEaseInOut:
            return UIViewAnimationCurveEaseInOut;
        case ofxGenericViewAnimationCurveEaseIn:
            return UIViewAnimationCurveEaseIn;
        case ofxGenericViewAnimationCurveEaseOut:
            return UIViewAnimationCurveEaseOut;
        case ofxGenericViewAnimationCurveLinear:
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

UIButtonType ofxGenericButtonTypeToiOS( ofxGenericButtonType from )
{
    switch (from)
    {
        case ofxGenericButtonTypeCustom:
            return UIButtonTypeCustom;
        case ofxGenericButtonTypeRoundedRect:
            return UIButtonTypeRoundedRect;
        case ofxGenericButtonTypeDetailDisclosure:
            return UIButtonTypeDetailDisclosure;
        case ofxGenericButtonTypeInfoLight:
            return UIButtonTypeInfoLight;
        case ofxGenericButtonTypeInfoDark:
            return UIButtonTypeInfoDark;
        case ofxGenericButtonTypeContactAdd:
            return UIButtonTypeContactAdd;
    }
    return UIButtonTypeCustom;
}

ofxGenericButtonType iOSToofxGenericButtonType( UIButtonType from )
{
    switch (from)
    {
        case UIButtonTypeCustom:
            return ofxGenericButtonTypeCustom;
        case UIButtonTypeRoundedRect:
            return ofxGenericButtonTypeRoundedRect;
        case UIButtonTypeDetailDisclosure:
            return ofxGenericButtonTypeDetailDisclosure;
        case UIButtonTypeInfoLight:
            return ofxGenericButtonTypeInfoLight;
        case UIButtonTypeInfoDark:
            return ofxGenericButtonTypeInfoDark;
        case UIButtonTypeContactAdd:
            return ofxGenericButtonTypeContactAdd;
    }
    return ofxGenericButtonTypeCustom;
}

UIDatePickerMode ofxGenericDateSelectorModeToiOS( ofxGenericDateSelectorMode from )
{
    switch( from )
    {
        case ofxGenericDateSelectorModeTime:
            return UIDatePickerModeTime;
        case ofxGenericDateSelectorModeDate:
            return UIDatePickerModeDate;
        case ofxGenericDateSelectorModeDateAndTime:
            return UIDatePickerModeDateAndTime;
    }
    
    return UIDatePickerModeTime;
}

ofxGenericDateSelectorMode iOSToofxGenericDateSelectorMode( UIDatePickerMode from )
{
    switch( from )
    {
        case UIDatePickerModeTime:
            return ofxGenericDateSelectorModeTime;
        case UIDatePickerModeDate:
            return ofxGenericDateSelectorModeDate;
        case UIDatePickerModeDateAndTime:
            return ofxGenericDateSelectorModeDateAndTime;
            
        default: return ofxGenericDateSelectorModeTime;
    }
    
    return ofxGenericDateSelectorModeTime;
}

UIGestureRecognizerState ofxGenericGestureStateToiOS( ofxGenericGestureState from )
{
    switch ( from )
    {
        case ofxGenericGestureStateBegin:
            return UIGestureRecognizerStateBegan;
        case ofxGenericGestureStateChanged:
            return UIGestureRecognizerStateChanged;
        case ofxGenericGestureStateEnded:
            return UIGestureRecognizerStateEnded;
        case ofxGenericGestureStateCancelled:
            return UIGestureRecognizerStateCancelled;
        case ofxGenericGestureStateFailed:
            return UIGestureRecognizerStateFailed;
    }
    return UIGestureRecognizerStateFailed;
}

ofxGenericGestureState iOSToofxGenericGestureState( UIGestureRecognizerState from )
{
    switch ( from )
    {
        case UIGestureRecognizerStateBegan:
            return ofxGenericGestureStateBegin;
        case UIGestureRecognizerStateChanged:
            return ofxGenericGestureStateChanged;
        case UIGestureRecognizerStateEnded:
            return ofxGenericGestureStateEnded;
        case UIGestureRecognizerStateCancelled:
            return ofxGenericGestureStateCancelled;
        case UIGestureRecognizerStateFailed:
            return ofxGenericGestureStateFailed;
        default:
            return ofxGenericGestureStateFailed;
    }
    return ofxGenericGestureStateFailed;
}

ofPtr< ofImage > UIImageToofImage( UIImage* image )
{
    ofPtr< ofImage > outImage( new ofImage() );
    if ( ofxiPhoneUIImageToOFImage( image, *outImage ) )
    {
        return outImage;
    }
    return ofPtr< ofImage >();
}

UIImage* OFImageToUIImage( ofImage& image )
{
//    NSData* pixelData = [ NSData dataWithBytes:image.getPixels() length:image.width * image.height * image.bpp / 8 ];
    
//    CGImageRef imgRef = CGBitmapContextCreateImage( context );
//    return [ UIImage imageWithCGImage:imgRef ];
//    return [ UIImage imageWithData:pixelData ];
    unsigned char* pixels = image.getPixels();
    CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
    CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault | kCGImageAlphaLast;
    CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
    CGDataProviderRef provider = CGDataProviderCreateWithData( NULL, pixels, image.width * image.height * image.bpp / 8, NULL );
    CGImageRef imageRef = CGImageCreate( image.width, image.height, image.bpp / 4, image.bpp, image.width * image.bpp / 8, colorSpaceRef, bitmapInfo, provider, NULL, false, renderingIntent );
    return [ UIImage imageWithCGImage:imageRef ];
}

CGSize ofPointToCGSize( const ofPoint& size )
{
    return CGSizeMake( size.x, size.y );
}

ofPoint CGSizeToofPoint( CGSize size )
{
    return ofPoint( size.width, size.height );
}

UIViewContentMode ofxGenericContentModeToiOS( ofxGenericContentMode from )
{
    switch ( from )
    {
        case ofxGenericContentModeScaleToFill:
            return UIViewContentModeScaleToFill;
        case ofxGenericContentModeScaleAspectFit:
            return UIViewContentModeScaleAspectFit;
        case ofxGenericContentModeScaleAspectFill:
            return UIViewContentModeScaleAspectFill;
        case ofxGenericContentModeRedraw:
            return UIViewContentModeRedraw;
        case ofxGenericContentModeCenter:
            return UIViewContentModeCenter;
        case ofxGenericContentModeTop:
            return UIViewContentModeTop;
        case ofxGenericContentModeBottom:
            return UIViewContentModeBottom;
        case ofxGenericContentModeLeft:
            return UIViewContentModeLeft;
        case ofxGenericContentModeRight:
            return UIViewContentModeRight;
        case ofxGenericContentModeTopLeft:
            return UIViewContentModeTopLeft;
        case ofxGenericContentModeTopRight:
            return UIViewContentModeTopRight;
        case ofxGenericContentModeBottomLeft:
            return UIViewContentModeBottomLeft;
        case ofxGenericContentModeBottomRight:
            return UIViewContentModeBottomRight;
    }
    return UIViewContentModeScaleToFill;
}

ofxGenericContentMode iOSToofxGenericContentMode( UIViewContentMode from )
{
    switch ( from )
    {
        case UIViewContentModeScaleToFill:
            return ofxGenericContentModeScaleToFill;
        case UIViewContentModeScaleAspectFit:
            return ofxGenericContentModeScaleAspectFit;
        case UIViewContentModeScaleAspectFill:
            return ofxGenericContentModeScaleAspectFill;
        case UIViewContentModeRedraw:
            return ofxGenericContentModeRedraw;
        case UIViewContentModeCenter:
            return ofxGenericContentModeCenter;
        case UIViewContentModeTop:
            return ofxGenericContentModeTop;
        case UIViewContentModeBottom:
            return ofxGenericContentModeBottom;
        case UIViewContentModeLeft:
            return ofxGenericContentModeLeft;
        case UIViewContentModeRight:
            return ofxGenericContentModeRight;
        case UIViewContentModeTopLeft:
            return ofxGenericContentModeTopLeft;
        case UIViewContentModeTopRight:
            return ofxGenericContentModeTopRight;
        case UIViewContentModeBottomLeft:
            return ofxGenericContentModeBottomLeft;
        case UIViewContentModeBottomRight:
            return ofxGenericContentModeBottomRight;
    }
    return ofxGenericContentModeScaleToFill;
}

UIColor* ofColorToUIColor( const ofColor& color )
{
    //assume it's always 0-255 for now...
    return [ UIColor colorWithRed: color.r / 255.0f green: color.g / 255.0f blue: color.b / 255.0f alpha: color.a / 255.0f];
}

ofColor UIColorToofColor( UIColor* color )
{
    const float* colors = CGColorGetComponents( color.CGColor );
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    float a = CGColorGetAlpha( color.CGColor );
    
    //WTF BS black and gray cause this crapola
    if (CGColorGetNumberOfComponents( color.CGColor ) == 2)
    {
        r = g = b = colors[0];
    }
    else if (CGColorGetNumberOfComponents( color.CGColor ) >= 3)
    {
        r = colors[0];
        g = colors[1];
        b = colors[2];
    }
    
    return ofColor( (int)( r * 255 ), (int)( g * 255 ), (int)( b * 255 ), (int)( a * 255 ) );
}