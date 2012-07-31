//
//  ofxGenericFrameView.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 7/30/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericView.h"

class ofxGenericFrameView : public ofxGenericView
{
public:
    static ofPtr< ofxGenericFrameView > create( const ofRectangle& setFrame );

    virtual void willAppear();
    
    virtual void refresh();

    virtual void setThickness( float thickness );
    virtual void setColor( const ofColor& color );
    
    virtual void setFrame( const ofRectangle& setFrame );
    
protected:
    ofxGenericFrameView();
        
    float _thickness;
    ofColor _color;
    std::vector< ofPtr< ofxGenericView > > _frame;

    enum PositionIndex
    {
        Left = 0,
        Right,
        Top,
        Bottom
    };
    ofPtr< ofxGenericView > getPosition( PositionIndex position );

    ofPtr< ofxGenericView > getLeft();
    ofPtr< ofxGenericView > getRight();
    ofPtr< ofxGenericView > getTop();
    ofPtr< ofxGenericView > getBottom();
};