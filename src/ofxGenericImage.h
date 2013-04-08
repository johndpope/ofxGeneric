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
    static ofPtr< ofxGenericImage > create( std::string fileName );
#if TARGET_OS_IPHONE
    static ofPtr< ofxGenericImage > create( UIImage* image );
#endif
    static ofPtr< ofxGenericImage > create( ofPtr< ofImage > image );
    static std::string getNativeImagePath( std::string fileName );

#if TARGET_OS_IPHONE
    virtual UIImage* getUIImage();
#endif
    virtual bool loadedSuccessfully();
    
    virtual ~ofxGenericImage();
    
protected:
    ofxGenericImage();
    ofPtrWeak< ofxGenericImage > _this;
    virtual void init( ofPtrWeak< ofxGenericImage > setThis, std::string fileName );
#if TARGET_OS_IPHONE
    virtual void init( ofPtrWeak< ofxGenericImage > setThis, UIImage* image );
#endif
    virtual void init( ofPtrWeak< ofxGenericImage > setThis, ofPtr< ofImage > image );
    
#if TARGET_OS_IPHONE
    UIImage* _image;
#endif

    std::string _filePath;
};