//
//  ofxGenericBarGraphView.h
//  iOS
//
//  Created by Ian Grossberg on 4/20/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericImageView.h"

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
    
    void setImage( string fileName );
    void setBackgroundImage( string fileName );
    void setColor( const ofColor& color );

    void willLoad();
    void didLoad();

    virtual ~ofxGenericBarGraphView();
    
protected:
    ofxGenericBarGraphView();
    virtual void init( ofPtrWeak< ofxGenericBarGraphView > setThis, ofxGenericBarGraphViewExpand expand, float minimum, float maximum, const ofRectangle& setFrame );
    
    ofxGenericBarGraphViewExpand _expand;
    ofPtr< ofxGenericImageView > _barView;
    float _minimum, _maximum;
    float _current;
    void recalculateBar();
};