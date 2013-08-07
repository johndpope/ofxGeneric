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
    static ofPtr< ofxGenericImage > create( ofPtr< ofImage > image, const std::string& fromFileName = "" );
    
    virtual bool loadedSuccessfully();
    
    virtual bool save( string fileName, bool inDocuments );
    
    virtual ~ofxGenericImage();
    
    std::string getFilePath();

protected:
    ofxGenericImage();
    ofPtrWeak< ofxGenericImage > _this;

    virtual void init( ofPtrWeak< ofxGenericImage > setThis, ofPtr< ofImage > image, const std::string& fromFileName );

    std::string _filePath;

#if TARGET_OS_IPHONE
public:
    static ofPtr< ofxGenericImage > create( UIImage* image, const std::string& fromFileName = "" );
    virtual UIImage* getUIImage();
    
protected:
    virtual void init( ofPtrWeak< ofxGenericImage > setThis, UIImage* image, const std::string& fromFileName );
    UIImage* _image;
#endif
};