//
//  ofxGenericButtonView.h
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 12/30/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericView.h"

@class UIButtonDelegateForwarder;

class ofxGenericButtonView : public ofxGenericView
{
public:
    void setText( string newText );
    string getText();
    //    void setTextColor( const ofColor newColor );
    //    ofColor getTextColor();
    
    // void setImage
    void setBackgroundImage( string fileName );
    
    // TODO: scheme so that UIButtonDelegateForwarder is the only one that can call these
    virtual void touchCancel(){};
    virtual void touchDown(){};
    virtual void touchDownRepeat(){};
    virtual void touchDragEnter(){};
    virtual void touchDragExit(){};
    virtual void touchDragInside(){};
    virtual void touchDragOutside(){};
    virtual void touchUpInside(){};
    virtual void touchUpOutside(){};
    
protected:
    virtual UIView* createUIView( const CGRect& frame );  
    UIButtonDelegateForwarder* _eventHandler;
};

@interface UIButtonDelegateForwarder : NSObject 
{
@private
    ofxGenericButtonView* _delegate;
}
-( void )setDelegate:( ofxGenericButtonView* )setDelegate;

-( void )touchCancel:( id )sender;
-( void )touchDown:( id )sender;
-( void )touchDownRepeat:( id )sender;
-( void )touchDragEnter:( id )sender;
-( void )touchDragExit:( id )sender;
-( void )touchDragInside:( id )sender;
-( void )touchDragOutside:( id )sender;
-( void )touchUpInside:( id )sender;
-( void )touchUpOutside:( id )sender;

@end
