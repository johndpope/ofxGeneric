//
//  ofxGenericImageView.cpp
//
//  Created by Ian Grossberg on 1/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericImageView.h"
#include "ofxGenericUtility.h"
#include "ofxGenericImage.h"
#include "ofxGenericImageManager.h"

#if TARGET_OS_IPHONE
#include "UIDevice-Hardware.h"
#include "ofxiPhoneExtras.h"

#elif TARGET_ANDROID
#include "JNIUtility.h"

jclass ofxGenericImageView::_jniClass = NULL;
const char* ofxGenericImageView::className = "cc/openframeworks/ofxGeneric/ImageView";
#endif

ofxGenericImageView::ofxGenericImageView()
{
}

ofPtr< ofxGenericImageView > ofxGenericImageView::create( const ofRectangle& setFrame, string fileName )
{
    ofPtr< ofxGenericImageView > create = ofPtr< ofxGenericImageView >( new ofxGenericImageView() );
    create->init( create, setFrame, fileName );
    return create;
}

void ofxGenericImageView::init( ofPtrWeak< ofxGenericImageView > setThis, const ofRectangle& setFrame, string fileName )
{
    ofxGenericView::init( setThis, setFrame );
    if ( !fileName.empty() )
    {
        setImage( fileName );
    }
}

NativeView ofxGenericImageView::createNativeView( const ofRectangle& frame )
{
#if TARGET_OS_IPHONE
    UIImageView* newView = [ [ UIImageView alloc ] initWithFrame:ofxRectangleToCGRect( frame ) ];
    [ newView setBackgroundColor:[ UIColor clearColor ] ];
    return newView;
#elif TARGET_ANDROID
    return ofxGenericView::createNativeView( frame );
#endif
}

void ofxGenericImageView::setImage( std::string fileName )
{
    if ( !ofxGenericImageManager::getInstance().imageIsLoaded( fileName ) )
    {
        _waitingOnAsyncLoadImageName = fileName;
        ofxGenericImageManager::getInstance().loadAsync( fileName, dynamic_pointer_cast< ofxGenericImageManagerDelegate >( _this ) );
    }
    setImage( ofxGenericImageManager::getInstance().getImage( fileName ) );
}

void ofxGenericImageView::setImage ( ofPtr< ofxGenericImage > image )
{
    if ( !image )
    {
        return;
    }
    
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UIImageView class ] ] )
    {
        UIImageView* view = ( UIImageView* )_view;
        
        [ view setImage: image->getUIImage() ];
    }
#elif TARGET_ANDROID
#endif
}

void ofxGenericImageView::willAppear()
{
    ofxGenericView::willAppear();
    
    if ( !_waitingOnAsyncLoadImageName.empty() )
    {
        ofxGenericImageManager::getInstance().load( _waitingOnAsyncLoadImageName );
        setImage( ofxGenericImageManager::getInstance().getImage( _waitingOnAsyncLoadImageName ) );
        _waitingOnAsyncLoadImageName = std::string();
    }
}

#if TARGET_OS_IPHONE

ofxGenericUIViewCastOperator( ofxGenericImageView, UIImageView );

#elif TARGET_ANDROID

jclass ofxGenericImageView::getJNIClassStatic()
{
	// TODO: handle exception
    if ( !_jniClass )
    	_jniClass = ( jclass )ofxGenericImageView::createJNIReferenceStatic( JNIFindClass( ofxGenericImageView::className ) );
    return _jniClass;
}

jclass ofxGenericImageView::getJNIClass()
{
	return ofxGenericImageView::getJNIClassStatic();
}

void ofxGenericImageView::registerJNIMethods()
{
	ofxGenericView::registerJNIMethods();
	registerJNIMethodID(
			_jniMethods,
			false,
			JNIMethod_SetImage,
			"setImage",
			JNIEncodeMethodSignature( 0, JNIType_void )
			);
}


#endif

string ofxGenericImageView::toString()
{
    string result = ofxGenericView::toString();
    
    if ( _image && !_image->getFilePath().empty() )
    {
        result += " " + ofFilePath::getFileName( _image->getFilePath() );
    }
    return result;
}

ofPtr< ofxGenericValueStore > ofxGenericImageView::toValueStore()
{
    ofPtr< ofxGenericValueStore > result = ofxGenericView::toValueStore();
    if ( result )
    {
        if ( _image && !_image->getFilePath().empty() )
        {
            result->write( "image", ofFilePath::getFileName( _image->getFilePath() ) );
        }
    }
    return result;
}

void ofxGenericImageView::imageManager_imageLoaded( std::string imageName, ofPtr< ofxGenericImage > image )
{
    setImage( image );
}
