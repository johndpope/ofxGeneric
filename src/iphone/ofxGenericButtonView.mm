//
//  ofxGenericButtonView.mm
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 12/30/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#import "ofxGenericButtonView.h"
#import "ofxGenericUtility.h"

ofxGenericButtonView::ofxGenericButtonView()
{
}

ofxGenericButtonView::~ofxGenericButtonView()
{
    release( _eventHandler );
}

void ofxGenericButtonView::init( ofPtrWeak< ofxGenericView > setThis, const ofRectangle& setBounds, ofPtrWeak< ofxGenericButtonViewTouchDelegate > touchDelegate )
{
    ofxGenericView::init( setThis, setBounds );
    _touchDelegate = touchDelegate;
}

UIView* ofxGenericButtonView::createUIView( const CGRect& frame )
{
    // TODO: configurable type
    UIButton* newView = [ [ UIButton buttonWithType:UIButtonTypeRoundedRect ] retain ];
    [ newView setFrame:frame ];
    [ newView setTitleColor:[ UIColor blackColor ] forState:UIControlStateNormal ];
    [ newView setBackgroundColor:[ UIColor clearColor ] ];
    
    // TODO: doesn't match createUIView design
    _eventHandler = [ [ UIButtonDelegateForwarder alloc ] init ];
    [ _eventHandler setDelegate:this ];
    
    [ newView addTarget:_eventHandler action:@selector( touchCancel: ) forControlEvents: UIControlEventTouchCancel ];
    [ newView addTarget:_eventHandler action:@selector( touchDown: ) forControlEvents: UIControlEventTouchDown ];
    [ newView addTarget:_eventHandler action:@selector( touchDownRepeat: ) forControlEvents: UIControlEventTouchDownRepeat ];
    [ newView addTarget:_eventHandler action:@selector( touchDragEnter: ) forControlEvents: UIControlEventTouchDragEnter ];
    [ newView addTarget:_eventHandler action:@selector( touchDragExit: ) forControlEvents: UIControlEventTouchDragExit ];
    [ newView addTarget:_eventHandler action:@selector( touchDragInside: ) forControlEvents: UIControlEventTouchDragInside ];
    [ newView addTarget:_eventHandler action:@selector( touchDragOutside: ) forControlEvents: UIControlEventTouchDragOutside ];
    [ newView addTarget:_eventHandler action:@selector( touchUpInside: ) forControlEvents: UIControlEventTouchUpInside ];
    [ newView addTarget:_eventHandler action:@selector( touchUpOutside: ) forControlEvents:UIControlEventTouchUpOutside ];
    return newView;
}

void ofxGenericButtonView::setText( string newText )
{
    if ( [ _view isKindOfClass:[ UIButton class ] ] )
    {
        UIButton* view = ( UIButton* )_view;
        [ view setTitle:ofxStringToNSString( newText ) forState:UIControlStateNormal ];
    }    
}

string ofxGenericButtonView::getText()
{
    if ( [ _view isKindOfClass:[ UIButton class ] ] )
    {
        UIButton* view = ( UIButton* )_view;
        return ofxNSStringToString( [ view currentTitle ] );
    }
    return string();
}

void ofxGenericButtonView::setBackgroundImage( string fileName )
{
    if ( [ _view isKindOfClass:[ UIButton class ] ] )
    {
        UIButton* view = ( UIButton* )_view;
        [ view setBackgroundImage:[ UIImage imageWithContentsOfFile:pathToBundle( ofxStringToNSString( fileName ) )  ] 
                         forState:UIControlStateNormal ];
    }
}

ofxGenericUIViewCastOperator( ofxGenericButtonView, UIButton );

#define touchEventMethod( eventName ) \
void ofxGenericButtonView::eventName() \
{ \
    if ( _touchDelegate ) \
    { \
        ( _touchDelegate.lock() )->button_ ## eventName(); \
    } \
}

touchEventMethod( touchCancel );
touchEventMethod( touchDown );
touchEventMethod( touchDownRepeat );
touchEventMethod( touchDragEnter );
touchEventMethod( touchDragExit );
touchEventMethod( touchDragInside );
touchEventMethod( touchDragOutside );
touchEventMethod( touchUpInside );
touchEventMethod( touchUpOutside );

@implementation UIButtonDelegateForwarder

-( void )setDelegate:(ofxGenericButtonView *)setDelegate
{
    _delegate = setDelegate;
}

-( void )touchCancel:( id )sender
{    
    _delegate->touchCancel();
}

-( void )touchDown:( id )sender
{    
    _delegate->touchDown();
}

-( void )touchDownRepeat:( id )sender
{    
    _delegate->touchDownRepeat();
}

-( void )touchDragEnter:( id )sender
{    
    _delegate->touchDragEnter();
}

-( void )touchDragExit:( id )sender
{    
    _delegate->touchDragExit();
}

-( void )touchDragInside:( id )sender
{    
    _delegate->touchDragInside();
}

-( void )touchDragOutside:( id )sender
{    
    _delegate->touchDragOutside();
}

-( void )touchUpInside:( id )sender
{    
    _delegate->touchUpInside();
}

-( void )touchUpOutside:( id )sender
{    
    _delegate->touchUpOutside();
}

@end