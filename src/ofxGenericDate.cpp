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

#include "ofxGenericLocalization.h"
#include "ofxGenericException.h"

#define SecondsInADay 86400.0

#if TARGET_OS_IPHONE
static NSDateFormatter* getCanonicalDateFormatter()
{
    static NSDateFormatter *canonicalDateFormatter = nil;  // creating NSDateFormatter is expensive .. hold on to static instance
    if (!canonicalDateFormatter)
    {
        canonicalDateFormatter = [[NSDateFormatter alloc] init];
        NSLocale *locale = [[NSLocale alloc] initWithLocaleIdentifier:@"en_US_POSIX"]; // reference: https://developer.apple.com/library/ios/qa/qa1480/_index.html
        [canonicalDateFormatter setLocale:[locale autorelease]];
        [canonicalDateFormatter setDateFormat:@"yyyy-MM-dd'T'HH:mm:ssxx"];
    }
    [canonicalDateFormatter setTimeZone:[NSTimeZone systemTimeZone]]; // set every time in case user has changed time zones with app still running
    
    return canonicalDateFormatter;
}

static NSDateFormatter* getCalendarDayCanonicalDateFormatter()
{
    static NSDateFormatter *calendarDayCanonicalDateFormatter = nil;  // creating NSDateFormatter is expensive .. hold on to static instance
    if (!calendarDayCanonicalDateFormatter)
    {
        calendarDayCanonicalDateFormatter = [[NSDateFormatter alloc] init];
        NSLocale *locale = [[NSLocale alloc] initWithLocaleIdentifier:@"en_US_POSIX"]; // reference: https://developer.apple.com/library/ios/qa/qa1480/_index.html
        [calendarDayCanonicalDateFormatter setLocale:[locale autorelease]];
        [calendarDayCanonicalDateFormatter setDateFormat:@"yyyy-MM-dd"];
    }
    [calendarDayCanonicalDateFormatter setTimeZone:[NSTimeZone systemTimeZone]]; // set every time in case user has changed time zones with app still running
    
    return calendarDayCanonicalDateFormatter;
}
#endif

string ofxGenericDate::getDateFormatAsString( ofxGenericDate::DateFormat format )
{
    switch ( format )
    {
        case DateFormatFullFileSafe: return "yyyy-MM-dd HH-mm-ss";
        case DateFormatFull: return "yyyy-MM-dd HH:mm:ss";
        case DateFormatDateOnly: return "yyyy-MM-dd";
        case DateFormatDateFileSafe: return getDateFormatAsString( DateFormatDateOnly );
        case DateFormatPretty: return "MMMM dd, yyyy";
        case DateFormatServer: return "yyyy-MM-dd\'T\'HH:mm:ssZ";
        case DateFormatMonthDayPretty: return "MMM dd";
        case DateFormatDayOfTheWeek: return "EEE";
        case DateFormatTime: return "hh:mm a";
            
        default: return "yyyy-MM-dd HH-mm-ss";
    }
    return "";
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
    return create( date, getDateFormatAsString( format ) );
}

ofPtr< ofxGenericDate > ofxGenericDate::create( string date, string format )
{
    ofPtr< ofxGenericDate > create;
    if ( !date.empty() )
    {
        double time = 0;
        
#if TARGET_OS_IPHONE
        NSDateFormatter* formatter = [ [ [ NSDateFormatter alloc ] init ] autorelease ];
        NSString* nsFormat = ofxStringToNSString( format );
        [ formatter setDateFormat:nsFormat ];
        
        NSString* nsDateString = ofxStringToNSString( date );
        NSDate* nsDate = [ formatter dateFromString:nsDateString ];
        time = [ nsDate timeIntervalSinceReferenceDate ];
#elif TARGET_ANDROID
        throw ofxGenericExceptionMemberNotImplement( "ofxGenericDate", "create( string, string )" );
#endif
        
        create = ofxGenericDate::create( time );
    }
    return create;
}

