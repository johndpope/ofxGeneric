//
//  ___FILENAME___
//  ___PROJECTNAME___
//
//  Created by ___FULLUSERNAME___ on ___DATE___.
//___COPYRIGHT___
//

#include "___FILEBASENAME___.h"

ofPtr< ___FILEBASENAMEASIDENTIFIER___ > ___FILEBASENAMEASIDENTIFIER___::create()
{
    ofPtr< ___FILEBASENAMEASIDENTIFIER___ > create( new ___FILEBASENAMEASIDENTIFIER___() );
    create->init( create );
    return create;
}

___FILEBASENAMEASIDENTIFIER___::___FILEBASENAMEASIDENTIFIER___()
{
}

void ___FILEBASENAMEASIDENTIFIER___::init( ofPtrWeak< ___FILEBASENAMEASIDENTIFIER___ > setThis )
{
    ofxGenericClass::init( setThis );
}