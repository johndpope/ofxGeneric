//
//  ofxGenericDelegateContainer.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 12/28/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"

#include <list>

template< class DelegateType >
class ofxGenericDelegateContainer
{
public:
    ofxGenericDelegateContainer() {}
    
    void add( ofPtrWeak< DelegateType > delegate, bool perminent )
    {
        if ( delegate )
        {
            if ( perminent )
            {
                if ( !contains( delegate, true ) )
                {
                    _perminentDelegates.push_back( delegate );
                }
            } else
            {
                if ( !contains( delegate, false ) )
                {
                    _oneTimeDelegates.push_back( delegate );
                }
            }
        }
    }
    
    std::list< ofPtrWeak< DelegateType > > get( bool perminent )
    {
        {
            if ( perminent )
            {
                return _perminentDelegates;
            } else
            {
                // arg _oneTimeDelegates being copied twice :_(
                std::list< ofPtrWeak< DelegateType > > oneTime = _oneTimeDelegates;
                _oneTimeDelegates.clear();
                return oneTime;
            }
        }
    }
        
    bool contains( ofPtrWeak< DelegateType > check )
    {
        if ( contains( check, false ) || contains( check, true ) )
        {
            return true;
        }
        return false;
    }
    
    bool contains( ofPtrWeak< DelegateType > check, bool perminent )
    {
        const std::list< ofPtrWeak< DelegateType > >* container = NULL;
        if ( perminent )
        {
            container = &_perminentDelegates;
        } else
        {
            container = &_oneTimeDelegates;
        }
        
        if ( container )
        {
            return ofxGContains( *container, check );
        }
        return false;
    }

    virtual ~ofxGenericDelegateContainer() {};
    
protected:
    std::list< ofPtrWeak< DelegateType > > _oneTimeDelegates;
    std::list< ofPtrWeak< DelegateType > > _perminentDelegates;
};