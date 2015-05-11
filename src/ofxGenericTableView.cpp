//
//  ofxGenericTableView.cpp
//
//  Created by Ian Grossberg on 1/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericTableView.h"

#include "ofxGenericTableViewCell.h"

#include "ofxGenericUtility.h"

#include <vector>

using namespace std;

#import "ofCommon.h"

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

ofPtr< ofxGenericTableView > ofxGenericTableView::create( bool areHeadersFixedInPlace, const ofRectangle& setFrame )
{
    ofPtr< ofxGenericTableView > create = ofPtr< ofxGenericTableView >( new ofxGenericTableView() );
    create->init( create, areHeadersFixedInPlace, setFrame );
    return create;
}

ofxGenericTableView::ofxGenericTableView()
: _separatorStyle( ofxGenericTableViewSeparatorStyleNone ),
    _paddedSeparatorHeight( 0.0f ),
    _cellDraggingEnabled( false ),
    _areHeadersFixedInPlace( false )
{    
}

ofxGenericTableView::~ofxGenericTableView()
{
#if TARGET_OS_IPHONE
    [ _forwarder release ];
    _forwarder = nil;
#endif
}

void ofxGenericTableView::init( ofPtrWeak< ofxGenericView > setThis, bool areHeadersFixedInPlace, const ofRectangle& setBounds, NativeView nativeView )
{
    _areHeadersFixedInPlace = areHeadersFixedInPlace;
    
    ofxGenericScrollView::init( setThis, setBounds, nativeView );
}

NativeView ofxGenericTableView::createNativeView( const ofRectangle& frame )
{
#if TARGET_OS_IPHONE
    UITableView* newView = [ [ UITableView alloc ] initWithFrame:ofxRectangleToCGRect( frame ) style:UITableViewStylePlain ];
    
    _forwarder = [ [ ofxGenericTableViewForwarder alloc ] initWithDelegate:this ];
    [ newView setDelegate:_forwarder ];
    [ newView setDataSource:_forwarder ];
    [ newView setAllowsSelection:NO ];
    
    return newView;
#endif
}

unsigned int ofxGenericTableView::getNumberOfCells( unsigned int section )
{
    if ( _delegate )
    {
        return _delegate.lock()->getNumberOfCells( dynamic_pointer_cast< ofxGenericTableView >( _this.lock() ), section );
    }
    return 0;
}

