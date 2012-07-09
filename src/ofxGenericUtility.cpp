#include "ofxGenericUtility.h"
#include "ofLog.h"
#include "ofxGenericConstants.h"

#include <sys/stat.h>
#include <sys/types.h>

//////////////////////////////// Path //////////////////////////////////

string ofxGPathToDataFolder( string fileName )
{
    return ofToDataPath( fileName, true, false );
}

string ofxGPathToDocumentsFolder( string fileName )
{
    return ofToDataPath( fileName, true, false );
}

string ofxGGetPathFromFileName( string fileName )
{
    int slashPos = fileName.find_last_of( "/\\" );
    if ( slashPos != fileName.npos )
    {
        return fileName.substr( 0, slashPos );
    }
    return "";
}

int ofxGmkdir( string loc, bool useDocuments )
{
    if ( !loc.empty() )
    {
        string filename = ofToDataPath( loc, true, useDocuments );
        //http://pubs.opengroup.org/onlinepubs/009695399/functions/mkdir.html
        return mkdir( filename.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ); //wtf omg wow I hate C++
    }
    return 0;
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
    CGFloat height = [ nsText sizeWithFont:font constrainedToSize:CGSizeMake( constrainedSize.x, FLT_MAX ) lineBreakMode:UILineBreakModeWordWrap ].height;
    UIFont *newFont = font;
    
    //Reduce font size while too large, break if no height (empty string)
    while ( height > constrainedSize.y && height != 0 ) 
    {   
        fontSize--;  
        newFont = [ UIFont fontWithName:nsFontName size:fontSize ];   
        height = [ nsText sizeWithFont:newFont constrainedToSize:CGSizeMake( constrainedSize.x, FLT_MAX ) lineBreakMode:UILineBreakModeWordWrap ].height;
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
    CGSize size = [ nsText sizeWithFont:font constrainedToSize:CGSizeMake( constrainedWidth, FLT_MAX ) lineBreakMode:UILineBreakModeWordWrap ];
    return ofPoint( size.width, size.height );
#else
    return ofPoint( 0, 0 );
#endif
}

//////////////////////////////// Logging //////////////////////////////////

void ofxGLog( ofLogLevel level, const string & message )
{
	ofLog( ofxGenericModuleName, level, message );
}

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

void ofxGLog( ofLogLevel level, const char* format, ... )
{
	va_list args;
	va_start( args, format );
	string message = ofVAArgsToString( format, args );
	va_end( args );
	ofLog( ofxGenericModuleName, level, message );
}

//////////////////////////////// String conversions //////////////////////////////////

string ofxGIntegerToString( int value )
{
    char buffer[ 1024 ];
    snprintf( buffer, 1024, "%d", value );
    return string( buffer );
}

string ofxGFloatToString( float value )
{
    char buffer[ 1024 ];
    snprintf( buffer, 1024, "%f", value );
    return string( buffer );
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
            ofxGLogError( "ofxSPrintf called expecting more replacement strings" );
        }

        start = foundAt + replace.size();
        replaceWithIndex ++;
    }
    if ( start < format.size() - 1 )
    {
        result += format.substr( start, format.size() - start );
    }
    
    if ( replaceWithIndex != replaceWith.size() )
    {
        ofxGLogError( "ofxSPrintf called with more replacement strings than format calls for" );
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

//////////////////////////////// Math //////////////////////////////////

int ofxRandomInRange( int minimum, int maximum )
{
    return minimum + ( rand() % ( int )( maximum - minimum + 1 ) );
}

