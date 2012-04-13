//
//  ofxGenericConstants.h
//  iOS
//
//  Created by Ian Grossberg on 1/19/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofMain.h"

#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>
typedef UIView* NativeView;
typedef UIWindow* NativeWindow;
#else //if TARGET_ANDROID // FIXME: ARG ECLIPSE WHY DO YOU HATE ME PARSEEEEEEEE DAAAMN YOU
#define TARGET_ANDROID 1
// WTFFFFFFF
#include <jni.h>
typedef jobject NativeView;
typedef jobject NativeWindow;

#endif

#define ofxGenericModuleName "OF::Generic"

enum ofxGenericTextLinebreakMode
{
    ofxGenericTextLinebreakModeWordWrap,
    ofxGenericTextLinebreakModeCharacterWrap,
    ofxGenericTextLinebreakModeClip,
    ofxGenericTextLinebreakModeHeadTruncation,
    ofxGenericTextLinebreakModeTailTruncation,
    ofxGenericTextLinebreakModeMiddleTruncation
};

enum ofxGenericTextHorizontalAlignment
{
    ofxGenericTextHorizontalAlignmentLeft,
    ofxGenericTextHorizontalAlignmentCenter,
    ofxGenericTextHorizontalAlignmentRight
};

enum ofxGenericTableViewSeparatorStyle
{
    ofxGenericTableViewSeparatorStyleNone,
    ofxGenericTableViewSeparatorStyleSingleLine,
    ofxGenericTableViewSeparatorStyleSingleLineEtched
};

enum ofxGenericTextAutoCapitalization
{
    ofxGenericTextAutoCapitalizationNone, // UITextAutocapitalizationTypeNone,
    ofxGenericTextAutoCapitalizationWords, // UITextAutocapitalizationTypeWords,
    ofxGenericTextAutoCapitalizationSentences, // UITextAutocapitalizationTypeSentences,
    ofxGenericTextAutoCapitalizationAllCharacters // UITextAutocapitalizationTypeAllCharacters,
};

enum ofxGenericKeyboardReturnKey
{
    ofxGenericKeyboardReturnKeyDefault, //UIReturnKeyDefault,
    ofxGenericKeyboardReturnKeyGo, //UIReturnKeyGo,
//    ofxGenericKeyboardReturnKeyGoogle, //UIReturnKeyGoogle,
//    ofxGenericKeyboardReturnKeyJoin, UIReturnKeyJoin,
    ofxGenericKeyboardReturnKeyNext, //UIReturnKeyNext,
//    ofxGenericKeyboardReturnKeyRoute, //UIReturnKeyRoute,
    ofxGenericKeyboardReturnKeySearch, //UIReturnKeySearch,
    ofxGenericKeyboardReturnKeySend, //UIReturnKeySend,
//    ofxGenericKeyboardReturnKeyYahoo, UIReturnKeyYahoo,
    ofxGenericKeyboardReturnKeyDone, //UIReturnKeyDone,
    //ofxGenericKeyboardReturnKeyEmergencyCall //UIReturnKeyEmergencyCall,
};

enum ofxGenericKeyboardType
{
    ofxGenericKeyboardTypeDefault, //UIKeyboardTypeDefault,                // Default type for the current input method.
    ofxGenericKeyboardTypeASCIICapable, //UIKeyboardTypeASCIICapable,           // Displays a keyboard which can enter ASCII characters, non-ASCII keyboards remain active
    ofxGenericKeyboardTypeNumbersAndPunctuation, //UIKeyboardTypeNumbersAndPunctuation,  // Numbers and assorted punctuation.
     ofxGenericKeyboardTypeURL, //UIKeyboardTypeURL,                    // A type optimized for URL entry (shows . / .com prominently).
     ofxGenericKeyboardTypeNumberPad, //UIKeyboardTypeNumberPad,              // A number pad (0-9). Suitable for PIN entry.
     ofxGenericKeyboardTypePhonePad, //UIKeyboardTypePhonePad,               // A phone pad (1-9, *, 0, #, with letters under the numbers).
     ofxGenericKeyboardTypeNamePhonePad, //UIKeyboardTypeNamePhonePad,           // A type optimized for entering a person's name or phone number.
     ofxGenericKeyboardTypeEmailAddress, //UIKeyboardTypeEmailAddress,           // A type optimized for multiple email address entry (shows space @ . prominently).
#if __IPHONE_4_1 <= __IPHONE_OS_VERSION_MAX_ALLOWED
     ofxGenericKeyboardTypeDecimalPad, //UIKeyboardTypeDecimalPad,             // A number pad with a decimal point.
#endif
#if __IPHONE_5_0 <= __IPHONE_OS_VERSION_MAX_ALLOWED
     ofxGenericKeyboardTypeTwitter, //UIKeyboardTypeTwitter,                // A type optimized for twitter text entry (easy access to @ #)
#endif
};

enum ofxGenericViewAnimationCurve
{
    ofxViewAnimationCurveEaseInOut, //UIViewAnimationCurveEaseInOut,         // slow at beginning and end
    ofxViewAnimationCurveEaseIn, //UIViewAnimationCurveEaseIn,            // slow at beginning
    ofxViewAnimationCurveEaseOut, //UIViewAnimationCurveEaseOut,           // slow at end
    ofxViewAnimationCurveLinear //UIViewAnimationCurveLinear
};

enum ofxGenericViewAnimationTransition
{
    ofxGenericViewAnimationTransitionNone, //UIViewAnimationTransitionNone,
    ofxGenericViewAnimationTransitionFlipFromLeft, //UIViewAnimationTransitionFlipFromLeft,
    ofxGenericViewAnimationTransitionFlipFromRight, //UIViewAnimationTransitionFlipFromRight,
    ofxGenericViewAnimationTransitionCurlUp, //UIViewAnimationTransitionCurlUp,
    ofxGenericViewAnimationTransitionCurlDown //UIViewAnimationTransitionCurlDown,
};