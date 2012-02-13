//
//  ofxGenericTableView.h
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 1/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericView.h"

#if TARGET_OS_IPHONE
@class ofxGenericTableViewDelegateForwarder;
#endif
class ofxGenericTableViewCell;

class ofxGenericTableView : public ofxGenericView
{
public:
    virtual ~ofxGenericTableView();
    virtual unsigned int getNumberOfCells( unsigned int section );
    virtual ofPtr< ofxGenericTableViewCell > getCell( unsigned int section, unsigned int index );
    virtual float getHeightForCell( unsigned int section, unsigned int index );

#if TARGET_OS_IPHONE
    operator UITableView*();
#endif

protected:
    virtual NativeView createNativeView( const ofRectangle& frame );
#if TARGET_OS_IPHONE
    ofxGenericTableViewDelegateForwarder* _forwarder;    
#endif
};

class ofxGenericTableViewCell : public ofxGenericView
{
public:
    virtual ~ofxGenericTableViewCell();
    virtual void init( ofPtr< ofxGenericTableViewCell > setThis, ofPtrWeak< ofxGenericTableView > table, const ofRectangle& setBounds = ofRectangle( 0, 0, 0, 0 )  );
    
#if TARGET_OS_IPHONE
    operator UITableViewCell*();
#endif

protected:
    ofPtrWeak< ofxGenericTableView > _table;
    
#if TARGET_OS_IPHONE
    virtual UITableViewCell* createUITableViewCell( const CGRect& frame );
#endif
};

#if TARGET_OS_IPHONE
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
#endif
