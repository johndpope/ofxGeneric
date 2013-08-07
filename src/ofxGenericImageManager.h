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
class ofxGenericImageDelegate;

#if TARGET_OS_IPHONE
@class ofxGenericImageManagerAsyncForwarder;
#endif

class ofxGenericImageManager
{
public:
    static ofxGenericImageManager& getInstance();
    virtual ~ofxGenericImageManager();

    static std::string getNativeImagePath( std::string fileName, bool makeAbsolute = true );

    /// Add an image to the image manager's cache.
    /** Returns the image previously assigned to the given name.
     * name: the key for the image.
     * image: the image to cache.
     */
    ofPtr< ofxGenericImage > add( const std::string& name, ofPtr< ofxGenericImage > image );
    virtual ofPtr< ofxGenericImage > getImage( const std::string& name );

    virtual ofPtr< ofxGenericImage > loadAsync( const std::string& name, ofPtrWeak< ofxGenericImageDelegate > delegate );
    
#if TARGET_OS_IPHONE
    virtual UIImage* getUIImage( const std::string& image );
    void finishedAsyncLoading( UIImage* image, const std::string& url );
    void finishedAsyncLoadingWithError( const std::string& error );
#endif
    
    
protected:
    ofxGenericImageManager();
    
    static ofPtr< ofxGenericImageManager > _this;
    virtual void init( ofPtr< ofxGenericImageManager > setThis );
    
    typedef std::map< std::string, ofPtrWeak< ofxGenericImage > > ImageMap;
    ImageMap _images;

    std::list< std::pair< std::string, ofPtrWeak< ofxGenericImageDelegate > > > _asyncQueuedImages;
    std::pair< std::string, ofPtrWeak< ofxGenericImageDelegate > > _currentlyLoading;
    void continueAsyncLoading();

    virtual bool imageIsLoaded( const std::string& image );

#if TARGET_OS_IPHONE
    ofxGenericImageManagerAsyncForwarder* _asyncLoadingForwarder;
#endif
};

