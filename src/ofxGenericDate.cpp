//
//  ofxGenericDate.cpp
//  ofxGeneric
//
//  Class for date/time information.
//
//  Created by Eli Delventhal on 4/17/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#include "ofxGenericDate.h"

ofxGenericDate::~ofxGenericDate()
{
    
}

ofxGenericDate::ofxGenericDate()
{
    
}

ofPtr< ofxGenericDate > ofxGenericDate::create()
{
    
#if TARGET_OS_IPHONE
    long now = [NSDate timeIntervalSinceReferenceDate]; 
#elif TARGET_ANDROID
    long now = 0;
#endif
    
    return ofxGenericDate::create( now );
}

ofPtr< ofxGenericDate > ofxGenericDate::create( long time )
{
    ofPtr< ofxGenericDate > d = ofPtr< ofxGenericDate > ( new ofxGenericDate() );
    d->init(d, time);
    return d;
}

//expecting a format like: 2012-05-01 09:24:14
ofPtr< ofxGenericDate > ofxGenericDate::create( string date )
{
    
#if TARGET_OS_IPHONE
    NSDateFormatter *f = [[[NSDateFormatter alloc] init] autorelease];
    [f setDateFormat:@"yyyy'-'MM'-'dd' 'HH':'mm':'ss"];
    NSDate *d = [f dateFromString:[NSString stringWithCString:date.c_str() encoding:NSUTF8StringEncoding]];
    long time = [d timeIntervalSinceReferenceDate];
#elif TARGET_ANDROID
    long time = 0;
#endif
    
    return ofxGenericDate::create( time );
}

ofPtr< ofxGenericDate > ofxGenericDate::dateByAddingTime( long time )
{
    return ofxGenericDate::create( _time + time );
}

long ofxGenericDate::getTime()
{
    return _time;
}

bool ofxGenericDate::isEqualToDate( ofPtr< ofxGenericDate > date )
{
    return _time == date->getTime();
}

ofPtr< ofxGenericDate > ofxGenericDate::earlierDate( ofPtr< ofxGenericDate > date )
{
    return _time < date->getTime() ? (_this.lock()) : date;
}

ofPtr< ofxGenericDate > ofxGenericDate::laterDate( ofPtr< ofxGenericDate > date )
{
    return _time > date->getTime() ? (_this.lock()) : date;
}

int ofxGenericDate::compare( ofPtr< ofxGenericDate > date )
{
    return _time > date->getTime() ? 1 : (_time < date->getTime() ? -1 : 0);
}

bool ofxGenericDate::isDifferentDay( ofPtr< ofxGenericDate > date )
{
    return ( _date != date->getDate() || _month != date->getMonth() || _year != date->getYear() );
}

int ofxGenericDate::getYear()
{
    return _year;
}

unsigned int ofxGenericDate::getMonth()
{
    return _month;
}

unsigned int ofxGenericDate::getDate()
{
    return _date;
}

unsigned int ofxGenericDate::getDay()
{
    return _day;
}

unsigned int ofxGenericDate::getHour()
{
    return _hour;
}

unsigned int ofxGenericDate::getMinute()
{
    return _minute;
}

unsigned int ofxGenericDate::getSecond()
{
    return _second;
}

string ofxGenericDate::getDescription()
{
#if TARGET_OS_IPHONE
    string desc = [[[NSDate dateWithTimeIntervalSinceReferenceDate:_time] description] UTF8String];
#else
    string desc = "";
#endif
    return desc;
}

void ofxGenericDate::init( ofPtrWeak< ofxGenericDate > setThis, long time )
{
    _this = setThis;
    _time = time;
    
#if TARGET_OS_IPHONE
    NSDate *date = [NSDate dateWithTimeIntervalSinceReferenceDate:time];
    NSCalendar *calendar = [[[NSCalendar alloc] initWithCalendarIdentifier:NSGregorianCalendar] autorelease];
    NSDateComponents *components = [calendar components:(NSHourCalendarUnit | NSMinuteCalendarUnit | NSSecondCalendarUnit | NSDayCalendarUnit | NSMonthCalendarUnit | NSYearCalendarUnit | NSWeekdayCalendarUnit) fromDate:date];
    _year = [components year];
    _month = [components month];
    _date = [components day];
    _day = [components weekday];
    _hour = [components hour];
    _minute = [components minute];
    _second = [components second];    
#else
    
#endif
}