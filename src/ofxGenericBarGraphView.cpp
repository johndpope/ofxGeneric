//
//  ofxGenericBarGraphView.cpp
//  iOS
//
//  Created by Ian Grossberg on 4/20/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericBarGraphView.h"

ofPtr< ofxGenericBarGraphView > ofxGenericBarGraphView::create( ofxGenericBarGraphViewExpand expand, float minimum, float maximum, const ofRectangle& setFrame )
{
    ofPtr< ofxGenericBarGraphView > create( new ofxGenericBarGraphView() );
    create->init( create, expand, minimum, maximum, setFrame );
    return create;
}

ofxGenericBarGraphView::ofxGenericBarGraphView()
: _minimum( 0.0f ), _maximum( 0.0f ), _current( 0.0f )
{
}

void ofxGenericBarGraphView::init( ofPtrWeak< ofxGenericBarGraphView > setThis, ofxGenericBarGraphViewExpand expand, float minimum, float maximum, const ofRectangle& setFrame )
{
    ofxGenericImageView::init( setThis, setFrame );
    _expand = expand;
    _minimum = minimum;
    _maximum = maximum;
}

void ofxGenericBarGraphView::setRange( float minimum, float maximum )
{
    _minimum = minimum;
    _maximum = maximum;
    recalculateBar();
}

void ofxGenericBarGraphView::setCurrent( float value )
{
    _current = value;
    recalculateBar();
}

void ofxGenericBarGraphView::recalculateBar()
{
    float percent = 0;
    if ( _maximum != _minimum )
    {
        percent = ( _current - _minimum ) / ( _maximum - _minimum );
    }
    if ( percent < _minimum )
    {
        percent = _minimum;
    }
    if ( percent > _maximum )
    {
        percent = _maximum;
    }
        
    ofRectangle frame = getFrame( ofPoint( 0, 0 ) );
    float newWidth, newHeight;
    switch( _expand )
    {
        case ofxGenericBarGraphViewExpandHorizontalLeft:
            newWidth = frame.width * percent;
            frame.x += frame.width - newWidth;
            frame.width = newWidth;
            break;
            
        case ofxGenericBarGraphViewExpandHorizontalRight:
            newWidth = frame.width * percent;
            frame.width = newWidth;
            break;
            
        case ofxGenericBarGraphViewExpandVerticalUp:
            newHeight = frame.height * percent;
            frame.y -= frame.height - newHeight;
            frame.height = newHeight;
            break;
            
        case ofxGenericBarGraphViewExpandVerticalDown:
            newHeight = frame.height * percent;
            frame.height = newHeight;
            break;
    }
    if ( frame.width == 0 || frame.height == 0 )
    {
        _barView->setVisible( false );
    } else 
    {
        _barView->setVisible( true );
        _barView->setFrame( frame );
    }
}

void ofxGenericBarGraphView::setImage( string fileName )
{
    _barView->setImage( fileName );
}

void ofxGenericBarGraphView::setBackgroundImage( string fileName )
{
    ofxGenericImageView::setImage( fileName );
}

void ofxGenericBarGraphView::setColor( const ofColor& color )
{
    _barView->setBackgroundColor( color );
}

void ofxGenericBarGraphView::willLoad()
{
}

void ofxGenericBarGraphView::didLoad()
{
    _barView = ofxGenericImageView::create( getFrame() );
    addChildView( _barView );
    recalculateBar();
}

ofxGenericBarGraphView::~ofxGenericBarGraphView()
{
    
}

