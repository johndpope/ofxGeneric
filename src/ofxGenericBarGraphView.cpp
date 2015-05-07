//
//  ofxGenericBarGraphView.cpp
//  iOS
//
//  Created by Ian Grossberg on 4/20/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericBarGraphView.h"

#include "ofxGenericTextView.h"

#include "ofxGenericMain.h"

#include "ofUtils.h"

ofPtr< ofxGenericBarGraphView > ofxGenericBarGraphView::create( ofxGenericBarGraphViewExpand expand, float minimum, float maximum, const ofRectangle& setFrame )
{
    ofPtr< ofxGenericBarGraphView > create( new ofxGenericBarGraphView() );
    create->init( create, expand, minimum, maximum, setFrame );
    return create;
}

ofxGenericBarGraphView::ofxGenericBarGraphView()
: _minimum( 0.0f ), _maximum( 0.0f ), _current( 0.0f ), _barFitCurrentText( false ), _barFitCurrentTextPadding( 0.0f ), _replaceZeroCurrentText( false )
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
    updateCurrentText();
    recalculateBar();
}

float ofxGenericBarGraphView::getCurrent()
{
    return _current;
}

void ofxGenericBarGraphView::setCurrentTextVisible( bool visible )
{
    if ( visible )
    {
        if ( !_currentText )
        {
            _currentText = ofxGenericTextView::create( getFrame( ofPoint( 0, 0 ) ) );
            if ( _currentText )
            {
                //_currentText->setAutosizeFontToFitText( ofxGenericViewAutoresizingFull );
                switch( _expand )
                {
                    case ofxGenericBarGraphViewExpandHorizontalRight:
                        _currentText->setTextAlignment( ofxGenericTextHorizontalAlignmentLeft );
                        break;
                    default:
                        _currentText->setTextAlignment( ofxGenericTextHorizontalAlignmentRight );
                        break;

                }
                addChildView( _currentText );
                
                _currentText->setText( ofToString( getCurrent() ) );
            }
        }
        updateCurrentText();
    } else
    {
        if ( _currentText )
        {
            _currentText->removeFromParent();
            _currentText = ofPtr< ofxGenericTextView >();
        }
    }
}



void ofxGenericBarGraphView::setCurrentTextPadding( float padding )
{
    if ( _currentText )
    {
        ofRectangle frame = _currentText->getFrame();
        frame.x += padding;
        frame.width -= padding;
        _currentText->setFrame( frame );
    }
}

void ofxGenericBarGraphView::setBarFitCurrentText( bool fit, float padding )
{
    _barFitCurrentText = fit;
    _barFitCurrentTextPadding = padding;
    recalculateBar();
}

void ofxGenericBarGraphView::updateCurrentText()
{
    if ( _currentText )
    {
        string text;
        if ( _replaceZeroCurrentText && getCurrent() == 0 )
        {
            text = _replaceZeroCurrentTextWith;
        } else
        {
            text = ofToString( getCurrent() );
        }
        _currentText->setText( text );
    }
}

void ofxGenericBarGraphView::setCurrentTextReplaceZero( bool replace, string replaceWith )
{
    _replaceZeroCurrentText = replace;
    _replaceZeroCurrentTextWith = replaceWith;
    if ( getCurrent() == 0 )
    {
        setCurrent( 0 );
    }
}

ofPtr< ofxGenericTextView > ofxGenericBarGraphView::getCurrentTextView()
{
    return _currentText;
}

ofRectangle ofxGenericBarGraphView::calculateBarFrame()
{
    float percent = calculatePercent();
    
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
    return frame;
}

ofRectangle ofxGenericBarGraphView::adjustBarFrameForCurrentText( const ofRectangle& barFrame )
{
    ofRectangle adjustedBarFrame = barFrame;
    if ( _currentText )
    {
        ofRectangle frame = getFrame();
        ofRectangle currentTextFrame = _currentText->getFrame();

        ofPoint textSize = ofxGPointSizeForText( _currentText->getText(), _currentText->getFontName(), _currentText->getFontSize(), frame.width );
        
        float minimumBarWidth = currentTextFrame.x + textSize.x + _barFitCurrentTextPadding;
        adjustedBarFrame.width = MAX( adjustedBarFrame.width, minimumBarWidth );
        adjustedBarFrame.width = MIN( adjustedBarFrame.width, frame.width );
        
        // HACK: adjust text vertical blah blah
        currentTextFrame.height = textSize.y;
        currentTextFrame.y = frame.height / 2 - currentTextFrame.height / 2;
        _currentText->setFrame( currentTextFrame );
    }

    return adjustedBarFrame;
}

void ofxGenericBarGraphView::recalculateBar()
{
    ofRectangle barFrame = calculateBarFrame();
    
    if ( _barFitCurrentText )
    {
        barFrame = adjustBarFrameForCurrentText( barFrame );
    }
    
    if ( barFrame.width == 0 || barFrame.height == 0 )
    {
        _barView->setVisible( false );
    } else 
    {
        _barView->setVisible( true );
        _barView->setFrame( barFrame );
    }
}

float ofxGenericBarGraphView::calculatePercent()
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
    return percent;
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

