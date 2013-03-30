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
    if ( create->_this )
    {
        return create;
    }
    return ofPtr< ___FILEBASENAMEASIDENTIFIER___ >();
}

___FILEBASENAMEASIDENTIFIER___::___FILEBASENAMEASIDENTIFIER___()
{
}

string ___FILEBASENAMEASIDENTIFIER___::getFileName()
{
    // TODO
}