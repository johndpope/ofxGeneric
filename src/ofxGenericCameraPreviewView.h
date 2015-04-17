//
//  ofxGenericCameraPreviewView.h
//  ofxGeneric
//
//  Created by Ian Grossberg on 5/26/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericView.h"

#if TARGET_OS_IPHONE
#import <AVFoundation/AVFoundation.h>
#endif

#include "ofxGenericApp.h"

class ofxGenericCameraPreviewViewDelegate;
class ofxGenericImage;

class ofxGenericCameraPreviewView : public ofxGenericView
{
public:
    static ofPtr< ofxGenericCameraPreviewView > create( ofPtrWeak< ofxGenericCameraPreviewViewDelegate > delegate, const ofRectangle& setFrame = ofRectangle( 0, 0, 0, 0 ) );
    virtual ~ofxGenericCameraPreviewView();
    
    virtual void didLoad();
    
    virtual void willAppear();
    virtual void didDisappear();
    
    virtual void takePicture();
    virtual void pictureTaken( ofPtr< ofxGenericImage > image );
    // IOSP-40 // virtual void pictureTaken( ofPtr< ofImage > image );
    
    virtual void deviceOrientationChanged( ofxGenericOrientationEventArgs& orientationArgs );
    
protected:
    ofxGenericCameraPreviewView();
    virtual void init( ofPtrWeak< ofxGenericCameraPreviewView > setThis, ofPtrWeak< ofxGenericCameraPreviewViewDelegate > delegate, const ofRectangle& setBounds );
    
    ofPtrWeak< ofxGenericCameraPreviewViewDelegate > _delegate;
    
#if TARGET_OS_IPHONE
    AVCaptureSession*   _captureSession;
    AVCaptureDeviceInput*     _captureInput;
    AVCaptureVideoDataOutput* _captureVideoDataOutput;
    AVCaptureStillImageOutput* _captureStillImageOutput;
    AVCaptureDevice*    _captureDevice;
    AVCaptureVideoPreviewLayer* _captureLayer;
#endif    
};

class ofxGenericCameraPreviewViewDelegate
{
public:
    virtual ~ofxGenericCameraPreviewViewDelegate() {};
    
    // IOSP-40 // virtual void pictureTaken( ofPtr< ofImage > image ) {};
    virtual void pictureTaken( ofPtr< ofxGenericImage > image ) {};
};