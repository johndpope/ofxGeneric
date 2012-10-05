//
//  ofxGenericImageManager.cpp
//  ofxGeneric
//
//  Created by Eli Delventhal on 10/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericImageManager.h"
#include "ofxGenericImage.h"

ofPtr< ofxGenericImageManager > ofxGenericImageManager::_this;

ofxGenericImageManager& ofxGenericImageManager::getInstance()
{
    if ( !ofxGenericImageManager::_this )
    {
        ofPtr< ofxGenericImageManager > instance = ofPtr< ofxGenericImageManager >( new ofxGenericImageManager() );
        instance->init( instance );
    }
    return *ofxGenericImageManager::_this;
}

ofxGenericImageManager::ofxGenericImageManager()
{
    
}

void ofxGenericImageManager::init( ofPtr< ofxGenericImageManager > setThis )
{
    ofxGenericImageManager::_this = setThis;
    
    _images = std::map< std::string, ofPtr< ofxGenericImage > >();
}

ofxGenericImageManager::~ofxGenericImageManager()
{
    
}

bool ofxGenericImageManager::load( std::string image )
{
    if ( !_images[ image ] )
    {
        ofPtr< ofxGenericImage > loadedImage = ofxGenericImage::create( image );
        if ( !loadedImage->loadedSuccessfully() )
        {
            return false;
        }
        _images[ image ] = loadedImage;
    }
    return true;
}

bool ofxGenericImageManager::load( std::vector< std::string > images )
{
    bool success = true;
    for ( unsigned int i = 0; i < images.size(); i++ )
    {
        success = load( images[i] ) && success;
    }
    return success;
}

void ofxGenericImageManager::unload( std::string image )
{
    _images[ image ] = ofPtr< ofxGenericImage >();
}

void ofxGenericImageManager::unload( std::vector< std::string > images )
{
    for ( unsigned int i = 0; i < images.size(); i++ )
    {
        unload( images[i] );
    }
}

void ofxGenericImageManager::unloadAll()
{
    _images = std::map< std::string, ofPtr< ofxGenericImage > >();
}

bool ofxGenericImageManager::imageIsLoaded( std::string image )
{
    if ( _images[ image ] )
    {
        return true;
    }
    return false;
}

ofPtr< ofxGenericImage > ofxGenericImageManager::getImage( std::string image )
{
    return _images[ image ];
}

#if TARGET_OS_IPHONE
UIImage* ofxGenericImageManager::getUIImage( std::string image )
{
    if ( _images[ image ] )
    {
        return _images[ image ]->getUIImage();
    }
    return nil;
}
#endif