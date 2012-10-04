//
//  ofxGenericHMAC.h
//  ofxGeneric
//
//  Created by Konstantin Yavichev on 10/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once


#include "ofxGenericMain.h"

class ofxGenericHMAC
{
public:
    static string createHMAC( string key, string text );
};

