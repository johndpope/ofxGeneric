#include "ofxGenericUtility.h"
#include "ofLog.h"
#include "ofxGenericConstants.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#if TARGET_OS_IPHONE
#include "NSFileManager+Tar.h"
#include "SSZipArchive.h"
#endif

//////////////////////////////// Path //////////////////////////////////

string ofxGGetPathFromFileName( string fileName )
{
    unsigned int slashPos = fileName.find_last_of( "/\\" );
    if ( slashPos != fileName.npos )
    {
        return fileName.substr( 0, slashPos );
    }
    return "";
}

bool ofxGmkdir( string loc, bool useDocuments )
{
    if ( !loc.empty() )
    {
        string filename = ofToPath( loc, useDocuments, true );
        return mkdir( filename.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) == 0;
    }
    return false;
}

bool ofxGrmdir( string loc, bool useDocuments, bool onlyIfEmpty )
{
    if ( !loc.empty() )
    {
        string fileName = ofToPath( loc, useDocuments, true );
#if TARGET_OS_IPHONE
        while ( fileName.length() > 0 && ( fileName[ fileName.length() - 1 ] == '\\' || fileName[ fileName.length() - 1 ] == '/' ) )
        {
            fileName = fileName.substr( 0, fileName.length() - 1 );
        }
                                                                   
        NSError* error = nil;
        NSFileManager* fileManager = [ NSFileManager defaultManager ];
        if ( onlyIfEmpty )
        {
            NSArray* contents = [ fileManager contentsOfDirectoryAtPath:ofxStringToNSString( fileName ) error:&error ];
            if ( [ contents count ] > 0 )
            {
                return false;
            }
        }
        return [ fileManager removeItemAtPath:ofxStringToNSString( fileName ) error:&error ] == YES;
#elif TARGET_ANDROID
        // TODO: onlyIfEmpty == false
        return rmdir( fileName.c_str() ) == 0;
#endif
    }
    return false;
}

bool ofxGFileExists( string absolutePath )
{
#if TARGET_OS_IPHONE
    return ( bool )[ [ NSFileManager defaultManager ] fileExistsAtPath:ofxStringToNSString( absolutePath ) ];
#elif TARGET_ANDROID
    ofxGLogError << "ofxGFileExists(...) isn't implemented for Android yet!";
#endif
    return false;
}

bool ofxGFileExists( string location, bool useDocuments )
{
    return ofxGFileExists( ofToPath( location, useDocuments, true ) );
}

//////////////////////////////// Font //////////////////////////////////

float ofxGFontSizeForText( string text, string fontName, float startingFontSize, const ofPoint& constrainedSize )
{
#if TARGET_OS_IPHONE
    // http://stackoverflow.com/questions/4382976/multiline-uilabel-with-adjustsfontsizetofitwidth
    CGFloat fontSize = startingFontSize;
    NSString* nsFontName = [ NSString stringWithCString:fontName.c_str() encoding:NSUTF8StringEncoding ];
    UIFont* font = [ UIFont fontWithName:nsFontName size:fontSize ];
    
    NSString* nsText = [ NSString stringWithCString:text.c_str() encoding:NSUTF8StringEncoding ];
    CGFloat height = [ nsText sizeWithFont:font constrainedToSize:CGSizeMake( constrainedSize.x, FLT_MAX ) lineBreakMode:NSLineBreakByWordWrapping ].height;
    UIFont *newFont = font;
    
    //Reduce font size while too large, break if no height (empty string)
    while ( height > constrainedSize.y && height != 0 ) 
    {   
        fontSize--;  
        newFont = [ UIFont fontWithName:nsFontName size:fontSize ];   
        height = [ nsText sizeWithFont:newFont constrainedToSize:CGSizeMake( constrainedSize.x, FLT_MAX ) lineBreakMode:NSLineBreakByWordWrapping ].height;
    };
    
    // Loop through words in string and resize to fit
    for ( NSString* word in [ nsText componentsSeparatedByCharactersInSet:[ NSCharacterSet whitespaceAndNewlineCharacterSet ] ] ) 
    {
        CGFloat width = [ word sizeWithFont:newFont ].width;
        while ( width > constrainedSize.x && width != 0 ) 
        {
            fontSize--;
            newFont = [ UIFont fontWithName:nsFontName size:fontSize ];   
            width = [ word sizeWithFont:newFont ].width;
        }
    }
    return ( float )fontSize;   
#else
    return 0.0f;
#endif
}

