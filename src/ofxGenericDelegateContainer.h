//
//  ofxGenericDelegateContainer.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 12/28/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"

class ofxGenericDelegate
{
public:
    virtual ~ofxGenericDelegate() {};
};

class ofxGenericDelegateContainer
{
public:
    ofxGenericDelegateContainer();
    
    void add( ofPtrWeak< ofxGenericDelegate > delegate, bool perminent = false );
    std::vector< ofPtrWeak< ofxGenericDelegate > > get( bool perminent = false );

    virtual ~ofxGenericDelegateContainer();
    
    static bool contains( std::vector< ofPtrWeak< ofxGenericDelegate > >& list, ofPtrWeak< ofxGenericDelegate > delegate );
protected:
    std::vector< ofPtrWeak< ofxGenericDelegate > > _oneTimeDelegates;
    std::vector< ofPtrWeak< ofxGenericDelegate > > _perminentDelegates;
};