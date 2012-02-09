//
//  ofxGenericTableView.cpp
//
//  Created by Ian Grossberg on 1/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericTableView.h"
#include "ofxGenericUtility.h"

ofxGenericTableView::~ofxGenericTableView()
{
#if TARGET_OS_IPHONE
	release( _forwarder );
#endif
}

#if TARGET_OS_IPHONE
UIView* ofxGenericTableView::createUIView( const CGRect& frame )
{
    UITableView* newView = [ [ UITableView alloc ] initWithFrame:frame style:UITableViewStylePlain ];
    _forwarder = [ [ ofxGenericTableViewDelegateForwarder alloc ] initWithDelegate:this ];
    [ newView setDelegate:_forwarder ];
    [ newView setDataSource:_forwarder ];
    return newView;
}
#endif

unsigned int ofxGenericTableView::getNumberOfCells( unsigned int section )
{
    return 0;
}

ofPtr< ofxGenericTableViewCell > ofxGenericTableView::getCell( unsigned int section, unsigned int index )
{
    return ofPtr< ofxGenericTableViewCell >();
}

float ofxGenericTableView::getHeightForCell( unsigned int section, unsigned int index )
{
    ofPtr< ofxGenericTableViewCell > cell = getCell( section, index );
    if ( cell )
    {
        return cell->getBounds().height;
    }
    return 0;
}

#if TARGET_OS_IPHONE
ofxGenericUIViewCastOperator( ofxGenericTableView, UITableView );
#endif

ofxGenericTableViewCell::~ofxGenericTableViewCell()
{
#if TARGET_OS_IPHONE
    releaseView( _view );
#endif
}

void ofxGenericTableViewCell::init( ofPtr< ofxGenericTableViewCell > setThis, ofPtrWeak< ofxGenericTableView > table, const ofRectangle& setBounds )
{
    // TODO: skipping parent init is too careless and potentially dangerous, come up with better scheme so we aren't creating unnecessary _view in parent init
    //    ofxGenericView::init( setThis, setBounds );
    _this = setThis;

    _table = table;
#if TARGET_OS_IPHONE
    _view = createUITableViewCell( ofRectangleToCGRect( setBounds ) );
    [ _view setFrame:ofRectangleToCGRect( setBounds ) ];
#endif
}

#if TARGET_OS_IPHONE
UITableViewCell* ofxGenericTableViewCell::createUITableViewCell( const CGRect& frame )
{
    return [ [ UITableViewCell alloc ] initWithFrame:frame ];
}

ofxGenericUIViewCastOperator( ofxGenericTableViewCell, UITableViewCell );

@implementation ofxGenericTableViewDelegateForwarder

-( id )initWithDelegate:( ofxGenericTableView* )delegate
{
    self = [ super init ];
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
        ofPtr< ofxGenericTableViewCell > view = _delegate->getCell( [ indexPath section ], [ indexPath row ] );
        if ( view )
        {
            return *view;
        }
    }
    return nil;
}

-( CGFloat )tableView:( UITableView* )tableView heightForRowAtIndexPath:( NSIndexPath* )indexPath
{
    if ( _delegate )
    {
        return _delegate->getHeightForCell( [ indexPath section ], [ indexPath row ] );
    }
    return 0;
}


@end
#endif