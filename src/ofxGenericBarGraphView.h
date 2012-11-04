//
//  ofxGenericBarGraphView.h
//  iOS
//
//  Created by Ian Grossberg on 4/20/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericImageView.h"

class ofxGenericTextView;

enum ofxGenericBarGraphViewExpand
{
    ofxGenericBarGraphViewExpandHorizontalLeft,
    ofxGenericBarGraphViewExpandHorizontalRight,
    ofxGenericBarGraphViewExpandVerticalUp,
    ofxGenericBarGraphViewExpandVerticalDown
};

class ofxGenericBarGraphView : public ofxGenericImageView
{
public:
    static ofPtr< ofxGenericBarGraphView > create( ofxGenericBarGraphViewExpand expand, float minimum = 0.0f, float maximum = 1.0f, const ofRectangle& setFrame = ofRectangle() );
    
    void setRange( float minimum, float maximum );
    void setCurrent( float value );
    float getCurrent();
    void setCurrentTextVisible( bool visible );
    void setCurrentTextPadding( float padding );
    void setBarFitCurrentText( bool fit, float padding = 0.0f );
    ofPtr< ofxGenericTextView > getCurrentTextView();
    
    void setImage( string fileName );
    void setBackgroundImage( string fileName );
    void setColor( const ofColor& color );

    void willLoad();
    void didLoad();

    virtual ~ofxGenericBarGraphView();
    
protected:
    ofxGenericBarGraphView();
    virtual void init( ofPtrWeak< ofxGenericBarGraphView > setThis, ofxGenericBarGraphViewExpand expand, float minimum, float maximum, const ofRectangle& setFrame );
    
    ofPtr< ofxGenericImageView > _barView;
    ofxGenericBarGraphViewExpand _expand;
    ofRectangle calculateBarFrame();
    ofRectangle adjustBarFrameForCurrentText( const ofRectangle& barFrame );

    ofPtr< ofxGenericTextView > _currentText;
    float _minimum, _maximum;
    float _current;
    void recalculateBar();
    float calculatePercent();

    bool _barFitCurrentText;
    float _barFitCurrentTextPadding;

    void updatedCurrentTextView();
};