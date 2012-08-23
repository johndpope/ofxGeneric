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

// memory macros
#define release( objectiveCObject ) \
[ objectiveCObject release ]; \
objectiveCObject = nil;

#define releaseView( objectiveCView ) \
[ objectiveCView removeFromSuperview ]; \
release( objectiveCView );

#define releaseViewController( objectCViewController ) \
[ objectCViewController release ]; \
objectCViewController = nil;

string ofxGToString( NSData* data );

CGFloat getWindowScale();

UITextAlignment ofxGenericTextHorizontalAlignmentToiOS( ofxGenericTextHorizontalAlignment from );
ofxGenericTextHorizontalAlignment iOSToofxGenericTextHorizontalAlignment( UITextAlignment from );

UITableViewCellSeparatorStyle ofxGenericTableViewSeparatorStyleToiOS( ofxGenericTableViewSeparatorStyle from );

UIReturnKeyType     ofxGenericKeyboardReturnKeyToiOS( ofxGenericKeyboardReturnKey from );
ofxGenericKeyboardReturnKey iOSToofxGenericKeyboardReturnKey( UIReturnKeyType from );

UIKeyboardType ofxGenericKeyboardTypeToiOS( ofxGenericKeyboardType from );
ofxGenericKeyboardType iOSToofxGenericKeyboardType( UIKeyboardType from );

UITextAutocapitalizationType ofxGenericTextAutoCapitalizationToiOS( ofxGenericTextAutoCapitalization from );
ofxGenericTextAutoCapitalization iOSToofxGenericTextAutoCapitalization( UITextAutocapitalizationType from );

UIDeviceOrientation ofOrientationToiOS( ofOrientation from );
ofOrientation iOSDeviceToofOrientation( UIDeviceOrientation from );

UIInterfaceOrientation ofInterfaceOrientationToiOS( ofOrientation from );
ofOrientation iOSToofOrientation( UIInterfaceOrientation from );

UILineBreakMode ofxGenericTextLinebreakModeToiOS( ofxGenericTextLinebreakMode from );
ofxGenericTextLinebreakMode iOSToofxGenericTextLinebreakMode( UILineBreakMode from );

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

UIImage* OFImageToUIImage( ofImage& image );

CGSize ofPointToCGSize( const ofPoint& size );
ofPoint CGSizeToofPoint( CGSize size );