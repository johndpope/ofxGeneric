//
//  ofxGenericTableView.h
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 1/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericView.h"
#include "ofxGenericScrollView.h"
#include "ofxGenericConstants.h"

#if TARGET_OS_IPHONE
@class ofxGenericTableViewForwarder;
#endif
class ofxGenericTableViewCell;
class ofxGenericTableViewDelegate;

class ofxGenericTableView : public ofxGenericScrollView
{
public:
    static ofPtr< ofxGenericTableView > create( const ofRectangle& setFrame = ofRectangle( 0, 0, 0, 0 ) );
    virtual ~ofxGenericTableView();
    
    virtual unsigned int getNumberOfCells( unsigned int section );
    virtual ofPtr< ofxGenericTableViewCell > getCell( unsigned int section, unsigned int index );
    virtual float getHeightForCell( unsigned int section, unsigned int index );
    
    virtual unsigned int getNumberOfSections();
    
    virtual float getHeightForHeaderInSection( unsigned int section );
    virtual ofPtr< ofxGenericView > getHeaderForSection( unsigned int section );
    
    void setAutoresizeToFit( bool autoResizeToFit );
    
    virtual void setSeparatorColor( const ofColor& separatorColor );
    virtual void setSeparatorStyle( ofxGenericTableViewSeparatorStyle separatorStyle );
    ofxGenericTableViewSeparatorStyle getSeparatorStyle();
    virtual void setSeparatorPaddedHeight( float height );

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

    virtual unsigned int internalGetNumberOfCells( unsigned int section );
    virtual ofPtr< ofxGenericTableViewCell > internalGetCell( unsigned int section, unsigned int index );
    virtual float internalGetHeightForCell( unsigned int section, unsigned int index );
    virtual void internalSelectedRow( unsigned int section, unsigned int index );

protected:
    virtual NativeView createNativeView( const ofRectangle& frame );
#if TARGET_OS_IPHONE
    ofxGenericTableViewForwarder* _forwarder;    
#endif
    ofPtrWeak< ofxGenericTableViewDelegate > _delegate;

    ofxGenericTableViewSeparatorStyle _separatorStyle;
    float _paddedSeparatorHeight;
    ofPtr< ofxGenericTableViewCell > _paddedSeparator;
    
    float getContentHeight();
    virtual void resizeToFitContents();
    bool _autoResizeToFit;
    float _maximumHeight;
    
    ofxGenericTableView();
};

class ofxGenericTableViewCell : public ofxGenericView
{
public:
    virtual ~ofxGenericTableViewCell();
    static ofPtr< ofxGenericTableViewCell > create( ofPtrWeak< ofxGenericTableView > table, unsigned int section = 0, unsigned int index = 0, const ofRectangle& setBounds = ofRectangle( 0, 0, 0, 0 ) );
    
    virtual void setTablePosition( unsigned int section, unsigned int index );
    unsigned int getSection();
    unsigned int getIndex();
        
    virtual void setText( string text );
    virtual void setImage( string imagePath );
    virtual void addChildView( ofPtr< ofxGenericView > add );
    virtual void addChildView( ofPtr< ofxGenericView > add, ofPtr< ofxGenericView > before );
    virtual void removeChildView( ofPtr< ofxGenericView > remove );
    virtual ofPtr< ofxGenericView > getContentView();
    
    virtual void selected();
    
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

class ofxGenericTableViewDelegate
{
public:
    virtual ~ofxGenericTableViewDelegate() {};
    virtual unsigned int getNumberOfCells( ofPtr< ofxGenericTableView > tableView, unsigned int section ) { return 0; };
    virtual ofPtr< ofxGenericTableViewCell > getCell( ofPtr< ofxGenericTableView > tableView, unsigned int section, unsigned int index ) { return ofPtr< ofxGenericTableViewCell >(); };
    virtual float getHeightForCell( ofPtr< ofxGenericTableView > tableView, unsigned int section, unsigned int index ) { return 0.0f; };
    
    virtual void selectedRow( ofPtr< ofxGenericTableView > tableView, unsigned int section, unsigned int index) {};

    virtual ofPtr< ofxGenericView > getHeaderForSection( ofPtr< ofxGenericTableView > tableView, unsigned int section ) { return ofPtr< ofxGenericView >(); };
    virtual float getHeightForHeaderInSection( ofPtr< ofxGenericTableView > tableView, unsigned int section ) { return 0.0f; };

    virtual unsigned int getNumberOfSections( ofPtr< ofxGenericTableView > tableView ) { return 1; };
};