ofPoint ofxGPointSizeForText( string text, string fontName, float fontSize, float constrainedWidth )
{
#if TARGET_OS_IPHONE
    NSString* nsFontName = [ NSString stringWithCString:fontName.c_str() encoding:NSUTF8StringEncoding ];
    UIFont* font = [ UIFont fontWithName:nsFontName size:fontSize ];
    NSString* nsText = [ NSString stringWithCString:text.c_str() encoding:NSUTF8StringEncoding ];
    CGSize size = [ nsText sizeWithFont:font constrainedToSize:CGSizeMake( constrainedWidth, FLT_MAX ) lineBreakMode:NSLineBreakByWordWrapping ];
    return ofPoint( size.width, size.height );
#else
    return ofPoint( 0, 0 );
#endif
}

//////////////////////////////// Logging //////////////////////////////////

void ofxGLogError( const string & message )
{
	ofLogError( ofxGenericModuleName, message );
}

void ofxGLogFatalError( const string & message )
{
	ofLogFatalError( ofxGenericModuleName, message );
}

void ofxGLogNotice( const string & message )
{
	ofLogNotice( ofxGenericModuleName, message );
}

void ofxGLogVerbose( const string & message )
{
	ofLogVerbose( ofxGenericModuleName, message );
}

void ofxGLogWarning( const string & message )
{
	ofLogWarning( ofxGenericModuleName, message );
}

//////////////////////////////// String conversions //////////////////////////////////

string ofxGToString( int value )
{
    char buffer[ 1024 ];
    snprintf( buffer, 1024, "%d", value );
    return string( buffer );
}

string ofxGToString( long int value )
{
    char buffer[ 1024 ];
    snprintf( buffer, 1024, "%ld", value );
    return string( buffer );
}

string ofxGToString( unsigned int value )
{
    char buffer[ 1024 ];
    snprintf( buffer, 1024, "%u", value );
    return string( buffer );
}

string ofxGToString( unsigned long int value )
{
    char buffer[ 1024 ];
    snprintf( buffer, 1024, "%lu", value );
    return string( buffer );
}

string ofxGToString( double value )
{
    char buffer[ 1024 ];
    snprintf( buffer, 1024, "%f", value );
    return string( buffer );
}

string ofxGToString( float value )
{
    char buffer[ 1024 ];
    snprintf( buffer, 1024, "%f", value );
    return string( buffer );
}

string ofxGToString( float value, unsigned int decimalCount )
{
    return ofxGTrimDecimals( ofxGToString( value ), decimalCount );
    
}

string ofxGToString( double value, unsigned int decimalCount )
{
    return ofxGTrimDecimals( ofxGToString( value ), decimalCount );
}

string ofxGTrimDecimals( string str, unsigned int decimalCount )
{
    unsigned int decimalPlace = str.find( "." );
    if ( decimalPlace == string::npos || str.size() - decimalPlace - 1 <= decimalCount )
    {
        return str;
    }
    
    if ( decimalCount <= 0 )
    {
        return str.substr( 0, decimalPlace );
    }
    
    return str.substr( 0, decimalPlace + decimalCount + 1 );
}

string ofxGToString( bool value )
{
    if ( value )
    {
        return "true";
    }
    return "false";
}

string ofxGToString( const ofColor& color )
{
    return "(" + ofxGToString( color.r ) + "," + ofxGToString( color.g ) + "," + ofxGToString( color.b ) + "," + ofxGToString( color.a ) + ")";
}

string ofxGToString( const ofPoint& point, bool addSpaceAfterComma )
{
    string result;
    
    result = "(" + ofxGToString( point.x ) + ",";
    if ( addSpaceAfterComma )
    {
        result += " ";
    }
    result += ofxGToString( point.y ) + ")";

    return result;
}

string ofxGToString( const ofRectangle& rect )
{
    return "(" + ofxGToString( rect.x ) + "," + ofxGToString( rect.y ) + ") " + ofxGToString( rect.width ) + "x" + ofxGToString( rect.height );
}

string ofxGToString( const void* value, unsigned int byteSize )
{
    char* dataBuffer = new char[ byteSize + 1 ];
    snprintf( dataBuffer, byteSize + 1, "%s", value );
    
    string dataString( dataBuffer );
    
    delete [] dataBuffer;
    
    return dataString;
}

bool ofxGToBool( string value )
{
    std::transform( value.begin(), value.end(), value.begin(), ::tolower );
    if ( value == "true" || value == "t" || value == "1" )
    {
        return true;
    }
    return false;
}

#define ofxSPrintfReplace "%s"

