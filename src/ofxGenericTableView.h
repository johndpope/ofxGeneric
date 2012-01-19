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
    virtual ofxGenericTableViewCell* getViewForCell( unsigned int section, unsigned int index );

protected:
    virtual UIView* createUIView( const CGRect& frame );  
    ofxGenericTableViewDelegateForwarder* _forwarder;    
};

class ofxGenericTableViewCell
{
public:
    virtual ~ofxGenericTableViewCell();
    void init();
    
    UITableViewCell* getUITableViewCell();
protected:
    virtual UITableViewCell* createUITableViewCell();
    UITableViewCell* _view;
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