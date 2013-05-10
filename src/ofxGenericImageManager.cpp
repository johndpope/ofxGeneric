//
//  ofxGenericImageManager.cpp
//  ofxGeneric
//
//  Created by Eli Delventhal on 10/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericImageManager.h"
#include "ofxGenericImage.h"

#if TARGET_OS_IPHONE
@interface ofxGenericImageManagerAsyncForwarder : NSObject
{
@private
    ofPtrWeak< ofxGenericImageManager > _forwardTo;
    
    NSURLConnection* _connection;
    NSMutableData* _data;
}
-( id )initWithForwardTo:( ofPtrWeak< ofxGenericImageManager > )forwardTo andURL:( std::string )url;
-( void )connection:( NSURLConnection* )connection didReceiveData:( NSData* )incrementalData;
-( void )connectionDidFinishLoading:( NSURLConnection* )connection;
@end
#endif

ofPtr< ofxGenericImageManager > ofxGenericImageManager::_this;

ofxGenericImageManager& ofxGenericImageManager::getInstance()
{
    if ( !ofxGenericImageManager::_this )
    {
        ofPtr< ofxGenericImageManager > instance = ofPtr< ofxGenericImageManager >( new ofxGenericImageManager() );
        instance->init( instance );
    }
    return *ofxGenericImageManager::_this;
}

ofxGenericImageManager::ofxGenericImageManager()
{
    
}

void ofxGenericImageManager::init( ofPtr< ofxGenericImageManager > setThis )
{
    ofxGenericImageManager::_this = setThis;
    
    _images = std::map< std::string, ofPtr< ofxGenericImage > >();
    _currentlyLoading = std::pair< std::string, ofPtrWeak< ofxGenericImageManagerDelegate > >();
}

ofxGenericImageManager::~ofxGenericImageManager()
{
    
}

bool ofxGenericImageManager::load( std::string image )
{
    if ( !_images[ image ] )
    {
        ofPtr< ofxGenericImage > loadedImage = ofxGenericImage::create( image );
        if ( !loadedImage->loadedSuccessfully() )
        {
            return false;
        }
        _images[ image ] = loadedImage;
    }
    return true;
}

bool ofxGenericImageManager::load( std::vector< std::string > images )
{
    bool success = true;
    for ( unsigned int i = 0; i < images.size(); i++ )
    {
        success = load( images[i] ) && success;
    }
    return success;
}

bool ofxGenericImageManager::loadAsync( std::string image, ofPtrWeak< ofxGenericImageManagerDelegate > delegate )
{
    _asyncQueuedImages.push_back( std::pair< std::string, ofPtrWeak< ofxGenericImageManagerDelegate > >( image, delegate ) );
    continueAsyncLoading();
    return true;
}

void ofxGenericImageManager::continueAsyncLoading()
{
    if ( _currentlyLoading.first.empty() )
    {
        if ( _asyncQueuedImages.size() > 0 )
        {
            _currentlyLoading = ( *_asyncQueuedImages.begin() );
            _asyncQueuedImages.pop_front();
            
            while( imageIsLoaded( _currentlyLoading.first ) )
            {
                std::string imageName = _currentlyLoading.first;
                ofPtr< ofxGenericImageManagerDelegate > delegate = _currentlyLoading.second.lock();
                if ( delegate )
                {
                    delegate->imageManager_imageLoaded( imageName, getImage( imageName ) );
                }
                
                if ( _asyncQueuedImages.size() == 0 )
                {
                    _currentlyLoading = std::pair< std::string, ofPtrWeak< ofxGenericImageManagerDelegate > >();
                    break;
                }
                
                _currentlyLoading = ( *_asyncQueuedImages.begin() );
                _asyncQueuedImages.pop_front();
            }
            
    #if TARGET_OS_IPHONE
            _asyncLoadingForwarder = [ [ ofxGenericImageManagerAsyncForwarder alloc ] initWithForwardTo:_this andURL: _currentlyLoading.first ];
    #endif
        }
    }
}

