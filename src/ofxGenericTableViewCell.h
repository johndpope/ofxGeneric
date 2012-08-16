//
//  ofxGenericTableViewCell.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 8/15/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericView.h"

class ofxGenericTableView;

class ofxGenericTableViewCell : public ofxGenericView
{
public:
    virtual ~ofxGenericTableViewCell();
    static ofPtr< ofxGenericTableViewCell > create( ofPtrWeak< ofxGenericTableView > table, unsigned int section = 0, unsigned int index = 0, const ofRectangle& setBounds = ofRectangle( 0, 0, 0, 0 ) );
    
    virtual void setTablePosition( unsigned int section, unsigned int index );
    unsigned int getSection();
    unsigned int getIndex();
    
    virtual void setBackgroundColor( const ofColor& color );
    virtual void setText( string text );
    virtual void setImage( string imagePath );
    virtual void addChildView( ofPtr< ofxGenericView > add );
    virtual void addChildViewBefore( ofPtr< ofxGenericView > add, ofPtr< ofxGenericView > before );
    virtual void addChildViewAfter( ofPtr< ofxGenericView > add, ofPtr< ofxGenericView > after );
    virtual void removeChildView( ofPtr< ofxGenericView > remove );
    virtual ofPtr< ofxGenericView > getContentView();
    
    virtual void selected();
    
    virtual bool canBeDragged();
    
#if TARGET_OS_IPHONE
    operator UITableViewCell*();
#endif
    
protected:
    virtual void init( ofPtrWeak< ofxGenericTableViewCell > setThis, ofPtrWeak< ofxGenericTableView > table, unsigned int section, unsigned int index, const ofRectangle& setBounds = ofRectangle( 0, 0, 0, 0 )  );
    ofPtrWeak< ofxGenericTableView > _table;
    ofPtr< ofxGenericView > _contentView;
    
    virtual NativeView createNativeView( const ofRectangle& frame );
    
    unsigned int _section;
    unsigned int _index;
    
    ofxGenericTableViewCell();
};