ofPtr< ofxGenericDate > ofxGenericDate::create( ofPtr< ofxGenericValueStore > date, ofxGenericDate::DateFormat format )
{
    if ( date )
    {
        return create( date->asString(), format );
    }
    return ofPtr< ofxGenericDate >();
}

ofPtr< ofxGenericDate > ofxGenericDate::create( ofPtr< ofxGenericValueStore > date, string format )
{
    if ( date )
    {
        return create( date->asString(), format );
    }
    return ofPtr< ofxGenericDate >();
}


ofPtr< ofxGenericDate > ofxGenericDate::createWithComponents( int dayOfTheWeek, int hour, int minute )
{
    ofPtr< ofxGenericDate > retval = ofPtr< ofxGenericDate >();
    
#if TARGET_OS_IPHONE
    NSDate* today = [NSDate date];
    NSCalendar* gregorian = [[NSCalendar alloc] initWithCalendarIdentifier:NSGregorianCalendar];
    [gregorian setLocale:[NSLocale currentLocale]];
    
    NSDateComponents* nowComponents = [gregorian components:NSYearCalendarUnit | NSWeekCalendarUnit | NSHourCalendarUnit | NSMinuteCalendarUnit | NSSecondCalendarUnit fromDate:today];    
    [nowComponents setWeekday:(dayOfTheWeek + 1)]; // Our days of the week are 0-6, 0 is sunday
    [nowComponents setHour:hour];
    [nowComponents setMinute:minute];
    [nowComponents setSecond:0];
    
    NSDate* newDate = [gregorian dateFromComponents:nowComponents];
    if ( [newDate timeIntervalSinceReferenceDate] <= [today timeIntervalSinceReferenceDate] )
    {
        [nowComponents setWeek: [nowComponents week] + 1];
        newDate = [gregorian dateFromComponents:nowComponents];
    }
    
    retval = ofxGenericDate::createFromNSDate( newDate );
#endif
    
    return retval;
}

ofPtr< ofxGenericDate > ofxGenericDate::createFromCanonicalRepresentation( string representation )
{
    ofPtr< ofxGenericDate > instance = ofPtr< ofxGenericDate >();
#if TARGET_OS_IPHONE
    NSDate *date = [getCanonicalDateFormatter() dateFromString:[NSString stringWithUTF8String:representation.c_str()]];
    if ( date )
    {
        instance = ofxGenericDate::createFromNSDate( date );
    }
#endif
    
    return instance;
}

