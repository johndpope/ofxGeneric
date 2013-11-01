//
//  ofxGenericImageManager.cpp
//  ofxGeneric
//
//  Created by Eli Delventhal on 10/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericImageManager.h"
#include "ofxGenericImage.h"

#include "ofxGenericPlatform.h"


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
}

ofxGenericImageManager::~ofxGenericImageManager()
{
    
}

std::string ofxGenericImageManager::getNativeImagePath( std::string fileName, bool makeAbsolute )
{
#if TARGET_OS_IPHONE
    if ( ofxGenericPlatform::is4InchDisplay() )
    {
        if ( ofxGenericPlatform::isRetinaDisplay() )
        {
            string test = ofxGenericPlatform::imageFileName( fileName, true, false, true );
            if ( ofxGFileExists( test, false ) )
            {
                if ( makeAbsolute )
                {
                    test = ofToPath( test, false );
                }
                return test;
            }
        }
        
        string test = ofxGenericPlatform::imageFileName( fileName, true, false, false );
        if ( ofxGFileExists( test, false ) )
        {
            if ( makeAbsolute )
            {
                test = ofToPath( test, false );
            }
            return test;
        }
    } else if ( ofxGenericPlatform::isTablet() )
    {
        if ( ofxGenericPlatform::isRetinaDisplay() )
        {
            string test = ofxGenericPlatform::imageFileName( fileName, false, true, true );
            if ( ofxGFileExists( test, false ) )
            {
                if ( makeAbsolute )
                {
                    test = ofToPath( test, false );
                }
                return test;
            }
        }
        
        string test = ofxGenericPlatform::imageFileName( fileName, false, true, false );
        if ( ofxGFileExists( test, false ) )
        {
            if ( makeAbsolute )
            {
                test = ofToPath( test, false );
            }
            return test;
        }
    }
    
    if ( ofxGenericPlatform::isRetinaDisplay() )
    {
        string test = ofxGenericPlatform::imageFileName( fileName, false, false, true );
        if ( ofxGFileExists( test, false ) )
        {
            if ( makeAbsolute )
            {
                test = ofToPath( test, false );
            }
            return test;
        }
    }
    
#elif TARGET_ANDROID
#endif
    if ( makeAbsolute )
    {
        fileName = ofToPath( fileName, false );
    }
    return fileName;
}

ofPtr< ofxGenericImage > ofxGenericImageManager::add( const std::string& name, ofPtr< ofxGenericImage > image )
{
    ofPtr< ofxGenericImage > oldImage;
    ImageMap::iterator itr = _images.find( name );
    if( itr != _images.end() )
    {
        oldImage = itr->second.lock();
        itr->second = image;
    }
    else
    {
        _images.insert(make_pair(name, image));
    }
    return oldImage;
}

ofPtr< ofxGenericImage > ofxGenericImageManager::loadAsync( const std::string& image, ofPtrWeak< ofxGenericImageDelegate > delegate )
{
    ofPtr< ofxGenericImage > imagePtr = _images[ image ].lock();
    if( !imagePtr )
    {
        _asyncQueuedImages.push_back( std::pair< std::string, ofPtrWeak< ofxGenericImageDelegate > >( image, delegate ) );
        continueAsyncLoading();
    }
    return imagePtr;
}

void ofxGenericImageManager::continueAsyncLoading()
{
    // process the async image queue
    while( _currentlyLoading.first.empty() && !_asyncQueuedImages.empty() )
    {
        _currentlyLoading = _asyncQueuedImages.front() ;
        _asyncQueuedImages.pop_front();
        ofPtr< ofxGenericImageDelegate > delegate = _currentlyLoading.second.lock();

        if( imageIsLoaded( _currentlyLoading.first ) )
        {
            // The image is already loaded. Tell the delegate and move on.
            if ( delegate )
            {
                delegate->imageManager_imageLoaded( _currentlyLoading.first, getImage( _currentlyLoading.first ) );
            }
            
            // Clear currentlyLoading for another round.
            _currentlyLoading = std::pair< std::string, ofPtrWeak< ofxGenericImageDelegate > >();
        }
        else
        {
            // Image is not loaded.
            if( delegate && !(delegate->imageManager_imageStillNeeded( _currentlyLoading.first )) )
            {
                // There is a delegate and it does not need the image. Clear the current and continue.
                _currentlyLoading = std::pair< std::string, ofPtrWeak< ofxGenericImageDelegate > >();
            }
            else
            {
                // The was no delegate, or the delgate still awaits this image.
                // Launch an async load.
#if TARGET_OS_IPHONE
                _asyncLoadingForwarder = [ [ ofxGenericImageManagerAsyncForwarder alloc ] initWithForwardTo:_this andURL: _currentlyLoading.first ];
#endif
            }
        }
    }
}

void ofxGenericImageManager::finishedAsyncLoading( UIImage* uiImage, const std::string& url )
{
    ofPtr< ofxGenericImageDelegate > delegate = _currentlyLoading.second.lock();
    if( imageIsLoaded(_currentlyLoading.first) )
    {
        // The image is loaded already! Tell the delegate.
        if ( delegate )
        {
            delegate->imageManager_imageLoaded( _currentlyLoading.first, getImage(_currentlyLoading.first) );
        }
    }
    else
    {
        // Image is not loaded, use the data from the async load.
        ofPtr< ofxGenericImage > image = ofxGenericImage::create( uiImage, url );
        if ( image )
        {
            // Add the image to the image manager.
            _images[ _currentlyLoading.first ] = image;
            
            // Inform the delegate.
            if ( delegate )
            {
                delegate->imageManager_imageLoaded( _currentlyLoading.first, image );
            }
        }
        else
        {
            ofxGLogError( "Error creating " + _currentlyLoading.first );
        }
    }

    // Release the async forwarder.
    if ( _asyncLoadingForwarder )
    {
        [ _asyncLoadingForwarder release ];
        _asyncLoadingForwarder = nil;
    }

    // Clear the current.
    _currentlyLoading = std::pair< std::string, ofPtrWeak< ofxGenericImageDelegate > >();
    
    // Process the queue.
    continueAsyncLoading();
}

void ofxGenericImageManager::finishedAsyncLoadingWithError( const string& error )
{
    ofxGLogError( "Error loading " + _currentlyLoading.first );
    finishedAsyncLoading( NULL, "" );
}

bool ofxGenericImageManager::imageIsLoaded( const std::string& image )
{
    bool result = true;
    ImageMap::iterator end = _images.end();
    ImageMap::iterator itr = _images.find(image);
    
    if( itr == end || itr->second.expired() )
    {
        result = false;
    }

    return result;
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
    _url = ofxGenericImageManager::getNativeImagePath( url, false );
    
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
    CGFloat scale = 1.0f;
    if (ofxGenericPlatform::isRetinaDisplay())
    {
        scale = 2.0f;
    }
    UIImage* uiImage = [UIImage imageWithData:_data scale:scale];
      
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

#endif