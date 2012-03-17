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
