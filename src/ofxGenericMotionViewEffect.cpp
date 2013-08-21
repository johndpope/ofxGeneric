//
//  ofxGenericMotionViewEffect.cpp
//  ofxGeneric
//
//  Created by Konstantin Yavichev on 8/21/13.
//  Copyright (c) 2013 Lumos Labs. All rights reserved.
//

#include "ofxGenericMotionViewEffect.h"

#include "ofxGenericView.h"

#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>
#endif

ofxGenericMotionViewEffect::ofxGenericMotionViewEffect()
: _minRelativeValue( 0.0f ), _maxRelativeValue( 0.0f )
{
}

ofPtr< ofxGenericMotionViewEffect > ofxGenericMotionViewEffect::create( ofxGenericMotionEffectType type, float minRelativeValue, float maxRelativeValue )
{
    ofPtr< ofxGenericMotionViewEffect > create = ofPtr< ofxGenericMotionViewEffect >( new ofxGenericMotionViewEffect() );
    create->init( create, type, minRelativeValue, maxRelativeValue );
    return create;
}

void ofxGenericMotionViewEffect::init( ofPtrWeak< ofxGenericMotionViewEffect > setThis, ofxGenericMotionEffectType type, float minRelativeValue, float maxRelativeValue )
{
    ofxGenericViewEffect::init( setThis );
    
    _type = type;
    _minRelativeValue = minRelativeValue;
    _maxRelativeValue = maxRelativeValue;
}

void ofxGenericMotionViewEffect::applyTo( ofPtr< ofxGenericView > view )
{
    if ( view )
    {
        
#if TARGET_OS_IPHONE
        
        UIView* nativeView = view->getNativeView();
        if ( nativeView && [ nativeView respondsToSelector:@selector(addMotionEffect:) ] ) // not available in iOS < 7.0
        {
            if( _type == ofxGenericMotionEffectTypeTiltAlongHorizontalAxis )
            {
                UIInterpolatingMotionEffect* interpolationHorizontal = [[UIInterpolatingMotionEffect alloc]initWithKeyPath:@"center.x" type:UIInterpolatingMotionEffectTypeTiltAlongHorizontalAxis];
                interpolationHorizontal.minimumRelativeValue = [ NSNumber numberWithFloat:_minRelativeValue ];
                interpolationHorizontal.maximumRelativeValue = [ NSNumber numberWithFloat:_maxRelativeValue ];
                
                [ nativeView addMotionEffect:interpolationHorizontal ];
            }
            else if ( _type == ofxGenericMotionEffectTypeTiltAlongVerticalAxis )
            {
                UIInterpolatingMotionEffect* interpolationVertical = [[UIInterpolatingMotionEffect alloc]initWithKeyPath:@"center.y" type:UIInterpolatingMotionEffectTypeTiltAlongVerticalAxis];
                interpolationVertical.minimumRelativeValue = [ NSNumber numberWithFloat:_minRelativeValue ];
                interpolationVertical.maximumRelativeValue = [ NSNumber numberWithFloat:_maxRelativeValue ];
                
                [ nativeView addMotionEffect:interpolationVertical ];
            }
        }
        
#endif
            
    }
}