unsigned int ofxGenericTableView::nativeGetNumberOfCells( unsigned int section )
{
    unsigned int count = getNumberOfCells( section );
    
    if ( count > 0 && getSeparatorStyle() == ofxGenericTableViewSeparatorStyleSizedPadding )
    {
        count = MAX( count * 2 - 1, 0 );
    }
    
    if ( getAreHeadersFixedInPlace() && getHeaderForSection( section ) )
    {
        if ( getSeparatorStyle() == ofxGenericTableViewSeparatorStyleSizedPadding )
        {
            if ( count > 0 )
            {
                count += 2; // Header is followed by separater
            } else
            {
                count += 1;
            }
        } else
        {
            count += 1;
        }
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

ofPtr< ofxGenericTableViewCell > ofxGenericTableView::nativeGetCell( unsigned int section, unsigned int index )
{
    if ( getAreHeadersFixedInPlace() )
    {
        ofPtr< ofxGenericTableViewCell > header = getHeaderForSection( section );
        if ( header )
        {
            if ( index == 0 )
            {
                return header;
            }
            
            if ( getSeparatorStyle() == ofxGenericTableViewSeparatorStyleSizedPadding )
            {
                if ( index == 0 )
                {
                    return _paddedSeparator;
                }
                index --; // skip separator cell
            }
            
            index --; // skip header cell
        }
    }
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

unsigned int ofxGenericTableView::getNumberOfSections()
{
    if ( _delegate )
    {
        return _delegate.lock()->getNumberOfSections( dynamic_pointer_cast< ofxGenericTableView >( _this.lock() ) );
    }
    return 1;
}

ofPtr< ofxGenericTableViewCell > ofxGenericTableView::nativeGetHeaderInSection( unsigned int section )
{
    if ( !getAreHeadersFixedInPlace() )
    {
        return getHeaderForSection( section );
    }
    return ofPtr< ofxGenericTableViewCell >();
}
ofPtr< ofxGenericTableViewCell > ofxGenericTableView::getHeaderForSection( unsigned int section )
{
    if ( _delegate )
    {
        return _delegate.lock()->getHeaderForSection( dynamic_pointer_cast< ofxGenericTableView >( _this.lock() ), section );
    }
    return ofPtr< ofxGenericTableViewCell >();
}

float ofxGenericTableView::nativeGetHeightForHeaderInSection( unsigned int section )
{
    if ( !getAreHeadersFixedInPlace() )
    {
        return getHeightForHeaderInSection( section );
    }
    return 0.0f;
}


float ofxGenericTableView::getHeightForHeaderInSection( unsigned int section )
{
    if ( _delegate )
    {
        return _delegate.lock()->getHeightForHeaderInSection( dynamic_pointer_cast< ofxGenericTableView >( _this.lock() ), section );
    }
    ofPtr< ofxGenericView > header = getHeaderForSection( section );
    if ( header )
    {
        return header->getFrame().height;
    }
    return 0.0f;
}

bool ofxGenericTableView::getAreHeadersFixedInPlace()
{
    return _areHeadersFixedInPlace;
}

float ofxGenericTableView::nativeGetHeightForCell( unsigned int section, unsigned int index )
{
    if ( getAreHeadersFixedInPlace() )
    {
        ofPtr< ofxGenericTableViewCell > header = getHeaderForSection( section );
        if ( header )
        {
            if ( index == 0 )
            {
                return getHeightForHeaderInSection( section );
            }
            
            if ( getSeparatorStyle() == ofxGenericTableViewSeparatorStyleSizedPadding )
            {
                if ( index == 1 )
                {
                    return _paddedSeparatorHeight;
                }
                index --; // skip separator cell
            }
            
            index --; // skip header cell
        }
    }
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

float ofxGenericTableView::nativeGetContentHeight()
{
    float height = 0.0f;
    for( unsigned int travSections = 0; travSections < getNumberOfSections(); travSections ++ )
    {
        height += nativeGetHeightForHeaderInSection( travSections );
        for ( unsigned int travCells = 0; travCells < nativeGetNumberOfCells( travSections ); travCells ++ )
        {
            height += nativeGetHeightForCell( travSections, travCells );
        }
    }
    return height;
}

void ofxGenericTableView::resizeToFitContents()
{
    float height = nativeGetContentHeight();
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
        [ view setSeparatorColor:ofxColorToUIColor( separatorColor ) ];
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

void ofxGenericTableView::setUsePaddedSeparator( float height )
{
    setSeparatorStyle( ofxGenericTableViewSeparatorStyleSizedPadding );
    setSeparatorPaddedHeight( height );
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
    ofPtr< ofxGenericTableViewCell > cell = getCell( section, index );
    if ( cell )
    {
        cell->selected();
    }
    if (_delegate)
    {
        _delegate.lock()->selectedRow( dynamic_pointer_cast< ofxGenericTableView >( _this.lock() ), section, index );
    }
}

void ofxGenericTableView::nativeSelectedRow( unsigned int section, unsigned int index )
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

void ofxGenericTableView::setCellDragging( bool enabled )
{
    _cellDraggingEnabled = enabled;
#if TARGET_OS_IPHONE
    UITableView* tableView = *this;
    if ( tableView )
    {
        [ tableView setEditing:( BOOL )enabled animated:NO ];
    }
#endif
}

bool ofxGenericTableView::getCellDraggingEnabled( unsigned int section, unsigned int index )
{
    ofPtr< ofxGenericTableViewCell > cell = getCell( section, index );
    if ( cell )
    {
        return cell->canBeDragged();
    }
    return _cellDraggingEnabled;
}

void ofxGenericTableView::moveRow( unsigned int sourceSection, unsigned int sourceIndex, unsigned int destinationSection, unsigned int destinationIndex )
{
    if ( _delegate )
    {
        _delegate.lock()->moveRow( dynamic_pointer_cast< ofxGenericTableView >( _this ), sourceSection, sourceIndex, destinationSection, destinationIndex );
    }
}

void ofxGenericTableView::reloadCell( unsigned int section, unsigned int index )
{
#if TARGET_OS_IPHONE
    UITableView* tableView = *this;
    if ( tableView )
    {
        NSArray* updateIndexPaths = [ NSArray arrayWithObjects:[ NSIndexPath indexPathForRow:index inSection:section ], nil ];
        [ tableView beginUpdates ];
        [ tableView reloadRowsAtIndexPaths:updateIndexPaths withRowAnimation:UITableViewRowAnimationNone ];
        [ tableView endUpdates ];
    }    
#endif
}

void ofxGenericTableView::scrollToCell( unsigned int section, unsigned int index )
{
#if TARGET_OS_IPHONE
    UITableView* tableView = *this;
    if ( tableView )
    {
        [ tableView scrollToRowAtIndexPath:[ NSIndexPath indexPathForRow:index inSection:section ] atScrollPosition:UITableViewScrollPositionTop animated:YES ];
    }    
#endif
}

void ofxGenericTableView::setHeaderView( ofPtr< ofxGenericView > headerView )
{
    if ( headerView )
    {
    
#if TARGET_OS_IPHONE
        UITableView* tableView = *this;
        if ( tableView )
        {
            tableView.tableHeaderView = headerView->getNativeView();
        }
#endif
        
    }
}

void ofxGenericTableView::setFooterView( ofPtr< ofxGenericView > footerView )
{
    if ( footerView )
    {
    
#if TARGET_OS_IPHONE
        UITableView* tableView = *this;
        if ( tableView )
        {
            tableView.tableFooterView = footerView->getNativeView();
        }
#endif
        
    }
}

#if TARGET_OS_IPHONE
ofxGenericUIViewCastOperator( ofxGenericTableView, UITableView );

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
        return _delegate->nativeGetHeightForHeaderInSection( section );
    }
    return 0.0f;
}

-( UIView* )tableView:( UITableView* )tableView viewForHeaderInSection:( NSInteger )section
{
    if ( _delegate )
    {
        ofPtr< ofxGenericTableViewCell > header = _delegate->nativeGetHeaderInSection( section );
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
        return _delegate->nativeGetNumberOfCells( section );
    }
    return 0;
}

-( UITableViewCell* )tableView:( UITableView* )tableView cellForRowAtIndexPath:( NSIndexPath* )indexPath
{
    if ( _delegate )
    {
        ofPtr< ofxGenericTableViewCell > view = _delegate->nativeGetCell( [ indexPath section ], [ indexPath row ] );
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
        return _delegate->nativeGetHeightForCell( [ indexPath section ], [ indexPath row ] );
    }
    return 0.0f;
}

-( void) tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    if ( _delegate )
    {
        _delegate->nativeSelectedRow( indexPath.section, indexPath.row );
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

// TODO: support editing for realsies
-( BOOL )tableView:( UITableView* )tableView canEditRowAtIndexPath:( NSIndexPath* )indexPath
{
    return NO;
}

-( BOOL )tableView:( UITableView* )tableView canMoveRowAtIndexPath:( NSIndexPath* )indexPath
{
    if ( _delegate )
    {
        return ( BOOL )_delegate->getCellDraggingEnabled( indexPath.section, indexPath.row );
    }
    return NO;
}

-( void )tableView:( UITableView* )tableView moveRowAtIndexPath:( NSIndexPath* )sourceIndexPath toIndexPath:( NSIndexPath* )destinationIndexPath
{
    if ( ( sourceIndexPath.section != destinationIndexPath.section || sourceIndexPath.row != destinationIndexPath.row ) && _delegate )
    {
        _delegate->moveRow( sourceIndexPath.section, sourceIndexPath.row, destinationIndexPath.section, destinationIndexPath.row );
    }
}

-( UITableViewCellEditingStyle )tableView:( UITableView* )tableView editingStyleForRowAtIndexPath:( NSIndexPath* )indexPath
{
    return UITableViewCellEditingStyleNone;
}

@end
#endif
