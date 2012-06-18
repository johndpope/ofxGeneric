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

ofxGenericTableView::ofxGenericTableView()
: _separatorStyle( ofxGenericTableViewSeparatorStyleNone ), _paddedSeparatorHeight( 0.0f )
{    
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

void ofxGenericTableViewCell::setTablePosition( unsigned int section, unsigned int index )
{
    _section = section;
    _index = index;
}

unsigned int ofxGenericTableViewCell::getSection()
{
    return _section;
}

unsigned int ofxGenericTableViewCell::getIndex()
{
    return _index;
}

unsigned int ofxGenericTableView::getNumberOfCells( unsigned int section )
{
    if ( _delegate )
    {
        return _delegate.lock()->getNumberOfCells( dynamic_pointer_cast< ofxGenericTableView >( _this.lock() ), section );
    }
    return 0;
}

unsigned int ofxGenericTableView::internalGetNumberOfCells( unsigned int section )
{
    unsigned int count = getNumberOfCells( section );
    if ( getSeparatorStyle() == ofxGenericTableViewSeparatorStyleSizedPadding )
    {
        count = MAX( count * 2 - 1, 0 );
    }
    return count;
}

ofPtr< ofxGenericTableViewCell > ofxGenericTableView::getCell( unsigned int section, unsigned int index )
{
    if ( _delegate )
    {
        return _delegate.lock()->getCell( dynamic_pointer_cast< ofxGenericTableView >( _this.lock() ), section, index );
    }
    return ofPtr< ofxGenericTableViewCell >();
}

ofPtr< ofxGenericTableViewCell > ofxGenericTableView::internalGetCell( unsigned int section, unsigned int index )
{
    if ( getSeparatorStyle() == ofxGenericTableViewSeparatorStyleSizedPadding )
    {
        if ( index % 2 == 1 )
        {
            return _paddedSeparator;
        }
        index = index / 2;
    }
    return getCell( section, index );
}

float ofxGenericTableView::getHeightForCell( unsigned int section, unsigned int index )
{
    if ( _delegate )
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

int ofxGenericTableView::getNumberOfSections()
{
    if ( _delegate )
    {
        return _delegate.lock()->getNumberOfSections();
    }
    return 1;
}

ofPtr< ofxGenericView > ofxGenericTableView::getHeaderForSection( unsigned int section )
{
    if ( _delegate )
    {
        return _delegate.lock()->getHeaderForSection( section );
    }
    return ofPtr< ofxGenericView >();
}

float ofxGenericTableView::getHeightForHeaderInSection( unsigned int section )
{
    if ( _delegate )
    {
        return _delegate.lock()->getHeightForHeaderInSection( section );
    }
    ofPtr< ofxGenericView > header = getHeaderForSection( section );
    if ( header )
    {
        return header->getFrame().height;
    }
    return 0.0f;
}

float ofxGenericTableView::internalGetHeightForCell( unsigned int section, unsigned int index )
{
    if ( getSeparatorStyle() == ofxGenericTableViewSeparatorStyleSizedPadding )
    {
        if ( index % 2 == 1 )
        {
            return _paddedSeparatorHeight;
        }
        index = index / 2;
    }
    return getHeightForCell( section, index );
}

void ofxGenericTableView::setAutoresizeToFit( bool autoResizeToFit )
{
    _autoResizeToFit = autoResizeToFit;
    if ( _autoResizeToFit )
    {
        _maximumHeight = getFrame().height;
        if ( _isAttachedToRoot )
        {
            resizeToFitContents();
        }
    }
}

float ofxGenericTableView::getContentHeight()
{
    float height = 0.0f;
    for( unsigned int travSections = 0; travSections < getNumberOfSections(); travSections ++ )
    {
        height += getHeightForHeaderInSection( travSections );
        for ( unsigned int travCells = 0; travCells < getNumberOfCells( travSections ); travCells ++ )
        {
            height += getHeightForCell( travSections, travCells );
        }
    }
    return height;
}

void ofxGenericTableView::resizeToFitContents()
{
    float height = getContentHeight();
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
    _separatorStyle = separatorStyle;
    if ( _separatorStyle == ofxGenericTableViewSeparatorStyleSizedPadding )
    {
        _paddedSeparator = ofxGenericTableViewCell::create( dynamic_pointer_cast< ofxGenericTableView >( _this ) );
    } else 
    {
        _paddedSeparator = ofPtr< ofxGenericTableViewCell >();
    }
#if TARGET_OS_IPHONE
    UITableView* view = ( UITableView* )*this;
    if ( view )
    {
        [ view setSeparatorStyle:ofxGenericTableViewSeparatorStyleToiOS( separatorStyle ) ];
    }
#endif    
}

ofxGenericTableViewSeparatorStyle ofxGenericTableView::getSeparatorStyle()
{
    return _separatorStyle;
}

void ofxGenericTableView::setSeparatorPaddedHeight( float height )
{
    _paddedSeparatorHeight = height;
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

void ofxGenericTableView::internalSelectedRow( unsigned int section, unsigned int index )
{
    if ( getSeparatorStyle() == ofxGenericTableViewSeparatorStyleSizedPadding )
    {
        if ( index % 2 == 1 )
        {
            return;
        }
        index = index / 2;
    }
    selectedRow( section, index );
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

////////////////////////////////////////////////////////////////////////////////////////////////////

ofxGenericTableViewCell::~ofxGenericTableViewCell()
{
#if TARGET_OS_IPHONE
    releaseView( _view );
#endif
}

ofPtr< ofxGenericTableViewCell > ofxGenericTableViewCell::create( ofPtrWeak< ofxGenericTableView > table, unsigned int section, unsigned int index, const ofRectangle& setBounds  )
{
    ofPtr< ofxGenericTableViewCell > c = ofPtr< ofxGenericTableViewCell >( new ofxGenericTableViewCell() );
    c->init(c, table, section, index, setBounds);
    return c;
}

ofxGenericTableViewCell::ofxGenericTableViewCell()
: _section( 0 ), _index( 0 )
{
    
}

void ofxGenericTableViewCell::init( ofPtrWeak< ofxGenericTableViewCell > setThis, ofPtrWeak< ofxGenericTableView > table, unsigned int section, unsigned int index, const ofRectangle& setBounds )
{
    _table = table;
    _section = section;
    _index = index;
    ofxGenericView::init( setThis );
}

ofPtr< ofxGenericView > ofxGenericTableViewCell::getContentView()
{
    return _contentView;
}

void ofxGenericTableViewCell::addChildView( ofPtr< ofxGenericView > add )
{
    if ( _contentView )
    {
        ofxGenericView::addChildViewTo( _contentView, add );
    }
    else
    {
        ofxGenericView::addChildView( add );
    }
}

void ofxGenericTableViewCell::addChildView( ofPtr< ofxGenericView > add, ofPtr< ofxGenericView > before )
{
    if ( _contentView )
    {
        ofxGenericView::addChildViewTo( _contentView, add, before );
    }
    else
    {
        ofxGenericView::addChildView( add, before );
    }
}

void ofxGenericTableViewCell::removeChildView( ofPtr< ofxGenericView > remove )
{
    if ( _contentView )
    {
        ofxGenericView::removeChildViewFrom( _contentView, remove );
    }
    else
    {
        ofxGenericView::removeChildView( remove );
    }
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

NativeView ofxGenericTableViewCell::createNativeView( const ofRectangle& frame )
{
#if TARGET_OS_IPHONE
    UITableViewCell* nativeView = [ [ UITableViewCell alloc ] initWithFrame:ofRectangleToCGRect( frame ) ];
    _contentView = ofxGenericView::create( CGRectToofRectangle( nativeView.contentView.frame ), nativeView.contentView );
    [ nativeView setSelectionStyle:UITableViewCellSelectionStyleNone ];
    
    return nativeView;
#endif
    //TODO android
}

#if TARGET_OS_IPHONE
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

-( CGFloat )tableView:( UITableView* )tableView heightForHeaderInSection:( NSInteger )section
{
    if ( _delegate )
    {
        return _delegate->getHeightForHeaderInSection( section );
    }
    return 0.0f;
}

-( UIView* )tableView:( UITableView* )tableView viewForHeaderInSection:( NSInteger )section
{
    if ( _delegate )
    {
        ofPtr< ofxGenericView > header = _delegate->getHeaderForSection( section );
        if ( header )
        {
            return header->getNativeView();
        }
    }
    return nil;
}

-( NSInteger )tableView:( UITableView* )tableView numberOfRowsInSection:( NSInteger )section
{
    if ( _delegate )
    {
        return _delegate->internalGetNumberOfCells( section );
    }
    return 0;
}

-( UITableViewCell* )tableView:( UITableView* )tableView cellForRowAtIndexPath:( NSIndexPath* )indexPath
{
    if ( _delegate )
    {
        ofPtr< ofxGenericTableViewCell > view = _delegate->internalGetCell( [ indexPath section ], [ indexPath row ] );
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
        return _delegate->internalGetHeightForCell( [ indexPath section ], [ indexPath row ] );
    }
    return 0.0f;
}

-( void) tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    if ( _delegate )
    {
        _delegate->internalSelectedRow( indexPath.section, indexPath.row );
    }
}

-( NSInteger )numberOfSectionsInTableView:( UITableView* )tableView
{
    if ( _delegate )
    {
        return _delegate->getNumberOfSections();
    }
    return 1;
}

@end
#endif
