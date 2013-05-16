//
//  ___FILENAME___
//  ___PROJECTNAME___
//
//  Created by ___FULLUSERNAME___ on ___DATE___.
//___COPYRIGHT___
//

#include "___FILEBASENAME___.h"

ofPtr< ___FILEBASENAMEASIDENTIFIER___ > ___FILEBASENAMEASIDENTIFIER___::create( const ofRectangle& setFrame, ofPtrWeak< ofxGenericButtonViewDelegate > touchDelegate )
{
    ofPtr< ___FILEBASENAMEASIDENTIFIER___ > create( new ___FILEBASENAMEASIDENTIFIER___() );
    create->init( create, ofxGenericButtonTypeCustom, setFrame, touchDelegate );
    return create;
}

___FILEBASENAMEASIDENTIFIER___::___FILEBASENAMEASIDENTIFIER___()
{
}