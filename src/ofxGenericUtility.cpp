#include "ofxGenericUtility.h"
#include "ofLog.h"
#include "ofxGenericConstants.h"

#include <sys/stat.h>
#include <sys/types.h>

string ofxGPathToDataFolder( string fileName )
{
    return ofToDataPath( fileName, true, false );
}

string ofxGPathToDocumentsFolder( string fileName )
{
    return ofToDataPath( fileName, true, false );
}

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

int ofxRandomInRange( int minimum, int maximum )
{
    return minimum + ( rand() % ( int )( maximum - minimum + 1 ) );
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