string ofxGSPrintf( string format, const std::vector< string >& replaceWith )
{
    string replace = ofxSPrintfReplace;
    
    string result;
    
    size_t start = 0;
    unsigned int replaceWithIndex = 0;
    for( size_t foundAt = format.find( replace, start ); foundAt != format.npos; foundAt = format.find( replace, start ) )
    {
        result += format.substr( start, foundAt - start );
        if ( replaceWithIndex < replaceWith.size() )
        {
            result += replaceWith[ replaceWithIndex ];
        } else
        {
            ofxGLogVerbose( "ofxSPrintf called expecting more replacement strings - \"" + format + "\"" );
        }

        start = foundAt + replace.size();
        replaceWithIndex ++;
    }
    if ( start < format.size() )
    {
        result += format.substr( start, format.size() - start );
    }
    
    if ( replaceWithIndex != replaceWith.size() )
    {
        ofxGLogVerbose( "ofxSPrintf called with more replacement strings than format calls for - \"" + format + "\"" );
    }
    
    return result;
}

string ofxGSPrintf( string format, string first )
{
    std::vector< string > replaceWith;
    replaceWith.push_back( first );
    return ofxGSPrintf( format, replaceWith );
}

string ofxGSPrintf( string format, string first, string second )
{
    std::vector< string > replaceWith;
    replaceWith.push_back( first );
    replaceWith.push_back( second );
    return ofxGSPrintf( format, replaceWith );
}

string ofxGSPrintf( string format, string first, string second, string third )
{
    std::vector< string > replaceWith;
    replaceWith.push_back( first );
    replaceWith.push_back( second );
    replaceWith.push_back( third );
    return ofxGSPrintf( format, replaceWith );
}

std::vector< string > ofxGSplit( string value, char splitOn )
{
    std::vector< string > strings;
    
    unsigned int lastPosition = 0;
    for( unsigned int position = value.find( splitOn, 0 ); position < value.size(); position = value.find( splitOn, position ) )
    {
        if ( position == lastPosition + 1 )
        {
            continue;
        }
        strings.push_back( value.substr( lastPosition, position - lastPosition ) );
        position ++;
        lastPosition = position;
    }
    if ( value[ value.size() - 1 ] != splitOn )
    {
        strings.push_back( value.substr( lastPosition, value.size() - lastPosition ) );
    }
    return strings;
}

string ofxGToUpperCase( const string& text )
{
    string retval;
    retval.resize( text.size() );
    std::transform( text.begin(), text.end(), retval.begin(), ::toupper );
    return retval;
}

string ofxGToLowerCase( const string& text )
{
    string retval;
    retval.resize( text.size() );
    std::transform( text.begin(), text.end(), retval.begin(), ::tolower );
    return retval;
}

string ofxGURLSafeString( const string& text )
{
#if TARGET_OS_IPHONE
    NSString *nstext = [ NSString stringWithCString:text.c_str() encoding:NSUTF8StringEncoding ];
    NSString *nsstr = (NSString *)CFURLCreateStringByAddingPercentEscapes(NULL, (CFStringRef)nstext, NULL, (CFStringRef)@"!*'\"();:@&=+$,/?%#[]% ", CFStringConvertNSStringEncodingToEncoding( NSUTF8StringEncoding ) );
    [ nsstr autorelease ];
    return string( [ nsstr cStringUsingEncoding:NSUTF8StringEncoding ] );
#elif TARGET_ANDROID
    //presumably there is an easy API way to do this on Android
#endif
    return text;
}

//TODO rewrite this better
bool ofxGStringIsXMLSafe( const string& text )
{
    const string unsafe = "\"&\'<>";
    for ( unsigned int i = 0; i < text.length(); i++ )
    {
        for ( unsigned int j = 0; j < unsafe.length(); j++ )
        {
            if ( text[i] == unsafe[j] )
            {
                return false;
            }
        }
    }
    return true;
}

ofPoint ofxGToPoint( const string& value )
{
    ofPoint result;
    
    string parse = value;
    if ( !parse.empty() )
    {
        size_t quoteLocation = parse.find( "\"" );
        while ( quoteLocation != std::string::npos )
        {
            parse.erase( quoteLocation, 1 );
            quoteLocation = parse.find( "\"" );
        }
        size_t leftParamLocation = parse.find( "(" );
        while ( leftParamLocation != std::string::npos )
        {
            parse.erase( leftParamLocation, 1 );
            leftParamLocation = parse.find( "(" );
        }
        
        size_t rightParamLocation = parse.find( ")" );
        while ( rightParamLocation != std::string::npos )
        {
            parse.erase( rightParamLocation, 1 );
            rightParamLocation = parse.find( ")" );
        }
        
        size_t commaLocation = parse.find( "," );
        if ( commaLocation != std::string::npos )
        {
            unsigned int index = 0;
            size_t from = 0;
            while( index < ofPoint::DIM )
            {
                string stringValue = parse.substr( from, commaLocation - from );
                result[ index ] = ofToFloat( stringValue );
                
                if ( commaLocation == parse.length() )
                {
                    break;
                } else
                {
                    commaLocation = commaLocation + 1;
                }
                
                from = commaLocation;
                commaLocation = parse.find( ",", from );
                if ( commaLocation == std::string::npos )
                {
                    commaLocation = parse.length();
                }
                index ++;
            }
        } else
        {
            float allValues = ofToFloat( parse );
            result.x = allValues;
            result.y = allValues;
            result.z = allValues;
        }
    }
    
    return result;
}

