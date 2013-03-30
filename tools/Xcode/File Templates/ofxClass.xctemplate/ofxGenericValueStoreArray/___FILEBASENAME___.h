//
//  ___FILENAME___
//  ___PROJECTNAME___
//
//  Created by ___FULLUSERNAME___ on ___DATE___.
//___COPYRIGHT___
//

#pragma once

#include "___VARIABLE_subclassOf___.h"

class ___FILEBASENAMEASIDENTIFIER___ : public ___VARIABLE_subclassOf___
{
public:
    static ofPtr< ___FILEBASENAMEASIDENTIFIER___ > create();
    static ofPtr< ___FILEBASENAMEASIDENTIFIER___ > create( ofPtr< ___VARIABLE_subclassOf___ > from );
    static ofPtr< ___FILEBASENAMEASIDENTIFIER___ > createIfNot( ofPtr< ___VARIABLE_subclassOf___ > from );
    
protected:
    static ___VARIABLE_subclassOf___::Type getValueStoreType();
    ___FILEBASENAMEASIDENTIFIER___();
};