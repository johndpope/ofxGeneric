//
//  ofxGenericFrameView.cpp
//  ofxGeneric
//
//  Created by Ian Grossberg on 7/30/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericFrameView.h"


ofPtr< ofxGenericFrameView > ofxGenericFrameView::create( const ofRectangle& setFrame )
{
    ofPtr< ofxGenericFrameView > create( new ofxGenericFrameView() );
    create->init( create, setFrame );
    return create;
}

ofxGenericFrameView::ofxGenericFrameView()
: _thickness( 0.0f )
{
}

void ofxGenericFrameView::willAppear()
{
    ofxGenericView::willAppear();
    
    for( unsigned int edge = 1; edge <= 4; edge ++ )
    {
        ofPtr< ofxGenericView > add = ofxGenericView::create();
        add->setBackgroundColor( _color );
        _frame.push_back( add );
        addChildView( add );
    }
    refresh();
}

ofPtr< ofxGenericView > ofxGenericFrameView::getPosition( PositionIndex position )
{
    if ( _frame.size() > position )
    {
        return _frame[ position ];
    }
    return ofPtr< ofxGenericView >();
}

ofPtr< ofxGenericView > ofxGenericFrameView::getLeft()
{
    return getPosition( Left );
}

ofPtr< ofxGenericView > ofxGenericFrameView::getRight()
{
    return getPosition( Right );
}

ofPtr< ofxGenericView > ofxGenericFrameView::getTop()
{
    return getPosition( Top );
}

ofPtr< ofxGenericView > ofxGenericFrameView::getBottom()
{
    return getPosition( Bottom );
}

void ofxGenericFrameView::refresh()
{
    if ( _thickness > 0 )
    {
        ofRectangle frame = getFrame();
        ofRectangle resize( 0, 0, _thickness, frame.height );
        
        ofPtr< ofxGenericView > view = getLeft();
        if ( view )
        {
            view->setFrame( resize );
        }
        resize.width = frame.width;
        resize.height = _thickness;
        
        view = getTop();
        if ( view )
        {
            view->setFrame( resize );
        }
        
        resize.y = frame.height - _thickness;
        view = getBottom();
        if ( view )
        {
            view->setFrame( resize );
        }
        resize.x = frame.width - _thickness;
        resize.y = 0;
        resize.width = _thickness;
        resize.height = frame.height;
        view = getRight();
        if ( view )
        {
            view ->setFrame( resize );
        }
        for( std::vector< ofPtr< ofxGenericView > >::iterator show = _frame.begin(); show != _frame.end(); show ++ )
        {
            if ( *show )
            {
                ( *show )->setVisible( true );
            }
        }

    } else
    {
        for( std::vector< ofPtr< ofxGenericView > >::iterator hide = _frame.begin(); hide != _frame.end(); hide ++ )
        {
            if ( *hide )
            {
                ( *hide )->setVisible( false );
            }
        }
    }
}

void ofxGenericFrameView::setThickness( float thickness )
{
    _thickness = thickness;
    refresh();
}

void ofxGenericFrameView::setColor( const ofColor &color )
{
    _color = color;
    for( std::vector< ofPtr< ofxGenericView > >::iterator view = _frame.begin(); view != _frame.end(); view ++ )
    {
        if ( *view )
        {
            ( *view )->setBackgroundColor( _color );
        }
    }

}

void ofxGenericFrameView::setFrame( const ofRectangle& setFrame )
{
    ofxGenericView::setFrame( setFrame );
    refresh();
}