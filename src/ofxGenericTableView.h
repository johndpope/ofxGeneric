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
    virtual ofPtr< ofxGenericTableViewCell > getViewForCell( unsigned int section, unsigned int index );

protected:
    virtual UIView* createUIView( const CGRect& frame );  
    ofxGenericTableViewDelegateForwarder* _forwarder;    
};

class ofxGenericTableViewCell : public ofxGenericView
{
public:
    virtual ~ofxGenericTableViewCell();
    virtual void init( ofPtr< ofxGenericTableViewCell > setThis, ofPtrWeak< ofxGenericTableView > table, const ofRectangle& setBounds  );
    
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

@end