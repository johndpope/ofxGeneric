//
//  ofxGenericTableView.cpp
//
//  Created by Ian Grossberg on 1/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericTableView.h"
#include "ofxGenericUtility.h"

#if TARGET_OS_IPHONE
@interface ofxGenericTableViewForwarder : UITableViewController
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

ofPtr< ofxGenericTableView > ofxGenericTableView::create( const ofRectangle& setFrame )
{
    ofPtr< ofxGenericTableView > create = ofPtr< ofxGenericTableView >( new ofxGenericTableView() );
    create->init( create, setFrame );
    return create;
}

ofxGenericTableView::~ofxGenericTableView()
{
#if TARGET_OS_IPHONE
	release( _forwarder );
#endif
}

NativeView ofxGenericTableView::createNativeView( const ofRectangle& frame )
{
#if TARGET_OS_IPHONE
    UITableView* newView = [ [ UITableView alloc ] initWithFrame:ofRectangleToCGRect( frame ) style:UITableViewStylePlain ];
    _forwarder = [ [ ofxGenericTableViewForwarder alloc ] initWithDelegate:this ];
    [ newView setDelegate:_forwarder ];
    [ newView setDataSource:_forwarder ];
    [ newView setAllowsSelection:NO ];
    return newView;
#endif
}

unsigned int ofxGenericTableView::getNumberOfCells( unsigned int section )
{
    if (_delegate)
    {
        return _delegate.lock()->getNumberOfCells( dynamic_pointer_cast< ofxGenericTableView >( _this.lock() ), section );
    }
    return 0;
}

ofPtr< ofxGenericTableViewCell > ofxGenericTableView::getCell( unsigned int section, unsigned int index )
{
    if (_delegate)
    {
        return _delegate.lock()->getCell( dynamic_pointer_cast< ofxGenericTableView >( _this.lock() ), section, index );
    }
    return ofPtr< ofxGenericTableViewCell >();
}

float ofxGenericTableView::getHeightForCell( unsigned int section, unsigned int index )
{
    if (_delegate)
    {
        return _delegate.lock()->getHeightForCell( dynamic_pointer_cast< ofxGenericTableView >( _this.lock() ), section, index );
    }
    ofPtr< ofxGenericTableViewCell > cell = getCell( section, index );
    if ( cell )
    {
        return cell->getFrame().height;
    }
    return 0;
}

void ofxGenericTableView::setAutoresizeToFit( bool autoResizeToFit )
{
    _autoResizeToFit = autoResizeToFit;
    if ( _autoResizeToFit )
    {
        _maximumHeight = getFrame().height;
        resizeToFitContents();
    }
}

void ofxGenericTableView::resizeToFitContents()
{
    // TODO: support multiple sections
    unsigned int section = 0;
    unsigned int cellsPerSection = getNumberOfCells( section );
    unsigned int height = 0;
    for( int travCells = 0; travCells < cellsPerSection; travCells++ )
    {
        height += getHeightForCell( section, travCells );
    }
    if ( height > _maximumHeight )
    {
        height = _maximumHeight;
    } 
    ofRectangle resized = getFrame();
    if ( resized.height != height )
    {
        resized.height = height;
        setFrame( resized );
    }
}

void ofxGenericTableView::setSeparatorColor( const ofColor& separatorColor )
{
#if TARGET_OS_IPHONE
    UITableView* view = ( UITableView* )*this;
    if ( view )
    {
        [ view setSeparatorColor:ofColorToUIColor( separatorColor ) ];
    }
#endif
}

void ofxGenericTableView::setSeparatorStyle( ofxGenericTableViewSeparatorStyle separatorStyle )
{
#if TARGET_OS_IPHONE
    UITableView* view = ( UITableView* )*this;
    if ( view )
    {
        [ view setSeparatorStyle:ofxGenericTableViewSeparatorStyleToiOS( separatorStyle ) ];
    }
#endif    
}

void ofxGenericTableView::setDelegate( ofPtrWeak< ofxGenericTableViewDelegate > delegate )
{
    _delegate = delegate;
}

