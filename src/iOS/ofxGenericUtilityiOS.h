//
//  ofxGenericUtilityiOS.h
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 12/29/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"

#import "ofxiPhoneExtras.h"

string ofxGToString( NSData* data );
string ofxGToString( NSString* string );

CGFloat getWindowScale();

NSTextAlignment ofxGenericTextHorizontalAlignmentToiOS( ofxGenericTextHorizontalAlignment from );
ofxGenericTextHorizontalAlignment iOSToofxGenericTextHorizontalAlignment( NSTextAlignment from );

UITableViewCellSeparatorStyle ofxGenericTableViewSeparatorStyleToiOS( ofxGenericTableViewSeparatorStyle from );

UIReturnKeyType     ofxGenericKeyboardReturnKeyToiOS( ofxGenericKeyboardReturnKey from );
ofxGenericKeyboardReturnKey iOSToofxGenericKeyboardReturnKey( UIReturnKeyType from );

UIKeyboardType ofxGenericKeyboardTypeToiOS( ofxGenericKeyboardType from );
ofxGenericKeyboardType iOSToofxGenericKeyboardType( UIKeyboardType from );

UIActivityIndicatorViewStyle ofxGenericActivityViewStyleToiOS( ofxGenericActivityViewStyle style);

ofxGenericActivityViewStyle iOSToofxGenericActivityViewStyle( UIActivityIndicatorViewStyle style );

UITextAutocapitalizationType ofxGenericTextAutoCapitalizationToiOS( ofxGenericTextAutoCapitalization from );
ofxGenericTextAutoCapitalization iOSToofxGenericTextAutoCapitalization( UITextAutocapitalizationType from );

UIDeviceOrientation ofOrientationToiOS( ofOrientation from );
ofOrientation iOSDeviceToofOrientation( UIDeviceOrientation from );

UIInterfaceOrientation ofInterfaceOrientationToiOS( ofOrientation from );
ofOrientation iOSToofOrientation( UIInterfaceOrientation from );

NSLineBreakMode ofxGenericTextLinebreakModeToiOS( ofxGenericTextLinebreakMode from );
ofxGenericTextLinebreakMode iOSToofxGenericTextLinebreakMode( NSLineBreakMode from );

UIViewAnimationCurve ofxGenericViewAnimationCurveToiOS( ofxGenericViewAnimationCurve from );
UIViewAnimationTransition ofxGenericViewAnimationTransitionToiOS( ofxGenericViewAnimationTransition from );

UIViewAutoresizing ofxGenericViewAutoresizingToiOS( ofxGenericViewAutoresizing from );
ofxGenericViewAutoresizing iOSToofxGenericViewAutoresizing( UIViewAutoresizing from );

UITextBorderStyle ofxGenericTextViewBorderStyleToiOS( ofxGenericTextViewBorderStyle from );
ofxGenericTextViewBorderStyle iOSToofxGenericTextViewBorderStyle( UITextBorderStyle from );

UISwipeGestureRecognizerDirection ofxGenericGestureTypeSwipeToiOS( ofxGenericGestureTypeSwipe from );
ofxGenericGestureTypeSwipe iOSToofxGenericGestureTypeSwipe( UISwipeGestureRecognizerDirection from );

UIButtonType ofxGenericButtonTypeToiOS( ofxGenericButtonType from );
ofxGenericButtonType iOSToofxGenericButtonType( UIButtonType from );

UIDatePickerMode ofxGenericDateSelectorModeToiOS( ofxGenericDateSelectorMode from );
ofxGenericDateSelectorMode iOSToofxGenericDateSelectorMode( UIDatePickerMode from );

UIGestureRecognizerState ofxGenericGestureStateToiOS( ofxGenericGestureState from );
ofxGenericGestureState iOSToofxGenericGestureState( UIGestureRecognizerState from );

ofPtr< ofImage > UIImageToofImage( UIImage* image );
UIImage* OFImageToUIImage( ofImage& image ); // fix to use ofPtr?

CGSize ofPointToCGSize( const ofPoint& size );
ofPoint CGSizeToofPoint( CGSize size );

UIViewContentMode ofxGenericContentModeToiOS( ofxGenericContentMode from );
ofxGenericContentMode iOSToofxGenericContentMode( UIViewContentMode from );

UIControlContentHorizontalAlignment ofxGenericContentHorizontalAlignmentToiOS( ofxGenericContentHorizontalAlignment from );
ofxGenericContentHorizontalAlignment iOSToofxGenericContentHorizontalAlignment( UIControlContentHorizontalAlignment from );
