//
//  ofxGenericCameraPreviewView.cpp
//  ofxGeneric
//
//  Created by Ian Grossberg on 5/26/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericCameraPreviewView.h"

#include "ofxiPhoneExtras.h"

#include "ofxGenericImage.h"

ofPtr< ofxGenericCameraPreviewView > ofxGenericCameraPreviewView::create( ofPtrWeak< ofxGenericCameraPreviewViewDelegate > delegate, const ofRectangle& setFrame )
{
    ofPtr< ofxGenericCameraPreviewView > create( new ofxGenericCameraPreviewView() );
    create->init( create, delegate, setFrame );
    return create;
}

ofxGenericCameraPreviewView::~ofxGenericCameraPreviewView()
{
#if TARGET_OS_IPHONE    
    [ _captureSession release ];
    [ _captureInput release ];
    [ _captureVideoDataOutput release ];
    [ _captureStillImageOutput release ];
    [ _captureDevice release ];
    [ _captureLayer release ];
#endif
}

ofxGenericCameraPreviewView::ofxGenericCameraPreviewView()
#if TARGET_OS_IPHONE
: _captureSession( nil ), _captureInput( nil ), _captureVideoDataOutput( nil ), _captureStillImageOutput( nil ), _captureDevice( nil ), _captureLayer( nil )
#endif
{
}

void ofxGenericCameraPreviewView::init( ofPtrWeak< ofxGenericCameraPreviewView > setThis, ofPtrWeak< ofxGenericCameraPreviewViewDelegate > delegate, const ofRectangle& setBounds )
{
    ofxGenericView::init( setThis, setBounds );
    _delegate = delegate;
}

void ofxGenericCameraPreviewView::didLoad()
{
    ofxGenericView::didLoad();
    setBackgroundColor( ofColor( 0, 0, 0 ) );
#if TARGET_OS_IPHONE
    _captureSession = [ [ AVCaptureSession alloc ] init ];
    if ( _captureSession )
    {
        _captureDevice = [ AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo ];
        
        if ( _captureDevice )
        {
            NSError* error = nil;
            _captureInput = [ AVCaptureDeviceInput deviceInputWithDevice:_captureDevice error:&error ];
            if ( !_captureInput ) 
            {
                // Handle the error appropriately.
            }
            _captureStillImageOutput = [ [ AVCaptureStillImageOutput alloc ] init ];
            _captureVideoDataOutput = [ [ AVCaptureVideoDataOutput alloc ] init ];
            
            if ( [ _captureSession canAddInput:_captureInput ] && [ _captureSession canAddOutput:_captureStillImageOutput ] && [ _captureSession canAddOutput:_captureVideoDataOutput ] )
            {
                [ _captureSession beginConfiguration ];
                [ _captureSession addInput:_captureInput ];
                [ _captureSession addOutput:_captureStillImageOutput ];
                [ _captureSession addOutput:_captureVideoDataOutput ];
                [ _captureSession setSessionPreset:AVCaptureSessionPresetHigh ];
                [ _captureSession commitConfiguration ];
                
                _captureLayer = [ AVCaptureVideoPreviewLayer layerWithSession:_captureSession ];
                _captureLayer.bounds = _view.layer.bounds;
                _captureLayer.videoGravity = AVLayerVideoGravityResizeAspectFill;
                _captureLayer.position = CGPointMake( CGRectGetMidX( _view.layer.bounds ), CGRectGetMidY( _view.layer.bounds ) );
                
                [ _view.layer addSublayer:_captureLayer ];
            }
        }
    }
#endif
}

void ofxGenericCameraPreviewView::willAppear()
{
    ofxGenericView::willAppear();
#if TARGET_OS_IPHONE
    [ _captureSession startRunning ];
#endif
    
    ofxGRegisterOrientationEvents( this );
}

void ofxGenericCameraPreviewView::didDisappear()
{
    ofxGenericView::didDisappear();
#if TARGET_OS_IPHONE
    [ _captureSession stopRunning ];
#endif    
    
    ofxGUnregisterOrientationEvents( this );
}

void ofxGenericCameraPreviewView::deviceOrientationChanged( ofxGenericOrientationEventArgs& orientationArgs )
{
#if TARGET_OS_IPHONE
    AVCaptureVideoOrientation avOrientation;
    switch ( orientationArgs.orientation ) 
    {
        case OF_ORIENTATION_DEFAULT:
            avOrientation = AVCaptureVideoOrientationPortrait;
            break;
        case OF_ORIENTATION_180:
            avOrientation = AVCaptureVideoOrientationPortraitUpsideDown;
            break;
        case OF_ORIENTATION_90_LEFT:
            avOrientation = AVCaptureVideoOrientationLandscapeLeft;
            break;
        case OF_ORIENTATION_90_RIGHT:
            avOrientation = AVCaptureVideoOrientationLandscapeRight;
            break;
        default:
            avOrientation = AVCaptureVideoOrientationPortrait;
            break;
    }
//    [ [ _captureLayer connection ] setVideoOrientation:avOrientation ];
    _captureLayer.bounds = _view.layer.bounds;
    _captureLayer.position = CGPointMake( CGRectGetMidX( _view.layer.bounds ), CGRectGetMidY( _view.layer.bounds ) );

#endif
}

void ofxGenericCameraPreviewView::takePicture()
{
#if TARGET_OS_IPHONE
    
#if TARGET_IPHONE_SIMULATOR
    UIImage* uiImage = [ UIImage imageNamed:@"Default.png" ];
    pictureTaken( ofxGenericImage::create( uiImage ) );

    ofPtr< ofImage > ofImage;
    ofxiPhoneUIImageToOFImage( uiImage, *ofImage, ( int )getFrame().width, ( int )getFrame().height );
    pictureTaken( ofImage );
    
#else

    AVCaptureConnection* videoConnection = nil;
    for ( AVCaptureConnection* connection in _captureStillImageOutput.connections ) 
    {
        for (AVCaptureInputPort *port in [ connection inputPorts ] ) 
        {
            if ( [ [ port mediaType ] isEqual:AVMediaTypeVideo ] ) 
            {
                videoConnection = connection;
                break;
            }
        }
        if ( videoConnection ) 
        { break; }
    }
    
    if ( videoConnection )
    {
        [ _captureStillImageOutput
         captureStillImageAsynchronouslyFromConnection:videoConnection 
         completionHandler:^( CMSampleBufferRef imageDataSampleBuffer, NSError *error )         
         {
             if ( imageDataSampleBuffer != NULL )
             {
                 NSData* imageData = [AVCaptureStillImageOutput jpegStillImageNSDataRepresentation:imageDataSampleBuffer];
                 UIImage* image = [ [ UIImage alloc ] initWithData:imageData ];
                 
                 pictureTaken( ofxGenericImage::create( image ) );

                 ofPtr< ofImage > convertedImage( new ofImage() );
                 ofxiPhoneUIImageToOFImage( image, *convertedImage );
                 pictureTaken( convertedImage );
             }
         }
         ];
    }

#endif
    
#endif
}

void ofxGenericCameraPreviewView::pictureTaken( ofPtr< ofxGenericImage > image )
{
    if ( _delegate )
    {
        _delegate.lock()->pictureTaken( image );
    }
}

void ofxGenericCameraPreviewView::pictureTaken( ofPtr< ofImage > image )
{
    if ( _delegate )
    {
        _delegate.lock()->pictureTaken( image );
    }
}
