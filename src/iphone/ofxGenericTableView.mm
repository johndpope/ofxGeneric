//
//  ofxGenericTableView.mm
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 1/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#import "ofxGenericTableView.h"

UIView* ofxGenericTableView::createUIView( const CGRect& frame )
{
    UITableView* newView = [ [ UITableView alloc ] initWithFrame:frame style:UITableViewStylePlain ];
    _forwarder = [ [ ofxGenericTableViewDelegateForwarder alloc ] initWithDelegate:this ];
    [ newView setDelegate:_forwarder ];
    return newView;
}

unsigned int ofxGenericTableView::getNumberOfCells( unsigned int section )
{
    return 0;
}

ofxGenericTableViewCell* ofxGenericTableView::getViewForCell( unsigned int section, unsigned int index )
{
    return NULL;
}

ofxGenericTableViewCell::~ofxGenericTableViewCell()
{
    [ _view release ];
    _view = nil;
}

void ofxGenericTableViewCell::init()
{
    _view = createUITableViewCell();
}

UITableViewCell* ofxGenericTableViewCell::getUITableViewCell()
{
    return _view;
}

UITableViewCell* ofxGenericTableViewCell::createUITableViewCell()
{
    UITableViewCell* newView = [ [ UITableViewCell alloc ] init ];
    return newView;
}

@implementation ofxGenericTableViewDelegateForwarder

-( id )initWithDelegate:( ofxGenericTableView* )delegate
{
    self = [ super init ];
    _delegate = nil;
    if ( self )
    {
        _delegate = delegate;
    }
    return self;
}

-( NSInteger )tableView:( UITableView* )tableView numberOfRowsInSection:( NSInteger )section
{
    if ( _delegate )
    {
        return _delegate->getNumberOfCells( section );
    }
    return 0;
}

-( UITableViewCell* )tableView:( UITableView* )tableView cellForRowAtIndexPath:( NSIndexPath* )indexPath
{
    if ( _delegate )
    {
        ofxGenericTableViewCell* view = _delegate->getViewForCell( [ indexPath section ], [ indexPath row ] );
        if ( view )
        {
            // TODO: fix
            return ( UITableViewCell* )view->getUITableViewCell();
        }
    }
    return nil;
}

@end