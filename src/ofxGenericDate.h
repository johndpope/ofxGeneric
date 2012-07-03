//
//  ofxGenericDate.h
//  ofxGeneric
//
//  Class for date/time information.
//
//  Created by Eli Delventhal on 4/17/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"

#define ofxGenericDateFormatFullFileSafe "yyyy-MM-dd HH-mm-ss"
#define ofxGenericDateFormatFull "yyyy-MM-dd HH:mm:ss"
#define ofxGenericDateFormatDateOnly "yyyy-MM-dd"

#if TARGET_OS_IPHONE
#import <Foundation/Foundation.h>
#endif

//time is always in seconds
class ofxGenericDate
{
public:
    virtual ~ofxGenericDate();
    static ofPtr< ofxGenericDate > create();
    static ofPtr< ofxGenericDate > create( double time );
    static ofPtr< ofxGenericDate > create( string date, string format = ofxGenericDateFormatDateOnly );
    static double getSystemTime();
    
#if TARGET_OS_IPHONE
    operator NSDate*();
#endif
    
    virtual double getTime();
    
    ofPtr< ofxGenericDate > dateByAddingTime( double time );
    ofPtr< ofxGenericDate > dateByAddingDays( int days );
    virtual bool isEqualToDate( ofPtr< ofxGenericDate > date );
    virtual ofPtr< ofxGenericDate > earlierDate( ofPtr< ofxGenericDate > date );
    virtual ofPtr< ofxGenericDate > laterDate( ofPtr< ofxGenericDate > date );
    virtual int compare( ofPtr< ofxGenericDate > date );
    virtual bool isDifferentDay( ofPtr< ofxGenericDate > date );
    
    //gives the calendar year (4 digits)
    virtual int getYear();
    //month of the year 0-11
    virtual unsigned int getMonth();
    //day of the month 0-30
    virtual unsigned int getDate();
    //day of the week 0-6
    virtual unsigned int getDay();
    //hour of the day 0-23
    virtual unsigned int getHour();
    //minute of the day 0-59
    virtual unsigned int getMinute();
    //second of the day 0-59
    virtual unsigned int getSecond();
    
    virtual string getDescription();
    
    virtual string getStringRepresentation( string format = ofxGenericDateFormatDateOnly );
    
protected:
    ofxGenericDate();
    virtual void init( ofPtrWeak< ofxGenericDate > setThis, double time );
    
    ofPtrWeak< ofxGenericDate > _this;
    double _time;
    
    int _year;
    unsigned int _month;
    unsigned int _date;
    unsigned int _day;
    unsigned int _hour;
    unsigned int _minute;
    unsigned int _second;
};