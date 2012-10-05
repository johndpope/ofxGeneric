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
    create->init( create, setFrame );
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
    UIImageView* newView = [ [ UIImageView alloc ] initWithFrame:ofRectangleToCGRect( frame ) ];
    [ newView setBackgroundColor:[ UIColor clearColor ] ];
    return newView;
#elif TARGET_ANDROID
    return ofxGenericView::createNativeView( frame );
#endif
}

void ofxGenericImageView::setImage( std::string fileName )
{
    if ( ofxGenericImageManager::getInstance().imageIsLoaded( fileName ) )
    {
        setImage( ofxGenericImageManager::getInstance().getImage( fileName ) );
        return;
    }
    
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UIImageView class ] ] )
    {
        UIImageView* view = ( UIImageView* )_view;
        
        [ view setImage:[ UIImage imageWithContentsOfFile:ofxStringToNSString( ofxGenericImage::getNativeImagePath( fileName ) ) ] ];
    }
#elif TARGET_ANDROID
    callJNIVoidMethod(
    		_jniMethods,
    		JNIMethod_SetImage
    		// , fileName
    		);
#endif
}

void ofxGenericImageView::setImage( ofPtr< ofImage > image )
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UIImageView class ] ] )
    {
        UIImageView* view = ( UIImageView* )_view;
        ofPtr< ofxGenericImage > newImage = ofxGenericImage::create( image );
        if ( _image && _image->loadedSuccessfully() )
        {
            _image = newImage;
            [ view setImage: _image->getUIImage() ];
        }
        else
        {
            _image = ofPtr< ofxGenericImage > ();
            [ view setImage:nil ];
        }
    }
#elif TARGET_ANDROID
#endif
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
