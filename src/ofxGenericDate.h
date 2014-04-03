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

#if TARGET_OS_IPHONE
#import <Foundation/Foundation.h>
#endif

class ofxGenericValueStore;

//time is always in seconds
class ofxGenericDate
{
public:
    
    enum DateFormat
    {
        DateFormatFullFileSafe,
        DateFormatFull,
        DateFormatDateOnly,
        DateFormatDateFileSafe,
        DateFormatPretty,
        DateFormatServer,
        DateFormatMonthDayPretty,
        DateFormatDayOfTheWeek,
        DateFormatTime,
        
        DateFormatCount
    };
    
    static string getDateFormatAsString( ofxGenericDate::DateFormat format );
    
    virtual ~ofxGenericDate();
    static ofPtr< ofxGenericDate > create();
    static ofPtr< ofxGenericDate > create( double time );
    
    // NOTE: for creating date objects from data interchange date strings, use the createFromCanonicalRepresentation functions below
    static ofPtr< ofxGenericDate > create( string date, ofxGenericDate::DateFormat format = ofxGenericDate::DateFormatDateOnly );
    static ofPtr< ofxGenericDate > create( string date, string format );
    static ofPtr< ofxGenericDate > create( ofPtr< ofxGenericValueStore > date, ofxGenericDate::DateFormat format = ofxGenericDate::DateFormatDateOnly );
    static ofPtr< ofxGenericDate > create( ofPtr< ofxGenericValueStore > date, string format );
    static ofPtr< ofxGenericDate > createWithComponents( int dayOfTheWeek, int hour, int minute );
    
    // NOTE: when creating date objects from ISO 8601 data interchange date strings, use these two functions
    static ofPtr< ofxGenericDate > createFromCanonicalRepresentation( string representation );
    static ofPtr< ofxGenericDate > createFromCalendarDayCanonicalRepresentation( string representation );
    
#if TARGET_OS_IPHONE       
    static ofPtr< ofxGenericDate > createFromNSDate( NSDate* date );
#endif
    static double getSystemTime();
    static ofPtr< ofxGenericDate > now();
    
    static string getDayName(int day);
    
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
    
    // NOTE: only use getStringRepresentation functions for creating user-visible strings for the UI.  For data interchange, use getCanonicalRepresentation functions below
    virtual string getStringRepresentation( ofxGenericDate::DateFormat format = ofxGenericDate::DateFormatDateOnly, bool convertToUTC = false );
    virtual string getStringRepresentation( string format, bool convertToUTC = false );
    
    // NOTE: use getCanonicalRepresentation functions to get an ISO 8601 data interchange-safe string that represents the date, for caching, sending to server, etc
    string getCanonicalRepresentation();            // eg, 2014-04-01T15:30:05-0800
    string getCalendarDayCanonicalRepresentation(); // eg, 2014-04-01 ... only represents a calendar day, not a moment in time. Not affected by user's calendar setting (e.g., Buddhist calendar)
    
    void setFromSinceReferenceDate( double time );
    
#if TARGET_OS_IPHONE    
    NSDate* convertToNSDate();
    void setFromNSDate( NSDate* date );
#endif
    
    static int getMillisecondOffsetFromUTC();
    static int getMillisecondOffsetFromUTCWithoutDaylightSavings();
    
    static double getMillisecondCurrentDaylightSavingsOffset();
    
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

bool operator == ( ofPtr< ofxGenericDate > left, ofPtr< ofxGenericDate > right );
bool operator != ( ofPtr< ofxGenericDate > left, ofPtr< ofxGenericDate > right );
bool operator < ( ofPtr< ofxGenericDate > left, ofPtr< ofxGenericDate > right );
bool operator > ( ofPtr< ofxGenericDate > left, ofPtr< ofxGenericDate > right );
ofPtr< ofxGenericDate > operator - ( ofPtr< ofxGenericDate > left, ofPtr< ofxGenericDate > right );
