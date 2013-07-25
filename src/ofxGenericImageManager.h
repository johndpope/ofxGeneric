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
class ofxGenericImageManagerDelegate;

#if TARGET_OS_IPHONE
@class ofxGenericImageManagerAsyncForwarder;
#endif

class ofxGenericImageManager
{
public:
    static ofxGenericImageManager& getInstance();
    
    virtual ~ofxGenericImageManager();
    
    virtual bool load( std::string image );
    virtual bool load( std::vector< std::string > images );
    virtual bool loadAsync( std::string image, ofPtrWeak< ofxGenericImageManagerDelegate > delegate );
    virtual void unload( std::string image );
    virtual void unload( std::vector< std::string > images );
    virtual void unloadAll();
    
    void cacheImage( ofPtr< ofxGenericImage >image, std::string name = "", bool async = true);
    
    virtual bool imageIsLoaded( std::string image );
    virtual ofPtr< ofxGenericImage > getImage( std::string image );
#if TARGET_OS_IPHONE
    virtual UIImage* getUIImage( std::string image );
#endif
    
    void finishedAsyncLoading( ofPtr< ofxGenericImage > image );
    void finishedAsyncLoadingWithError( std::string error );
    
protected:
    ofxGenericImageManager();
    
    static ofPtr< ofxGenericImageManager > _this;
    virtual void init( ofPtr< ofxGenericImageManager > setThis );
    
    std::map< std::string, ofPtr< ofxGenericImage > > _images;

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
    virtual void imageManager_imageLoaded( std::string imageName, ofPtr< ofxGenericImage > image ) {};
    
    virtual ~ofxGenericImageManagerDelegate() {};
};