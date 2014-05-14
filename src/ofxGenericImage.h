//
//  ofxGenericImage.h
//  ofxGeneric
//
//  Created by Eli Delventhal on 10/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"
#include <set>

class ofxGenericImageDelegate;

class ofxGenericImage
{
public:
    
    /// Create from a resource.
    /** Construct an ofxGenericImage using the contents of the named resource. This will 
     * use a cache of loaded image resources to avoid loading the same resource twice.
     * fileName: the resource name to use as source data. Also becomes the image name.
     */
    static ofPtr< ofxGenericImage > create( const std::string& fileName );

    /// Create from a resource, with asynchronous loading.
    /** Construct an ofxGenericImage using the contents of the named resource. This will
     * use a cache of loaded image resources to avoid loading the same resource twice.
     * fileName: the resource name to use as source data. Also becomes the image name.
     * delegate: a callback to be called when loading is complete or fails.
     */
    static ofPtr< ofxGenericImage > createAsync( const std::string& fileName,
                                                 ofPtrWeak< ofxGenericImageDelegate > delegate );
    
    /// Create from ofImage.
    /** Contruct an ofxGenericImage from an ofImage object. The image is not cached.
     * image: The source image object.
     * name: An identifier string, for debugging.
     */
    static ofPtr< ofxGenericImage > create( ofPtr< ofImage > image, const std::string& name = "" );

#if TARGET_OS_IPHONE
    /// Create an image from an ofImage.
    static ofPtr< ofxGenericImage > create( UIImage* image, const std::string& fromFileName = "" );
    virtual UIImage* getUIImage();
#endif
    
    virtual bool loadedSuccessfully();
    
    virtual bool save( string fileName, bool inDocuments );
    
    virtual ~ofxGenericImage();
    
    std::string getFilePath();
    ofPoint getSize();

protected:
    ofxGenericImage();
    ofPtrWeak< ofxGenericImage > _this;

    virtual void init( ofPtrWeak< ofxGenericImage > setThis, const std::string& fileName );
    virtual void init( ofPtrWeak< ofxGenericImage > setThis, ofPtr< ofImage > image, const std::string& fromFileName );

#if TARGET_OS_IPHONE
    virtual void init( ofPtrWeak< ofxGenericImage > setThis, UIImage* image, const std::string& fromFileName );
    UIImage* _image;
#endif

    std::string _filePath;
};

/// Delegate to pass ofxGenericImage events
class ofxGenericImageDelegate
{
public:
    virtual void imageManager_imageLoaded( const std::string& imageName, ofPtr< ofxGenericImage > image ) {};
    virtual bool imageManager_imageStillNeeded( const std::string& imageName ) { return true; };
    virtual ~ofxGenericImageDelegate() {};
};


/// Helper class to manage a set of images.
/** Offers an api to load and free images by name or by  * Use only if a cache of otherwise unreferenced images is needed.
 */
class ofxGenericImageCache
{
public:
    static ofPtr< ofxGenericImageCache > create();
    
    ofPtr< ofxGenericImage > add( const std::string& imageName );
    void add( ofPtr< ofxGenericImage > image );
    
    void release( const std::string& imageName );
    void release( ofPtr< ofxGenericImage > image );
    
    void clear();
    
    virtual ~ofxGenericImageCache() {};
    
protected:
    ofPtrWeak< ofxGenericImageCache > _this;
    ofxGenericImageCache() {};
    void init(ofPtr< ofxGenericImageCache > instance);
    
    std::set< ofPtr< ofxGenericImage > > _images;
};

