//
//  ofxGenericEncryption.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 10/24/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"

class ofxGenericEncryption
{
public:
    static string encrypt( string raw, string key );
    static string decrypt( string encrypted, string key );
};