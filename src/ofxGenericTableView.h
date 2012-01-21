//
//  ofxGenericTableView.h
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 1/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericView.h"

@class ofxGenericTableViewDelegateForwarder;
class ofxGenericTableViewCell;

class ofxGenericTableView : public ofxGenericView
{
public:
    virtual ~ofxGenericTableView();
    virtual unsigned int getNumberOfCells( unsigned int section );
    virtual ofPtr< ofxGenericTableViewCell > getCell( unsigned int section, unsigned int index );
    virtual float getHeightForCell( unsigned int section, unsigned int index );

protected:
    virtual UIView* createUIView( const CGRect& frame );  
    ofxGenericTableViewDelegateForwarder* _forwarder;    
};

class ofxGenericTableViewCell : public ofxGenericView
{
public:
    virtual ~ofxGenericTableViewCell();
    virtual void init( ofPtr< ofxGenericTableViewCell > setThis, ofPtrWeak< ofxGenericTableView > table, const ofRectangle& setBounds = ofRectangle( 0, 0, 0, 0 )  );
    
    UITableViewCell* getUITableViewCell();
protected:
    ofPtrWeak< ofxGenericTableView > _table;
    
    virtual UITableViewCell* createUITableViewCell( const CGRect& frame );
};

@interface ofxGenericTableViewDelegateForwarder : UITableViewController
{
@private
    ofxGenericTableView* _delegate;
}
-( id )initWithDelegate:( ofxGenericTableView* )delegate;
-( NSInteger )tableView:( UITableView* )tableView numberOfRowsInSection:( NSInteger )section;
-( UITableViewCell* )tableView:( UITableView* )tableView cellForRowAtIndexPath:( NSIndexPath* )indexPath;
-( CGFloat )tableView:( UITableView* )tableView heightForRowAtIndexPath:( NSIndexPath* )indexPath;

@end