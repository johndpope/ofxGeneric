//
//  ofxGenericDelegateContainer.cpp
//  ofxGeneric
//
//  Created by Ian Grossberg on 12/28/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericDelegateContainer.h"

ofxGenericDelegateContainer::ofxGenericDelegateContainer()
{
}

ofxGenericDelegateContainer::~ofxGenericDelegateContainer()
{
}

void ofxGenericDelegateContainer::add( ofPtrWeak< ofxGenericDelegate > delegate, bool perminent )
{
    if ( delegate )
    {
        if ( perminent )
        {
            if ( !ofxGenericDelegateContainer::contains( _perminentDelegates, delegate ) )
            {
                _perminentDelegates.push_back( delegate );
            }
        } else
        {
            if ( !ofxGenericDelegateContainer::contains( _oneTimeDelegates, delegate ) )
            {
                _oneTimeDelegates.push_back( delegate );
            }
        }
    }
}

bool ofxGenericDelegateContainer::contains( std::vector< ofPtrWeak< ofxGenericDelegate > >& list, ofPtrWeak< ofxGenericDelegate > delegate )
{
    return std::find( list.begin(), list.end(), delegate ) != list.end();
}

std::vector< ofPtrWeak< ofxGenericDelegate > > ofxGenericDelegateContainer::get( bool perminent )
{
    if ( perminent )
    {
        return _perminentDelegates;
    } else
    {
        // arg _oneTimeDelegates being copied twice :_(
        std::vector< ofPtrWeak< ofxGenericDelegate > > oneTime = _oneTimeDelegates;
        _oneTimeDelegates.clear();
        return oneTime;
    }
}