//
//  ofxGenericImage.cpp
//  ofxGeneric
//
//  Created by Eli Delventhal on 10/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericImage.h"
#include "ofxGenericUtility.h"

#if TARGET_OS_IPHONE
#include <UIKit/UIKit.h>

#include "ofxGenericPlatform.h"
#endif

ofPtr< ofxGenericImage > ofxGenericImage::create( std::string fileName )
{
    ofPtr< ofxGenericImage > create( new ofxGenericImage() );
    create->init( create, fileName );
    return create;
}

#if TARGET_OS_IPHONE
ofPtr< ofxGenericImage > ofxGenericImage::create( UIImage* image, std::string fromFileName )
{
    ofPtr< ofxGenericImage > create( new ofxGenericImage() );
    create->init( create, image, fromFileName );
    return create;
}
#endif

ofPtr< ofxGenericImage > ofxGenericImage::create( ofPtr< ofImage > image, std::string fromFileName )
{
    ofPtr< ofxGenericImage > create( new ofxGenericImage() );
    create->init( create, image, fromFileName );
    return create;
}


std::string ofxGenericImage::getNativeImagePath( std::string fileName, bool makeAbsolute )
{
#if TARGET_OS_IPHONE
    if ( ofxGenericPlatform::is4InchDisplay() )
    {
        if ( ofxGenericPlatform::isRetinaDisplay() )
        {
            string test = ofxGenericPlatform::imageFileName( fileName, true, false, true );
            if ( ofxGFileExists( test, false ) )
            {
                return ofToPath( test, false );
            }
        }
        
        string test = ofxGenericPlatform::imageFileName( fileName, true, false, false );
        if ( ofxGFileExists( test, false ) )
        {
            if ( makeAbsolute )
            {
                test = ofToPath( test, false );
            }
            return test;
        }
    } else if ( ofxGenericPlatform::isTablet() )
    {
        if ( ofxGenericPlatform::isRetinaDisplay() )
        {
            string test = ofxGenericPlatform::imageFileName( fileName, false, true, true );
            if ( ofxGFileExists( test, false ) )
            {
                return ofToPath( test, false );
            }
        }
        
        string test = ofxGenericPlatform::imageFileName( fileName, false, true, false );
        if ( ofxGFileExists( test, false ) )
        {
            if ( makeAbsolute )
            {
                test = ofToPath( test, false );
            }
            return test;
        }
    }
    
    if ( ofxGenericPlatform::isRetinaDisplay() )
    {
        string test = ofxGenericPlatform::imageFileName( fileName, false, false, true );
        if ( ofxGFileExists( test, false ) )
        {
            if ( makeAbsolute )
            {
                test = ofToPath( test, false );
            }
            return test;
        }
    }
    
#elif TARGET_ANDROID
#endif
    if ( makeAbsolute )
    {
        fileName = ofToPath( fileName, false );
    }
    return fileName;
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

#if TARGET_OS_IPHONE
void ofxGenericImage::init( ofPtrWeak< ofxGenericImage > setThis, UIImage* image, std::string fromFileName )
{
    _this = setThis;
    
    _filePath = fromFileName;
    
    _image = [ image retain ];
}
#endif

void ofxGenericImage::init( ofPtrWeak< ofxGenericImage > setThis, ofPtr< ofImage > image, std::string fromFileName )
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