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
    
    void add( ofPtrWeak< DelegateType > delegate, bool permanent )
    {
        if ( delegate )
        {
            if ( permanent )
            {
                if ( !contains( delegate, true ) )
                {
                    _permanentDelegates.push_back( delegate );
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
    
    std::list< ofPtrWeak< DelegateType > > get( bool permanent )
    {
        {
            if ( permanent )
            {
                return _permanentDelegates;
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
    
    bool contains( ofPtrWeak< DelegateType > check, bool permanent )
    {
        const std::list< ofPtrWeak< DelegateType > >* container = NULL;
        if ( permanent )
        {
            container = &_permanentDelegates;
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
    
    void remove( ofPtrWeak< DelegateType > remove )
    {
        _oneTimeDelegates.remove( remove );
        _permanentDelegates.remove( remove );
    }

    virtual ~ofxGenericDelegateContainer() {};
    
protected:
    std::list< ofPtrWeak< DelegateType > > _oneTimeDelegates;
    std::list< ofPtrWeak< DelegateType > > _permanentDelegates;
};