void ofxGenericImageManager::finishedAsyncLoading( ofPtr< ofxGenericImage > image )
{
    if ( image )
    {
        _images[ _currentlyLoading.first ] = image;
    } else
    {
        ofxGLogError( "Error loading " + _currentlyLoading.first );
    }

    ofPtr< ofxGenericImageManagerDelegate > delegate = _currentlyLoading.second.lock();
    if ( delegate )
    {
        delegate->imageManager_imageLoaded( _currentlyLoading.first, image );
    }
    
    _currentlyLoading = std::pair< std::string, ofPtrWeak< ofxGenericImageManagerDelegate > >();
    
    if ( _asyncLoadingForwarder )
    {
        [ _asyncLoadingForwarder release ];
        _asyncLoadingForwarder = nil;
    }
    
    continueAsyncLoading();
}

void ofxGenericImageManager::finishedAsyncLoadingWithError( string error )
{
    ofxGLogError( "Error loading " + _currentlyLoading.first );
    finishedAsyncLoading( ofPtr< ofxGenericImage >() );
}

void ofxGenericImageManager::unload( std::string image )
{
    _images[ image ] = ofPtr< ofxGenericImage >();
}

void ofxGenericImageManager::unload( std::vector< std::string > images )
{
    for ( unsigned int i = 0; i < images.size(); i++ )
    {
        unload( images[i] );
    }
}

void ofxGenericImageManager::unloadAll()
{
    _images = std::map< std::string, ofPtr< ofxGenericImage > >();
}

bool ofxGenericImageManager::imageIsLoaded( std::string image )
{
    if ( _images[ image ] )
    {
        return true;
    }
    return false;
}

ofPtr< ofxGenericImage > ofxGenericImageManager::getImage( std::string image )
{
    return _images[ image ];
}

#if TARGET_OS_IPHONE
UIImage* ofxGenericImageManager::getUIImage( std::string image )
{
    if ( _images[ image ] )
    {
        return _images[ image ]->getUIImage();
    }
    return nil;
}
#endif

#if TARGET_OS_IPHONE

@implementation ofxGenericImageManagerAsyncForwarder

-( id )initWithForwardTo:( ofPtrWeak< ofxGenericImageManager > ) forwardTo andURL:( std::string )url
{
    self = [ super init ];
    if ( self )
    {
        _forwardTo = forwardTo;
        [ self start:url ];
    }
    return self;
}

-( void )start:( std::string )url
{
    NSString* fileNameMinusExtension = ofxStringToNSString( ofFilePath::removeExt( url ) );
    
    NSURL* nsURL = [ [ NSBundle mainBundle ] URLForResource:fileNameMinusExtension withExtension:@"png" ];//[ NSURL URLWithString:ofxStringToNSString( "file://" + url ) ];
    
    NSURLRequest* request = [ NSURLRequest requestWithURL:nsURL
                                              cachePolicy:NSURLRequestReturnCacheDataElseLoad
                                          timeoutInterval:30.0
                             ];
    
    _connection = [ [ NSURLConnection alloc ] initWithRequest:request delegate:self ];
}

-( void )connection:( NSURLConnection* )connection didReceiveData:( NSData* )incrementalData
{
    if ( _data == nil )
    {
        _data = [ [ NSMutableData alloc ] initWithCapacity:2048 ];
    }
    [ _data appendData:incrementalData ];
}

-( void )connectionDidFinishLoading:( NSURLConnection* )connection
{
    UIImage* uiImage = [ UIImage imageWithData:_data ];
    
    ofPtr< ofxGenericImage > image = ofxGenericImage::create( uiImage );
    
    [ _data release ];
    _data = nil;
    
    [ _connection release ];
    _connection = nil;
    
    ofPtr< ofxGenericImageManager > forwardTo = _forwardTo.lock();
    if ( forwardTo )
    {
        forwardTo->finishedAsyncLoading( image );
    }
}

- (void)connection:( NSURLConnection* )connection didFailWithError:( NSError* )error
{
    [ _data release ];
    _data = nil;
    
    [ _connection release ];
    _connection = nil;

    ofPtr< ofxGenericImageManager > forwardTo = _forwardTo.lock();
    if ( forwardTo )
    {
        forwardTo->finishedAsyncLoadingWithError( ofxNSStringToString( [ error description ] ) );
    }
}

@end
#endif