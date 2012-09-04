//
//  ofxGenericPageControl.h
//  ofxGeneric
//
//  Created by Eli Delventhal on 8/31/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericView.h"

class ofxGenericPageControl : public ofxGenericView
{
public:
    static ofPtr< ofxGenericPageControl > create( const ofRectangle& setFrame = ofRectangle( 0, 0, 0, 0 ) );
#if TARGET_OS_IPHONE
    operator UIPageControl*();
#endif
    
    //the index of the currently selected dot
    int getCurrentPage();
    void setCurrentPage( int page );
    
    //the number of dots
    int getPageCount();
    void setPageCount( int pageCount );
    
    //do not show if there is only one page
    bool hidesForSinglePage();
    void setHidesForSinglePage( bool hides );
    
    //tell this to refresh the image to match the selected page
    void refresh();
    
    //returns pixel size given a certain number of pages
    ofPoint getSizeForPageCount( int number );
    
    //if true, the image will not update when the user taps a dot, it must be told to do so manually
    //implement if needed
    //bool defersCurrentPageDisplay();
    
protected:
    virtual NativeView createNativeView( const ofRectangle& frame );
};