//
//  ofxGenericUI.h
//  iPhone+OF Lib
//
//  Created by Ian Grossberg on 12/28/11.
//  Copyright (c) 2011 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "ofxAppGenericWindow.h"

#include "ofAppRunner.h"

void ofGenericSetupOpenGL( int w, int h, int screenMode )
{
    ofPtr< ofAppBaseWindow > window = ofPtr< ofAppBaseWindow >( new ofxAppGenericWindow() );
    
	ofSetupOpenGL( window,w,h,screenMode );
}


