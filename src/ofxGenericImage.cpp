//
//  ofxGenericImage.cpp
//  ofxGeneric
//
//  Created by Eli Delventhal on 10/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericImage.h"
#include "ofxGenericUtility.h"
#include "UIDevice-Hardware.h"

#if TARGET_OS_IPHONE
#include <UIKit/UIKit.h>
#endif

ofPtr< ofxGenericImage > ofxGenericImage::create( std::string fileName )
{
    ofPtr< ofxGenericImage > create( new ofxGenericImage() );
    create->init( create, fileName );
    return create;
}

ofPtr< ofxGenericImage > ofxGenericImage::create( ofPtr< ofImage > image )
{
    ofPtr< ofxGenericImage > create( new ofxGenericImage() );
    create->init( create, image );
    return create;
}


std::string ofxGenericImage::getNativeImagePath( std::string fileName )
{
#if TARGET_OS_IPHONE
    std::string resolutionFileName = fileName;
    if ( [ [ UIDevice currentDevice ] hasRetinaDisplay ] )
    {
        resolutionFileName = ofFilePath::removeExt( fileName );
        resolutionFileName += "@2x.";
        resolutionFileName += ofFilePath::getFileExt( fileName );
        if ( !ofxGFileExists( resolutionFileName, false ) )
        {
            resolutionFileName = fileName;
        }
    }
    return ofxGPathToDataFolder( resolutionFileName );
#elif TARGET_ANDROID
    return ofxGPathToDataFolder( fileName );
#endif
}

ofxGenericImage::ofxGenericImage()
#if TARGET_OS_IPHONE
: _image( nil )
#endif
{
}

void ofxGenericImage::init( ofPtrWeak< ofxGenericImage > setThis, std::string fileName )
{
    _this = setThis;
    
    _filePath = getNativeImagePath( fileName );

#if TARGET_OS_IPHONE
    if ( !ofxGFileExists( fileName, false ) )
    {
        _image = nil;
        return;
    }

    _image = [ [ UIImage imageWithContentsOfFile: ofxStringToNSString( _filePath ) ] retain ];
#elif TARGET_ANDROID
    
#endif
    
}

void ofxGenericImage::init( ofPtrWeak< ofxGenericImage > setThis, ofPtr< ofImage > image )
{
    _this = setThis;
    
    _filePath = "";

#if TARGET_OS_IPHONE
    if ( image )
    {
        _image = [ [ UIImage imageWithData:UIImagePNGRepresentation( OFImageToUIImage( *image ) ) ] retain ];
    }
    else
    {
        _image = nil;
    }
#elif TARGET_ANDROID
    
#endif
    
}

ofxGenericImage::~ofxGenericImage()
{
#if TARGET_OS_IPHONE
    if ( _image )
    {
        [ _image release ];
        _image = nil;
    }
#elif TARGET_ANDROID
    
#endif
}

#if TARGET_OS_IPHONE
UIImage* ofxGenericImage::getUIImage()
{
    return _image;
}
#endif

bool ofxGenericImage::loadedSuccessfully()
{
    if ( _image == NativeNull )
    {
        return false;
    }
    return true;
}