void ofxGenericTableView::reloadData()
{
#if TARGET_OS_IPHONE
    UITableView* view = ( UITableView* )*this;
    if ( view )
    {
        [ view reloadData ];
    }
#endif
    if ( _autoResizeToFit )
    {
        resizeToFitContents();
    }
}

void ofxGenericTableView::selectedRow( unsigned int section, unsigned int index)
{
    if (_delegate)
    {
        _delegate.lock()->selectedRow( dynamic_pointer_cast< ofxGenericTableView >( _this.lock() ), section, index );
    }
}

void ofxGenericTableView::setAllowsSelection( bool allow )
{
#if TARGET_OS_IPHONE
    UITableView* view = ( UITableView* )*this;
    if ( view )
    {
        view.allowsSelection = allow ? YES : NO;
    }
#endif    
}

bool ofxGenericTableView::allowsSelection()
{
#if TARGET_OS_IPHONE
    UITableView* view = ( UITableView* )*this;
    if ( view )
    {
        return view.allowsSelection == YES ? true : false;
    }
#endif    
    return false;
}

void ofxGenericTableView::setCellIsSelected( unsigned int section, unsigned int index, bool selected)
{
#if TARGET_OS_IPHONE    
    UITableView* view = ( UITableView* )*this;
    if ( view )
    {
        if (selected)
        {
            [ view selectRowAtIndexPath:[NSIndexPath indexPathForRow:index inSection:section] animated:NO scrollPosition:UITableViewScrollPositionNone];
        }
        else
        {
            [ view deselectRowAtIndexPath:[NSIndexPath indexPathForRow:index inSection:section] animated:NO];
        }
    }
    
#endif    
}

int ofxGenericTableView::getSelectedIndex()
{
#if TARGET_OS_IPHONE    
    UITableView* view = ( UITableView* )*this;
    if ( view )
    {
        NSIndexPath *p = [ view indexPathForSelectedRow ];
        if (!p)
        {
            return -1;
        }
        return p.row;
    }
#endif
    return -1;
}

int ofxGenericTableView::getSelectedSection()
{
#if TARGET_OS_IPHONE    
    UITableView* view = ( UITableView* )*this;
    if ( view )
    {
        NSIndexPath *p = [ view indexPathForSelectedRow ];
        if (!p)
        {
            return -1;
        }
        return p.section;
    }
#endif
    return -1;
}

bool ofxGenericTableView::deselectAllCells()
{
    int s = getSelectedSection();
    int i = getSelectedIndex();
    if (s >= 0 && i >= 0)
    {
        setCellIsSelected( s, i, false );
        return true;
    }
    return false;
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

ofPtr< ofxGenericTableViewCell > ofxGenericTableViewCell::create( ofPtrWeak< ofxGenericTableView > table, const ofRectangle& setBounds  )
{
    ofPtr< ofxGenericTableViewCell > c = ofPtr< ofxGenericTableViewCell >( new ofxGenericTableViewCell() );
    c->init(c, table, setBounds);
    return c;
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

void ofxGenericTableViewCell::setText( string text )
{
#if TARGET_OS_IPHONE
    UITableViewCell* cell = ( UITableViewCell* )_view;
    if (cell)
    {
        cell.textLabel.text = [NSString stringWithCString:text.c_str() encoding:NSUTF8StringEncoding];
    }
#endif
}

void ofxGenericTableViewCell::setImage( string imagePath )
{
#if TARGET_OS_IPHONE
    UITableViewCell* cell = ( UITableViewCell* )_view;
    if (cell)
    {
        cell.imageView.image = [UIImage imageNamed:[NSString stringWithCString:imagePath.c_str() encoding:NSUTF8StringEncoding]];
    }
#endif    
}

#if TARGET_OS_IPHONE
UITableViewCell* ofxGenericTableViewCell::createUITableViewCell( const CGRect& frame )
{
    return [ [ UITableViewCell alloc ] initWithFrame:frame ];
}

ofxGenericUIViewCastOperator( ofxGenericTableViewCell, UITableViewCell );

@implementation ofxGenericTableViewForwarder

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

-( void) tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    if ( _delegate )
    {
        _delegate->selectedRow( indexPath.section, indexPath.row );
    }
}


@end
#endif
