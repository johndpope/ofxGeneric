//
//  ofxGenericPageControl.cpp
//  ofxGeneric
//
//  Created by Eli Delventhal on 8/31/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericPageControl.h"
#include "ofxGenericUtility.h"

ofPtr< ofxGenericPageControl > ofxGenericPageControl::create( const ofRectangle& setFrame )
{
    ofPtr< ofxGenericPageControl > create = ofPtr< ofxGenericPageControl >( new ofxGenericPageControl() );
    create->init( create, setFrame );
    create->setAllowsDotInteraction( false );
    return create;
}

NativeView ofxGenericPageControl::createNativeView( const ofRectangle& frame )
{
#if TARGET_OS_IPHONE
    UIPageControl* newView = [ [ UIPageControl alloc ] initWithFrame:ofxRectangleToCGRect( frame ) ];
    return newView;
#endif
}

//the index of the currently selected dot
int ofxGenericPageControl::getCurrentPage()
{
#if TARGET_OS_IPHONE
    UIPageControl* nativeView = ( UIPageControl* )*this;
    if ( nativeView )
    {
        return (int) nativeView.currentPage;
    }
#endif
    return 0;
}

void ofxGenericPageControl::setCurrentPage( int page )
{
#if TARGET_OS_IPHONE
    UIPageControl* nativeView = ( UIPageControl* )*this;
    if ( nativeView )
    {
       nativeView.currentPage = page;
    }
#endif
}

//the number of dots
int ofxGenericPageControl::getPageCount()
{
#if TARGET_OS_IPHONE
    UIPageControl* nativeView = ( UIPageControl* )*this;
    if ( nativeView )
    {
        return (int) nativeView.numberOfPages;
    }
#endif
    return 0;
}

void ofxGenericPageControl::setPageCount( int pageCount )
{
#if TARGET_OS_IPHONE
    UIPageControl* nativeView = ( UIPageControl* )*this;
    if ( nativeView )
    {
        nativeView.numberOfPages = pageCount;
    }
#endif
}

//do not show if there is only one page
bool ofxGenericPageControl::hidesForSinglePage()
{
#if TARGET_OS_IPHONE
    UIPageControl* nativeView = ( UIPageControl* )*this;
    if ( nativeView )
    {
        return (int) nativeView.hidesForSinglePage;
    }
#endif
    return false;
}

void ofxGenericPageControl::setHidesForSinglePage( bool hides )
{
#if TARGET_OS_IPHONE
    UIPageControl* nativeView = ( UIPageControl* )*this;
    if ( nativeView )
    {
        nativeView.hidesForSinglePage = hides ? YES : NO;
    }
#endif
}

//tell this to refresh the image to match the selected page
void ofxGenericPageControl::refresh()
{
#if TARGET_OS_IPHONE
    UIPageControl* nativeView = ( UIPageControl* )*this;
    if ( nativeView )
    {
        [ nativeView updateCurrentPageDisplay ];
    }
#endif
}

//returns pixel size given a certain number of pages
ofPoint ofxGenericPageControl::getSizeForPageCount( int number )
{
#if TARGET_OS_IPHONE
    UIPageControl* nativeView = ( UIPageControl* )*this;
    if ( nativeView )
    {
        CGSize size = [ nativeView sizeForNumberOfPages:number ];
        return ofPoint( size.width, size.height );
    }
#endif
    return ofPoint( 0, 0 );
}

bool ofxGenericPageControl::allowsDotInteraction()
{
#if TARGET_OS_IPHONE
    UIPageControl* nativeView = ( UIPageControl* )*this;
    if ( nativeView )
    {
        return nativeView.defersCurrentPageDisplay ? false : true;
    }
#endif
    return false;
}

void ofxGenericPageControl::setAllowsDotInteraction( bool interacts )
{
#if TARGET_OS_IPHONE
    UIPageControl* nativeView = ( UIPageControl* )*this;
    if ( nativeView )
    {
        nativeView.defersCurrentPageDisplay = interacts ? NO : YES;
    }
#endif
}

void ofxGenericPageControl::setFilledDotColor( const ofColor& color )
{
#if TARGET_OS_IPHONE
    UIPageControl* nativeView = ( UIPageControl* )*this;
    if ( [ nativeView respondsToSelector:@selector( setCurrentPageIndicatorTintColor ) ] )
    {
        [ nativeView setCurrentPageIndicatorTintColor: ofxColorToUIColor( color ) ];
    } else
    {
        ofxGLogNotice( "Setting current page indicator color is not supported" );
    }

#endif
}

ofColor ofxGenericPageControl::getFilledDotColor()
{
#if TARGET_OS_IPHONE
    UIPageControl* nativeView = ( UIPageControl* )*this;
    if ( [ nativeView respondsToSelector:@selector( currentPageIndicatorTintColor ) ] )
    {
        return ofxUIColorToofColor( [ nativeView currentPageIndicatorTintColor ] );
    } else
    {
        ofxGLogNotice( "Setting current page indicator color is not supported" );
    }
#endif
    return ofColor( 255, 255, 255, 255 );
}

void ofxGenericPageControl::setEmptyDotColor( const ofColor& color )
{
#if TARGET_OS_IPHONE
    UIPageControl* nativeView = ( UIPageControl* )*this;
    if ( nativeView )
    {
        if ( [ nativeView respondsToSelector:@selector( setPageIndicatorTintColor: ) ] )
        {
            [ nativeView setPageIndicatorTintColor: ofxColorToUIColor( color ) ];
        } else
        {
            ofxGLogNotice( "Setting page indicator tint color is not supported" );
        }
    }
#endif
}

ofColor ofxGenericPageControl::getEmptyDotColor()
{
#if TARGET_OS_IPHONE
    UIPageControl* nativeView = ( UIPageControl* )*this;
    if ( [ nativeView respondsToSelector:@selector( pageIndicatorTintColor: ) ] )
    {
        return ofxUIColorToofColor( [ nativeView pageIndicatorTintColor ] );
    } else
    {
        ofxGLogNotice( "Setting page indicator tint color is not supported" );
    }
#endif
    return ofColor( 255, 255, 255, 255 );
}


#if TARGET_OS_IPHONE

ofxGenericUIViewCastOperator( ofxGenericPageControl, UIPageControl );

#endif
