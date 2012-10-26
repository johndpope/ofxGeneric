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

const char* ofxGenericDate::getFormat( ofxGenericDate::DateFormat format )
{
    switch ( format )
    {
        case DateFormatFullFileSafe: return "yyyy-MM-dd HH-mm-ss";
        case DateFormatFull: return "yyyy-MM-dd HH:mm:ss";
        case DateFormatDateOnly: return "yyyy-MM-dd";
        case DateFormatDateFileSafe: return getFormat( DateFormatDateOnly );
        case DateFormatPretty: return "MMMM dd, YYYY";
        case DateFormatServer: return "yyyy-MM-dd\'T\'HH:mm:ssZ";
        case DateFormatMonthDayPretty: return "MMM dd";
        case DateFormatDayOfTheWeek: return "EEE";
            
        default: return "yyyy-MM-dd HH-mm-ss";
    }
}

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
    d->init( d, time );
    return d;
}

ofPtr< ofxGenericDate > ofxGenericDate::create( string date, ofxGenericDate::DateFormat format )
{
    const char* formatAsString = getFormat( format );
    
    double time = 0;

#if TARGET_OS_IPHONE
    NSDateFormatter* formatter = [[[NSDateFormatter alloc] init] autorelease];
    [ formatter setDateFormat:[NSString stringWithCString:formatAsString encoding:NSUTF8StringEncoding]];
    
    NSDate* nsDate = [ formatter dateFromString:[NSString stringWithCString:date.c_str() encoding:NSUTF8StringEncoding ] ];
    time = [ nsDate timeIntervalSinceReferenceDate ];
#elif TARGET_ANDROID
#endif
    
    return ofxGenericDate::create( time );
}

ofPtr< ofxGenericDate > ofxGenericDate::create( ofPtr< ofxGenericValueStore > date, ofxGenericDate::DateFormat format )
{
    if ( date )
    {
        return create( date->asString(), format );
    }
    return ofPtr< ofxGenericDate >();
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

int ofxGenericDate::getDayOfTheWeek()
{
    int retval = 0;
    
#if TARGET_OS_IPHONE
    NSDate* date = convertToNSDate();
    NSCalendar* calendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSGregorianCalendar];
    NSDateComponents* weekdayComponents =[ calendar components:NSWeekdayCalendarUnit fromDate:date ];
    retval = [weekdayComponents weekday];
    [calendar release];
    
    // Adjust the value to be 0 - 6
    retval--;
#endif
    
    return retval;
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

string ofxGenericDate::getStringRepresentation( ofxGenericDate::DateFormat format, bool converToUTC )
{
    const char* formatAsString = getFormat( format );
    
#if TARGET_OS_IPHONE
    NSDateFormatter* formatter = [[[NSDateFormatter alloc] init] autorelease];
    if( converToUTC )
    {
        [formatter setTimeZone:[NSTimeZone timeZoneWithName:@"UTC"]];
    }
    [formatter setDateFormat:[NSString stringWithCString:formatAsString encoding:NSUTF8StringEncoding]];
    string str = string([[formatter stringFromDate:[NSDate dateWithTimeIntervalSinceReferenceDate:_time]] cStringUsingEncoding:NSUTF8StringEncoding]);
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
        
        NSCalendar* calendar = [[[NSCalendar alloc] initWithCalendarIdentifier:NSGregorianCalendar] autorelease];
        NSDateComponents* components = [calendar components:(NSHourCalendarUnit | NSMinuteCalendarUnit | NSSecondCalendarUnit | NSDayCalendarUnit | NSMonthCalendarUnit | NSYearCalendarUnit | NSWeekdayCalendarUnit) fromDate:date];
        
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

bool operator < ( ofPtr< ofxGenericDate > left, ofPtr< ofxGenericDate > right )
{
    if ( !left || !right )
    {
        return false;
    }
    return left->getTime() < right->getTime();
}

ofPtr< ofxGenericDate > operator - ( ofPtr< ofxGenericDate > left, ofPtr< ofxGenericDate > right )
{
    if ( left && right )
    {
        double difference = left->getTime() - right->getTime();
        return ofxGenericDate::create( difference );
    }
    
    return ofxGenericDate::create( 0 );
}