//////////////////////////////// Math //////////////////////////////////

int ofxGRandom()
{
    return abs( (int)arc4random() );
}

int ofxGRandomInRange( int minimum, int maximum )
{
    return minimum + ( ofxGRandom() % ( int )( maximum - minimum + 1 ) );
}

int ofxGRandomInRangeExclusive( int minimumInclusive, int maximumExclusive )
{
    return ofxGRandomInRange( minimumInclusive, maximumExclusive - 1 );
}

ofRectangle ofxGGetCenteredSquareFit( const ofRectangle& rectangle )
{
    float dimension = MIN( rectangle.width, rectangle.height );
    ofRectangle result( 0, 0, dimension, dimension );
    result.alignTo( rectangle );
    return result;
}

//////////////////////////////// HTTP //////////////////////////////////

#define ofxGenericMIMETypeJSONString "application/json"
#define ofxGenericMIMETypeXMLString "application/xml"

string ofxGMIMETypeToString( ofxGenericMIMEType type )
{
    switch( type )
    {
        case ofxGenericMIMETypeJSON:
            return ofxGenericMIMETypeJSONString;
        case ofxGenericMIMETypeXML:
            return ofxGenericMIMETypeXMLString;
        case ofxGenericMIMETypeUnknown:
            return "";
    }
}

ofxGenericMIMEType ofxGStringToMIMEType( string type )
{
    if ( type == ofxGenericMIMETypeJSONString )
    {
        return ofxGenericMIMETypeJSON;
    } else if ( type == ofxGenericMIMETypeXMLString )
    {
        return ofxGenericMIMETypeXML;
    }
    
    return ofxGenericMIMETypeUnknown;
}

string ofxGMIMETypeToExtension( ofxGenericMIMEType type )
{
    switch( type )
    {
        case ofxGenericMIMETypeJSON:
            return "json";
        case ofxGenericMIMETypeXML:
            return "xml";
        case ofxGenericMIMETypeUnknown:
            return "";
    }
}

//////////////////////////////// Compression //////////////////////////////////

bool ofxGUntar( string tarLocation, bool tarIsInDocuments, string destinationWithinDocuments )
{
    string tarFullPath = ofToPath( tarLocation, tarIsInDocuments );
    if ( ofxGFileExists( tarFullPath ) )
    {
        ofxGmkdir( destinationWithinDocuments, true );
        string destinationFullPath = ofToPath( destinationWithinDocuments, true );
#if TARGET_OS_IPHONE
        NSError* error;
        return ( bool )[ [ NSFileManager defaultManager ] createFilesAndDirectoriesAtPath:ofxStringToNSString( destinationFullPath ) withTarPath:ofxStringToNSString( tarFullPath ) error:&error ];
#endif
    } else
    {
        ofxGLogError( "ofxGUntar called with path to tar file " + tarFullPath + " that cannot be found!" );
    }
    
    return false;
}


bool ofxGUnzip( string zipLocation, bool zipIsInDocuments, string destinationWithinDocuments )
{
    string zipFullPath = ofToPath( zipLocation, zipIsInDocuments );
    if ( ofxGFileExists( zipFullPath ) )
    {
        ofxGmkdir( destinationWithinDocuments, true );
        string destinationFullPath = ofToPath( destinationWithinDocuments, true );
#if TARGET_OS_IPHONE
        [ SSZipArchive unzipFileAtPath:ofxStringToNSString( zipFullPath ) toDestination:ofxStringToNSString( destinationFullPath ) ];
#endif
    } else
    {
        ofxGLogError( "ofxGUnzip called with path to zip file " + zipFullPath + " that cannot be found!" );
    }
    
    return false;
}

//////////////////////////////// Color //////////////////////////////////

ofPoint ofxGToPoint( const ofColor& color )
{
    return ofPoint(
                   ( float )color.r,
                   ( float )color.g,
                   ( float )color.b
                );
}

ofColor ofxGToColor( const ofPoint& point )
{
    return ofColor(
                   ( unsigned char )point.x,
                   ( unsigned char )point.y,
                   ( unsigned char )point.z,
                   255
                );
}
