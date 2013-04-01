//
//  ___FILENAME___
//  ___PROJECTNAME___
//
//  Created by ___FULLUSERNAME___ on ___DATE___.
//___COPYRIGHT___
//

#include "___FILEBASENAME___.h"
#include "Generated___FILEBASENAMEASIDENTIFIER___.h"
#include "Generated___FILEBASENAMEASIDENTIFIER____iPad.h"
#include "ofxGenericPlatform.h"

ofPtr< ___FILEBASENAMEASIDENTIFIER___ > ___FILEBASENAMEASIDENTIFIER___::create( const ofRectangle& setFrame )
{
    ofPtr< ___FILEBASENAMEASIDENTIFIER___ > create( new ___FILEBASENAMEASIDENTIFIER___() );
    create->init( create, setFrame );
    return create;
}

void ___FILEBASENAMEASIDENTIFIER___::didLoad()
{
	___VARIABLE_subclassOf___::didLoad();
	
	if ( ofxGenericPlatform::isTablet() )
	{
		populatePreservedGenerated___FILEBASENAMEASIDENTIFIER____iPad(
				_this.lock(),
				true,
				true
				);
	}
	else
	{
		populatePreservedGenerated___FILEBASENAMEASIDENTIFIER___(
				_this.lock(),
				true,
				true
				);
	}
}

___FILEBASENAMEASIDENTIFIER___::___FILEBASENAMEASIDENTIFIER___()
{
}