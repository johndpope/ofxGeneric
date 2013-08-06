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
    
    std::string _url;
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
    _currentlyLoading = std::pair< std::string, ofPtrWeak< ofxGenericImageManagerDelegate > >();
}

ofxGenericImageManager::~ofxGenericImageManager()
{
    
}

 ofPtr< ofxGenericImage > ofxGenericImageManager::load( const std::string& image )
{
    ofPtr< ofxGenericImage > result = getImage( image );
    if ( !result )
    {
        ofPtr< ofxGenericImage > loadedImage = ofxGenericImage::create( image );
        if ( loadedImage->loadedSuccessfully() )
        {
            _images[ image ] = loadedImage;
            result = loadedImage;
        }
    }
    return result;
}

vector< ofPtr< ofxGenericImage > > ofxGenericImageManager::load( vector< std::string > images )
{
    // Initialize with an empty pointer for each image name passed in.
    vector< ofPtr< ofxGenericImage > > results(images.size(), ofPtr< ofxGenericImage >());
    for ( unsigned int i = 0; i < images.size(); i++ )
    {
        results[i] = load( images[i] );
    }
    return results;
}

ofPtr< ofxGenericImage > ofxGenericImageManager::loadAsync( const std::string& image, ofPtrWeak< ofxGenericImageManagerDelegate > delegate )
{
    ofPtr< ofxGenericImage > imagePtr = _images[ image ].lock();
    if( !imagePtr )
    {
        _asyncQueuedImages.push_back( std::pair< std::string, ofPtrWeak< ofxGenericImageManagerDelegate > >( image, delegate ) );
        continueAsyncLoading();
    }
    return imagePtr;
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

void ofxGenericImageManager::finishedAsyncLoading( UIImage* uiImage, const std::string& url )
{
    ofPtr< ofxGenericImage > image = ofxGenericImage::create( uiImage, url );
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

void ofxGenericImageManager::finishedAsyncLoadingWithError( const string& error )
{
    ofxGLogError( "Error loading " + _currentlyLoading.first );
    finishedAsyncLoading( NULL, "" );
}

bool ofxGenericImageManager::imageIsLoaded( const std::string& image )
{
    if ( _images[ image ] )
    {
        return true;
    }
    return false;
}

ofPtr< ofxGenericImage > ofxGenericImageManager::getImage( const std::string& image )
{
    return _images[ image ].lock();
}

#if TARGET_OS_IPHONE
UIImage* ofxGenericImageManager::getUIImage( const std::string& image )
{
    if ( _images[ image ] )
    {
        return _images[ image ].lock()->getUIImage();
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
    _url = ofxGenericImage::getNativeImagePath( url, false );
    
    string extension = ofFilePath::getFileExt( _url );
    
    NSString* fileNameMinusExtension = ofxStringToNSString( ofFilePath::removeExt( _url ) );
    
    NSURL* nsURL = [ [ NSBundle mainBundle ] URLForResource:fileNameMinusExtension withExtension:ofxStringToNSString( extension ) ];//[ NSURL URLWithString:ofxStringToNSString( "file://" + url ) ];
    
    if ( !nsURL ) // TODO: properly recover from flow on error instead of continueing through and using connection's error
    {
        ofxGLogError( "Unable to retrieve URLForResource \"" + ofxNSStringToString( fileNameMinusExtension ) + "\" with extension \"" + extension + "\" from Main Bundle" );
    }
    
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
      
    [ _data release ];
    _data = nil;
    
    [ _connection release ];
    _connection = nil;
    
    ofPtr< ofxGenericImageManager > forwardTo = _forwardTo.lock();
    if ( forwardTo )
    {
        forwardTo->finishedAsyncLoading( uiImage, _url );
    }
}

- (void)connection:( NSURLConnection* )connection didFailWithError:( NSError* )error
{
    if ( error )
    {
        ofxGLogError( ofxNSStringToString( [ NSString stringWithFormat:@"Error loading URL %@: %@", [ [ _connection currentRequest] URL], error ] ) );
    }
    
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


ofPtr< ofxGenericImageCache > ofxGenericImageCache::create()
{
    ofPtr< ofxGenericImageCache > instance(new ofxGenericImageCache);
    instance->init(instance);
    return instance;
}

void ofxGenericImageCache::init(ofPtr< ofxGenericImageCache > instance)
{
    _this = instance;
}

ofPtr< ofxGenericImage > ofxGenericImageCache::add( const std::string& imageName )
{
    ofPtr< ofxGenericImage > image = ofxGenericImageManager::getInstance().load( imageName );
    if( image )
    {
        add( image );
    }
    return image;
}

void ofxGenericImageCache::add( ofPtr< ofxGenericImage > image )
{
    _images.insert(image);
}

void ofxGenericImageCache::release( const std::string& imageName )
{
    ofPtr< ofxGenericImage > image = ofxGenericImageManager::getInstance().getImage( imageName );
    release( image );
    
}

void ofxGenericImageCache::release( ofPtr< ofxGenericImage > image )
{
    _images.erase(image);
}

void ofxGenericImageCache::clear()
{
    _images.clear();
}


#endif