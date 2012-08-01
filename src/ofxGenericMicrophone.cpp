//
//  ofxGenericMicrophone.cpp
//  ofxGeneric
//
//  Created by Ian Grossberg on 7/30/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericMicrophone.h"

ofPtr< ofxGenericMicrophone > ofxGenericMicrophone::create()
{
    ofPtr< ofxGenericMicrophone > create( new ofxGenericMicrophone() );
    create->init( create );
    return create;
}

ofxGenericMicrophone::ofxGenericMicrophone()
: _minimumPower( -160.0f ), _maximumPower( 0.0f )
{
}

void ofxGenericMicrophone::init( ofPtrWeak< ofxGenericMicrophone > setThis )
{
    _this = setThis;
    
#if TARGET_OS_IPHONE

    NSURL *url = [NSURL fileURLWithPath:@"/dev/null"];
		
	NSDictionary* settings = [ NSDictionary dictionaryWithObjectsAndKeys:
							  [ NSNumber numberWithFloat: 44100.0 ],                 AVSampleRateKey,
							  [ NSNumber numberWithInt: kAudioFormatAppleLossless ], AVFormatIDKey,
							  [ NSNumber numberWithInt: 1 ],                         AVNumberOfChannelsKey,
							  [ NSNumber numberWithInt: AVAudioQualityMax ],         AVEncoderAudioQualityKey,
							  nil ];
		
	NSError* error;
		
	_recorder = [ [ AVAudioRecorder alloc ] initWithURL:url settings:settings error:&error ];
	[ _recorder prepareToRecord ];
    [ _recorder setMeteringEnabled:YES ];
#endif
}

ofxGenericMicrophone::~ofxGenericMicrophone()
{
#if TARGET_OS_IPHONE
    [ _recorder release ];
#endif
}

void ofxGenericMicrophone::start()
{
#if TARGET_OS_IPHONE
    [ _recorder record ];
#endif
}

void ofxGenericMicrophone::pause()
{
#if TARGET_OS_IPHONE
    [ _recorder pause ];
#endif
}


void ofxGenericMicrophone::stop()
{
#if TARGET_OS_IPHONE
    [ _recorder stop ];
#endif
}



bool ofxGenericMicrophone::isRecording()
{
#if TARGET_OS_IPHONE
    return ( bool )[ _recorder isRecording ];
#endif
}


float ofxGenericMicrophone::getRecordedTime()
{
#if TARGET_OS_IPHONE
    return ( float )[ _recorder currentTime ]; 
#endif
}

float ofxGenericMicrophone::getAveragePowerForChannel( unsigned int channel )
{
#if TARGET_OS_IPHONE
    [ _recorder updateMeters ];
    return [ _recorder averagePowerForChannel:( NSUInteger ) channel ];
#endif
}

float ofxGenericMicrophone::getPeakPowerForChannel( unsigned int channel )
{
#if TARGET_OS_IPHONE
    [ _recorder updateMeters ];
    return [ _recorder peakPowerForChannel:( NSUInteger ) channel ];
#endif
}

float ofxGenericMicrophone::normalizePower( float value )
{
    return ( value - _minimumPower ) / ( _maximumPower - _minimumPower );
}