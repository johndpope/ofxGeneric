//
//  ofxGenericImageManager.h
//  ofxGeneric
//
//  Created by Eli Delventhal on 10/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"
#include "ofxGenericConstants.h"
#include <map>

class ofxGenericImage;

class ofxGenericImageManager
{
public:
    static ofxGenericImageManager& getInstance();
    
    virtual ~ofxGenericImageManager();
    
    virtual bool load( std::string image );
    virtual bool load( std::vector< std::string > images );
    virtual void unload( std::string image );
    virtual void unload( std::vector< std::string > images );
    virtual void unloadAll();
    
    virtual bool imageIsLoaded( std::string image );
    virtual ofPtr< ofxGenericImage > getImage( std::string image );
#if TARGET_OS_IPHONE
    virtual UIImage* getUIImage( std::string image );
#endif
    
protected:
    
    ofxGenericImageManager();
    static ofPtr< ofxGenericImageManager > _this;
    virtual void init( ofPtr< ofxGenericImageManager > setThis );
    
    std::map< std::string, ofPtr< ofxGenericImage > > _images;
};