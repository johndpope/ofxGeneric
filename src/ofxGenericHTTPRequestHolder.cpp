//
//  ofxGenericHTTPRequestHolder.cpp
//  ofxGeneric
//
//  Created by Ian Grossberg on 8/5/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericHTTPRequestHolder.h"

ofPtr< ofxGenericHTTPRequestHolder > ofxGenericHTTPRequestHolder::_instance;

ofxGenericHTTPRequestHolder::ofxGenericHTTPRequestHolder()
{
}

ofxGenericHTTPRequestHolder::~ofxGenericHTTPRequestHolder()
{
}

ofPtr< ofxGenericHTTPRequestHolder > ofxGenericHTTPRequestHolder::createInstance()
{
    if ( !( ofxGenericHTTPRequestHolder::_instance ) )
    {
        ( new ofxGenericHTTPRequestHolder() )->setofxGenericHTTPRequestHolderInstanceToThis();
    }
    return ofxGenericHTTPRequestHolder::_instance;
}


ofPtr< ofxGenericHTTPRequestHolder > ofxGenericHTTPRequestHolder::getInstance()
{
    if ( !( ofxGenericHTTPRequestHolder::_instance ) )
    {
        createInstance();
    }
    return ofxGenericHTTPRequestHolder::_instance;
}

void ofxGenericHTTPRequestHolder::setofxGenericHTTPRequestHolderInstanceToThis()
{
    if ( ofxGenericHTTPRequestHolder::_instance == NULL )
    {
        ofxGenericHTTPRequestHolder::_instance = ofPtr< ofxGenericHTTPRequestHolder >( this );
    }
}

void ofxGenericHTTPRequestHolder::holdRequestUntilComplete( ofPtr< ofxGenericHTTPRequest > request )
{
    std::list< ofPtr< ofxGenericHTTPRequest > >::iterator find = std::find( _holdRequestUntilComplete.begin(), _holdRequestUntilComplete.end(), request );
    if ( find != _holdRequestUntilComplete.end() )
    {
        return;
    }
    request->setDelegate( ofxGenericHTTPRequestHolder::_instance );
    _holdRequestUntilComplete.push_back( request );
}

void ofxGenericHTTPRequestHolder::httpRequest_finished( ofPtr< ofxGenericHTTPRequest > request )
{
    removeHeldRequest( request );
}

void ofxGenericHTTPRequestHolder::removeHeldRequest( ofPtr< ofxGenericHTTPRequest > request )
{
    _holdRequestUntilComplete.remove( request );
}