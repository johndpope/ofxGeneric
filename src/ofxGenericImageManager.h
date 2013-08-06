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
#include <set>

class ofxGenericImage;
class ofxGenericImageManagerDelegate;

#if TARGET_OS_IPHONE
@class ofxGenericImageManagerAsyncForwarder;
#endif

class ofxGenericImageManager
{
public:
    static ofxGenericImageManager& getInstance();
    
    virtual ~ofxGenericImageManager();
    
    virtual ofPtr< ofxGenericImage > load( const std::string& image );
    virtual std::vector< ofPtr< ofxGenericImage > > load( std::vector< std::string > images );
    virtual ofPtr< ofxGenericImage > loadAsync( const std::string& image, ofPtrWeak< ofxGenericImageManagerDelegate > delegate );
    
    virtual bool imageIsLoaded( const std::string& image );
    virtual ofPtr< ofxGenericImage > getImage( const std::string& image );
#if TARGET_OS_IPHONE
    virtual UIImage* getUIImage( const std::string& image );
    void finishedAsyncLoading( UIImage* image, const std::string& url );
    void finishedAsyncLoadingWithError( const std::string& error );
#endif
    
    
protected:
    ofxGenericImageManager();
    
    static ofPtr< ofxGenericImageManager > _this;
    virtual void init( ofPtr< ofxGenericImageManager > setThis );
    
    std::map< std::string, ofPtrWeak< ofxGenericImage > > _images;

    std::list< std::pair< std::string, ofPtrWeak< ofxGenericImageManagerDelegate > > > _asyncQueuedImages;
    std::pair< std::string, ofPtrWeak< ofxGenericImageManagerDelegate > > _currentlyLoading;
    void continueAsyncLoading();
#if TARGET_OS_IPHONE
    ofxGenericImageManagerAsyncForwarder* _asyncLoadingForwarder;
#endif
};

class ofxGenericImageManagerDelegate
{
public:
    virtual void imageManager_imageLoaded( const std::string& imageName, ofPtr< ofxGenericImage > image ) {};
    
    virtual ~ofxGenericImageManagerDelegate() {};
};

///
/// Helper class to manage a set of images.
/// Offers an api to load and free images by name or by ptr.
/// Use only if a cache of otherwise unreferenced images is needed.
///
class ofxGenericImageCache
{
public:
    static ofPtr< ofxGenericImageCache > create();
    
    ofPtr< ofxGenericImage > add( const std::string& imageName );
    void add( ofPtr< ofxGenericImage > image );
    
    void release( const std::string& imageName );
    void release( ofPtr< ofxGenericImage > image );
    
    void clear();
   
protected:
    ofPtr< ofxGenericImageCache > _this;
    ofxGenericImageCache() {};
    void init(ofPtr< ofxGenericImageCache > instance);
    
    std::set< ofPtr< ofxGenericImage > > _images;
};
