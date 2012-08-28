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
#include "ofUtils.h"
#include "ofxGenericLocalization.h"

#if TARGET_OS_IPHONE
#include <QuartzCore/QuartzCore.h>
#endif

#define SecondsInADay 86400.0

ofxGenericDate::~ofxGenericDate()
{
    
}

ofxGenericDate::ofxGenericDate()
{
    
}

ofPtr< ofxGenericDate > ofxGenericDate::create()
{
    
#if TARGET_OS_IPHONE
    double now = [NSDate timeIntervalSinceReferenceDate]; 
#elif TARGET_ANDROID
    double now = 0;
#endif
    
    return ofxGenericDate::create( now );
}

ofPtr< ofxGenericDate > ofxGenericDate::create( double time )
{
    ofPtr< ofxGenericDate > d = ofPtr< ofxGenericDate > ( new ofxGenericDate() );
    d->init(d, time);
    return d;
}

ofPtr< ofxGenericDate > ofxGenericDate::create( string date, string format )
{
    
#if TARGET_OS_IPHONE
    NSDateFormatter *f = [[[NSDateFormatter alloc] init] autorelease];
    [f setDateFormat:[NSString stringWithCString:format.c_str() encoding:NSUTF8StringEncoding]];
    NSDate *d = [f dateFromString:[NSString stringWithCString:date.c_str() encoding:NSUTF8StringEncoding]];
    double time = [d timeIntervalSinceReferenceDate];
#elif TARGET_ANDROID
    double time = 0;
#endif
    
    return ofxGenericDate::create( time );
}

#if TARGET_OS_IPHONE
ofPtr< ofxGenericDate > ofxGenericDate::createFromNSDate( NSDate* date )
{
    if( date )
    {
        ofPtr< ofxGenericDate > retval = ofxGenericDate::create();
        retval->setFromNSDate( date );
        return retval;
    }

    return ofPtr< ofxGenericDate >();
}
#endif

double ofxGenericDate::getSystemTime()
{
#if TARGET_OS_IPHONE
    return [NSDate timeIntervalSinceReferenceDate]; //CACurrentMediaTime(); //not compatible with what we're using for date
#elif TARGET_ANDROID
    return 0.0;
#endif
}

string  ofxGenericDate::getDayName(int day)
{
    switch (day)
    {
        case 0:
            return ofxGLocalized("Sunday", "Sunday");
        case 1:
            return ofxGLocalized("Monday", "Monday");
        case 2:
            return ofxGLocalized("Tuesday", "Tuesday");
        case 3:
            return ofxGLocalized("Wednesday", "Wednesday");
        case 4:
            return ofxGLocalized("Thursday", "Thursday");
        case 5:
            return ofxGLocalized("Friday", "Friday");
        case 6:
            return ofxGLocalized("Saturday", "Saturday");
    }
    return "";
}

ofPtr< ofxGenericDate > ofxGenericDate::dateByAddingTime( double time )
{
    return ofxGenericDate::create( _time + time );
}

ofPtr< ofxGenericDate > ofxGenericDate::dateByAddingDays( int days )
{
    return ofxGenericDate::create( _time + days * SecondsInADay );
}

double ofxGenericDate::getTime()
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

void ofxGenericDate::init( ofPtrWeak< ofxGenericDate > setThis, double time )
{
    _this = setThis;
    _time = time;

    setFromSinceReferenceDate( time );
}

string ofxGenericDate::getStringRepresentation( string format )
{
#if TARGET_OS_IPHONE
    NSDateFormatter *f = [[[NSDateFormatter alloc] init] autorelease];
    [f setDateFormat:[NSString stringWithCString:format.c_str() encoding:NSUTF8StringEncoding]];
    string str = string([[f stringFromDate:[NSDate dateWithTimeIntervalSinceReferenceDate:_time]] cStringUsingEncoding:NSUTF8StringEncoding]);
#elif TARGET_ANDROID
    string str = "";
#endif
    
    return str;
}

void ofxGenericDate::setFromSinceReferenceDate( double time )
{
    _time = time;
    
#if TARGET_OS_IPHONE
    setFromNSDate( [ NSDate dateWithTimeIntervalSinceReferenceDate:_time ] );
#endif
}

#if TARGET_OS_IPHONE
NSDate* ofxGenericDate::convertToNSDate()
{
    return [NSDate dateWithTimeIntervalSinceReferenceDate:_time];
}

void ofxGenericDate::setFromNSDate( NSDate* date )
{
    if( date )
    {
        _time = [date timeIntervalSinceReferenceDate];
        
        NSCalendar *calendar = [[[NSCalendar alloc] initWithCalendarIdentifier:NSGregorianCalendar] autorelease];
        NSDateComponents *components = [calendar components:(NSHourCalendarUnit | NSMinuteCalendarUnit | NSSecondCalendarUnit | NSDayCalendarUnit | NSMonthCalendarUnit | NSYearCalendarUnit | NSWeekdayCalendarUnit) fromDate:date];
        _year = [components year];
        _month = [components month];
        _date = [components day];
        _day = [components weekday] - 1; //returns 1 - 7, where Sunday is 1
        _hour = [components hour];
        _minute = [components minute];
        _second = [components second];
    }
}
#endif