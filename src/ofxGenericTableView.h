//
//  ofxGenericTableView.h
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 1/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericView.h"
#include "ofxGenericConstants.h"

#if TARGET_OS_IPHONE
@class ofxGenericTableViewForwarder;
#endif
class ofxGenericTableViewCell;
class ofxGenericTableViewDelegate;

class ofxGenericTableView : public ofxGenericView
{
public:
    static ofPtr< ofxGenericTableView > create( const ofRectangle& setFrame = ofRectangle( 0, 0, 0, 0 ) );
    virtual ~ofxGenericTableView();
    
    virtual unsigned int getNumberOfCells( unsigned int section );
    virtual ofPtr< ofxGenericTableViewCell > getCell( unsigned int section, unsigned int index );
    virtual float getHeightForCell( unsigned int section, unsigned int index );
    
    void setAutoresizeToFit( bool autoResizeToFit );
    
    virtual void setSeparatorColor( const ofColor& separatorColor );
    virtual void setSeparatorStyle( ofxGenericTableViewSeparatorStyle separatorStyle );

    virtual void setDelegate( ofPtrWeak< ofxGenericTableViewDelegate > delegate );
    virtual void reloadData();
    
    virtual void selectedRow( unsigned int section, unsigned int index);
    virtual void setAllowsSelection( bool allow );
    virtual bool allowsSelection();
    virtual void setCellIsSelected( unsigned int section, unsigned int index, bool selected);
    virtual bool deselectAllCells();
    virtual int getSelectedIndex();
    virtual int getSelectedSection();
    
#if TARGET_OS_IPHONE
    operator UITableView*();
#endif

protected:
    virtual NativeView createNativeView( const ofRectangle& frame );
#if TARGET_OS_IPHONE
    ofxGenericTableViewForwarder* _forwarder;    
#endif
    ofPtrWeak< ofxGenericTableViewDelegate > _delegate;
    
    virtual void resizeToFitContents();
    bool _autoResizeToFit;
    float _maximumHeight;
};

class ofxGenericTableViewCell : public ofxGenericView
{
public:
    virtual ~ofxGenericTableViewCell();
    static ofPtr< ofxGenericTableViewCell > create( ofPtrWeak< ofxGenericTableView > table, const ofRectangle& setBounds = ofRectangle( 0, 0, 0, 0 )  );
    virtual void init( ofPtr< ofxGenericTableViewCell > setThis, ofPtrWeak< ofxGenericTableView > table, const ofRectangle& setBounds = ofRectangle( 0, 0, 0, 0 )  );
    virtual void setText( string text );
    virtual void setImage( string imagePath );
    
#if TARGET_OS_IPHONE
    operator UITableViewCell*();
#endif

protected:
    ofPtrWeak< ofxGenericTableView > _table;
    
#if TARGET_OS_IPHONE
    virtual UITableViewCell* createUITableViewCell( const CGRect& frame );
#endif
};

class ofxGenericTableViewDelegate
{
public:
    virtual ~ofxGenericTableViewDelegate() {};
    virtual unsigned int getNumberOfCells( ofPtr< ofxGenericTableView > tableView, unsigned int section ) = 0;
    virtual ofPtr< ofxGenericTableViewCell > getCell( ofPtr< ofxGenericTableView > tableView, unsigned int section, unsigned int index ) = 0;
    virtual float getHeightForCell( ofPtr< ofxGenericTableView > tableView, unsigned int section, unsigned int index ) = 0;
    virtual void selectedRow( ofPtr< ofxGenericTableView > tableView, unsigned int section, unsigned int index) {};
};