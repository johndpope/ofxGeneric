//
//  ofxGenericImage.cpp
//  ofxGeneric
//
//  Created by Eli Delventhal on 10/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericImage.h"

#include "ofxGenericImageManager.h"
#include "ofxGenericUtility.h"

#if TARGET_OS_IPHONE
#include <UIKit/UIKit.h>

#endif

#import "ofCommon.h"
#include "ofUtils.h"


ofPtr< ofxGenericImage > ofxGenericImage::create( const std::string& fileName)
{
    ofxGenericImageManager& mgr = ofxGenericImageManager::getInstance();
    ofPtr< ofxGenericImage > instance = mgr.getImage(fileName);
    if( !instance )
    {
        instance = ofPtr< ofxGenericImage >( new ofxGenericImage() );
        instance->init( instance, fileName );
        mgr.add( fileName, instance );
    }
    return instance;
}

ofPtr< ofxGenericImage > ofxGenericImage::createAsync( const std::string& fileName,
                                                       ofPtrWeak< ofxGenericImageDelegate > delegate )
{
    ofPtr< ofxGenericImage > instance = ofxGenericImageManager::getInstance().loadAsync( fileName,  delegate );
    return instance;
}

/* IOSP-40 // ofPtr< ofxGenericImage > ofxGenericImage::create( ofPtr< ofImage > image, const std::string& fromFileName )
{
    ofPtr< ofxGenericImage > create( new ofxGenericImage() );
    create->init( create, image, fromFileName );
    return create;
}*/

#if TARGET_OS_IPHONE
ofPtr< ofxGenericImage > ofxGenericImage::create( UIImage* image, const std::string& fromFileName )
{
    ofPtr< ofxGenericImage > create( new ofxGenericImage() );
    create->init( create, image, fromFileName );
    return create;
}
#endif


ofxGenericImage::ofxGenericImage()
#if TARGET_OS_IPHONE
: _image( nil )
#endif
{
}

void ofxGenericImage::init( ofPtrWeak< ofxGenericImage > setThis, const std::string& fileName)
{
    _this = setThis;
    
    _filePath = ofxGenericImageManager::getNativeImagePath( fileName );

#if TARGET_OS_IPHONE
    _image = nil;
    if ( ofxGFileExists( _filePath, false ) )
    {
        _image = [ [ UIImage imageWithContentsOfFile: ofxStringToNSString( _filePath ) ] retain ];
    }
    
#elif TARGET_ANDROID
    
#endif
    
}

/* IOSP-40 // void ofxGenericImage::init( ofPtrWeak< ofxGenericImage > setThis, ofPtr< ofImage > image, const std::string& fromFileName )
{
    _this = setThis;
    
    _filePath = fromFileName;
    
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
    
}*/

#if TARGET_OS_IPHONE
void ofxGenericImage::init( ofPtrWeak< ofxGenericImage > setThis, UIImage* image, const std::string& fromFileName )
{
    _this = setThis;
    
    _filePath = fromFileName;
    
    _image = [ image retain ];
}
#endif


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

bool ofxGenericImage::save( string fileName, bool inDocuments )
{
    bool result = false;
    
#if TARGET_OS_IPHONE
    if ( _image != nil)
    {
        string path = ofToPath( fileName, inDocuments, true );
        
        NSData* data = UIImagePNGRepresentation( _image );
        [ data writeToFile:ofxStringToNSString( path ) atomically:YES ];
        
        result = true;
    }
#else
    throw ofxGenericExceptionMemberNotImplement( "ofxGenericImage", "save" );
#endif
    
    return result;
}

std::string ofxGenericImage::getFilePath()
{
    return _filePath;
}

ofPoint ofxGenericImage::getSize()
{
#if TARGET_OS_IPHONE
    return CGSizeToofPoint( _image.size );
#elif TARGET_ANDROID
    return ofPoint( 0,0 );
#endif
}

ofPtr< ofxGenericImageCache > ofxGenericImageCache::create()
{
    ofPtr< ofxGenericImageCache > instance(new ofxGenericImageCache);
    instance->init(instance);
    return instance;
}

void ofxGenericImageCache::init(ofPtr< ofxGenericImageCache > instance)
{
    _this = instance;
}

ofPtr< ofxGenericImage > ofxGenericImageCache::add( const std::string& imageName )
{
    ofPtr< ofxGenericImage > image = ofxGenericImage::create( imageName );
    if( image )
    {
        add( image );
    }
    return image;
}

void ofxGenericImageCache::add( ofPtr< ofxGenericImage > image )
{
    _images.insert(image);
}

void ofxGenericImageCache::release( const std::string& imageName )
{
    ofPtr< ofxGenericImage > image = ofxGenericImageManager::getInstance().getImage( imageName );
    release( image );
}

void ofxGenericImageCache::release( ofPtr< ofxGenericImage > image )
{
    _images.erase(image);
}

void ofxGenericImageCache::clear()
{
    _images.clear();
}