ofPtr< ofxGenericDate > ofxGenericDate::createFromCalendarDayCanonicalRepresentation( string representation )
{
    ofPtr< ofxGenericDate > instance = ofPtr< ofxGenericDate >();
#if TARGET_OS_IPHONE
    NSDate *date = [getCalendarDayCanonicalDateFormatter() dateFromString:[NSString stringWithUTF8String:representation.c_str()]];
    if ( date )
    {
        instance = ofxGenericDate::createFromNSDate( date );
    }
#endif
    
    return instance;
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

ofPtr< ofxGenericDate > ofxGenericDate::now()
{
    return ofxGenericDate::create();
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

string ofxGenericDate::getStringRepresentation( ofxGenericDate::DateFormat format, bool convertToUTC )
{
    return getStringRepresentation( getDateFormatAsString( format ), convertToUTC );
}

string ofxGenericDate::getStringRepresentation( string format, bool convertToUTC )
{
    string result;
    
#if TARGET_OS_IPHONE
    NSDateFormatter* formatter = [ [ [ NSDateFormatter alloc ] init ] autorelease ];
    if( convertToUTC )
    {
        [ formatter setTimeZone:[ NSTimeZone timeZoneWithName:@"UTC" ] ];
    }
    
    bool replaceDay = false;
    size_t start, end;
    if ( ofxGenericLocalization::getPreferredISOLanguage() != "en" )
    {
        start = format.find( "E" );
        if ( start != string::npos )
        {
            replaceDay = true;
            end = format.find_first_not_of( "E", start );
            if ( end == string::npos )
            {
                end = format.size();
            }
            format.replace( start, end - start, "|||" );
        }
    }
    
    NSDate* nsDate = convertToNSDate();
    [ formatter setDateFormat:ofxStringToNSString( format ) ];
    result = ofxNSStringToString( [ formatter stringFromDate:nsDate ] );
    
    if ( replaceDay )
    {
        NSArray* localizedDaysOfTheWeek;
        if ( end - start > 3 )
        {
            localizedDaysOfTheWeek = [ formatter weekdaySymbols ];
        } else if ( end - start > 1 )
        {
            localizedDaysOfTheWeek = [ formatter shortWeekdaySymbols ];
        } else
        {
            localizedDaysOfTheWeek = [ formatter veryShortWeekdaySymbols ];
        }
        string localizedDay = ofxNSStringToString( [ localizedDaysOfTheWeek objectAtIndex:getDay() ] );
        size_t insertStart, insertEnd;
        insertStart = result.find( "|" );
        insertEnd = result.find_first_not_of( "|", insertStart );
        if ( insertEnd == string::npos )
        {
            insertEnd = result.size();
        }

        result.replace( insertStart, insertEnd - insertStart, localizedDay.substr( 0, end - start ) );
    }

#elif TARGET_ANDROID
#endif
    
    return result;
}

string ofxGenericDate::getCanonicalRepresentation()
{
    string representation = "";
#if TARGET_OS_IPHONE
    representation = [[getCanonicalDateFormatter() stringFromDate:convertToNSDate()] UTF8String];
#endif
    
    return representation;
}

string ofxGenericDate::getCalendarDayCanonicalRepresentation()
{
    string representation = "";
#if TARGET_OS_IPHONE
    representation = [[getCalendarDayCanonicalDateFormatter() stringFromDate:convertToNSDate()] UTF8String];
#endif
    
    return representation;
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

int ofxGenericDate::getMillisecondOffsetFromUTC()
{
    int result = 0;
#if TARGET_OS_IPHONE
    result = 1000 * [ [ NSTimeZone localTimeZone ] secondsFromGMT ];
#else 
    throw ofxGenericExceptionMemberNotImplement( "ofxGenericDate", "getMillisecondOffsetFromUTC" );
#endif
    return result;
}

int ofxGenericDate::getMillisecondOffsetFromUTCWithoutDaylightSavings()
{
    return getMillisecondOffsetFromUTC() + ( int )( getMillisecondCurrentDaylightSavingsOffset() );
}

double ofxGenericDate::getMillisecondCurrentDaylightSavingsOffset()
{
    double result = 0;
#if TARGET_OS_IPHONE
    if ( [ [ NSTimeZone localTimeZone ] isDaylightSavingTime ] )
    {
        result = 1000 * -1 * [ [ NSTimeZone localTimeZone ] daylightSavingTimeOffset ];
    }
#else
    throw ofxGenericExceptionMemberNotImplement( "ofxGenericDate", "getMillisecondCurrentDaylightSavingsOffset" );
#endif
    return result;
}

bool operator == ( ofPtr< ofxGenericDate > left, ofPtr< ofxGenericDate > right )
{
    if ( !left || !right )
    {
        return !left && !right;
    }
    
    return left->getTime() == right->getTime();
}

bool operator != ( ofPtr< ofxGenericDate > left, ofPtr< ofxGenericDate > right )
{
    return !( left == right );
}

bool operator < ( ofPtr< ofxGenericDate > left, ofPtr< ofxGenericDate > right )
{
    if ( !left || !right )
    {
        return false;
    }
    return left->getTime() < right->getTime();
}

bool operator > ( ofPtr< ofxGenericDate > left, ofPtr< ofxGenericDate > right )
{
    return right < left;
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