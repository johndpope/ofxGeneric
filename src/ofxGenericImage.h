//
//  ofxGenericImage.h
//  ofxGeneric
//
//  Created by Eli Delventhal on 10/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"

class ofxGenericImage
{
public:
    static std::string getNativeImagePath( std::string fileName, bool makeAbsolute = true );

#if TARGET_OS_IPHONE
    virtual UIImage* getUIImage();
#endif
    virtual bool loadedSuccessfully();
    
    virtual bool save( string fileName, bool inDocuments );
    
    virtual ~ofxGenericImage();
    
    std::string getFilePath();


protected:
    //*REMOVE:mpalange these extras when done!!
    friend class ofxGenericCameraPreviewView;
    friend class TrainingWorkoutConclusionAnimationView;
    
    friend class ofxGenericImageManager;
    static ofPtr< ofxGenericImage > create( const std::string& fileName );
    static ofPtr< ofxGenericImage > create( ofPtr< ofImage > image, const std::string& fromFileName = "" );
#if TARGET_OS_IPHONE
    static ofPtr< ofxGenericImage > create( UIImage* image, const std::string& fromFileName = "" );
#endif
    
    

    ofxGenericImage();
    ofPtrWeak< ofxGenericImage > _this;
    virtual void init( ofPtrWeak< ofxGenericImage > setThis, const std::string& fileName );
#if TARGET_OS_IPHONE
    virtual void init( ofPtrWeak< ofxGenericImage > setThis, UIImage* image, const std::string& fromFileName );
#endif
    virtual void init( ofPtrWeak< ofxGenericImage > setThis, ofPtr< ofImage > image, const std::string& fromFileName );
    
#if TARGET_OS_IPHONE
    UIImage* _image;
#endif

    std::string _filePath;
};