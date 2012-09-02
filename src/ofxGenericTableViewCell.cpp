//
//  ofxGenericTableViewCell.cpp
//  ofxGeneric
//
//  Created by Ian Grossberg on 8/15/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericTableViewCell.h"

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
    ofxGenericView::init( setThis, setBounds );
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

ofPtr< ofxGenericView > ofxGenericTableViewCell::getContentView()
{
    return _contentView;
}

void ofxGenericTableViewCell::selected()
{
}

bool ofxGenericTableViewCell::canBeDragged()
{
    return false;
}


void ofxGenericTableViewCell::addChildView( ofPtr< ofxGenericView > add )
{
    if ( _contentView )
    {
        _contentView->addChildView( add );
    }
    else
    {
        ofxGenericView::addChildView( add );
    }
}

void ofxGenericTableViewCell::addChildViewBefore( ofPtr< ofxGenericView > add, ofPtr< ofxGenericView > before )
{
    if ( _contentView )
    {
        _contentView->addChildViewBefore( add, before );
    }
    else
    {
        ofxGenericView::addChildViewBefore( add, before );
    }
}

void ofxGenericTableViewCell::addChildViewAfter( ofPtr< ofxGenericView > add, ofPtr< ofxGenericView > after )
{
    if ( _contentView )
    {
        _contentView->addChildViewAfter( add, after );
    }
    else
    {
        ofxGenericView::addChildViewAfter( add, after );
    }
}

void ofxGenericTableViewCell::removeChildView( ofPtr< ofxGenericView > remove )
{
    if ( _contentView )
    {
        _contentView->removeChildView( remove );
    }
    else
    {
        ofxGenericView::removeChildView( remove );
    }
}

void ofxGenericTableViewCell::setBackgroundColor( const ofColor& color )
{
    if ( _contentView )
    {
        _contentView->setBackgroundColor( color );
    }
    ofxGenericView::setBackgroundColor( color );
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
    [ nativeView setFrame:ofRectangleToCGRect( frame ) ];
    _contentView = ofxGenericView::create( frame, nativeView.contentView );
    [ nativeView setSelectionStyle:UITableViewCellSelectionStyleNone ];
    
    return nativeView;
#endif
    //TODO android
}

#if TARGET_OS_IPHONE
ofxGenericUIViewCastOperator( ofxGenericTableViewCell, UITableViewCell );
#endif