//
//  ofxGenericImageView.cpp
//
//  Created by Ian Grossberg on 1/3/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericImageView.h"
#include "ofxGenericUtility.h"

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
    if ( !fileName.empty() )
    {
        create->setImage( fileName );
    }
    return create;
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

// TODO: auto @2x if available
void ofxGenericImageView::setImage( std::string fileName )
{
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UIImageView class ] ] )
    {
        UIImageView* view = ( UIImageView* )_view;
        
        std::string resolutionFileName( "" );
        if ( [ [ UIDevice currentDevice ] hasRetinaDisplay ] )
        {
            resolutionFileName = ofFilePath::removeExt( fileName );
            resolutionFileName += "@2x.";
            resolutionFileName += ofFilePath::getFileExt( fileName );
            if ( !ofxGFileExists( resolutionFileName, false ) )
            {
                resolutionFileName = fileName;
            }
        } else
        {
            resolutionFileName = fileName;
        }
        [ view setImage:[ UIImage imageWithContentsOfFile:ofxStringToNSString( ofxGPathToDataFolder( resolutionFileName ) ) ] ];
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
    _image = image;
#if TARGET_OS_IPHONE
    if ( [ _view isKindOfClass:[ UIImageView class ] ] )
    {
        UIImageView* view = ( UIImageView* )_view;
        if ( _image )
        {
            [ view setImage:[ UIImage imageWithData:UIImagePNGRepresentation( OFImageToUIImage( *image ) ) ] ]; // sanity check, was crashing for unknown reasons before :(
        } else
        {
            [ view setImage:nil ];
        }
    }
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
