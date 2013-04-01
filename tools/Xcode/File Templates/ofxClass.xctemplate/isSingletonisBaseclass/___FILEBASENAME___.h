//
//  ___FILENAME___
//  ___PROJECTNAME___
//
//  Created by ___FULLUSERNAME___ on ___DATE___.
//___COPYRIGHT___
//

#pragma once

#include "ofxGenericMain.h"

class ___FILEBASENAMEASIDENTIFIER___
{
public:
    static ___FILEBASENAMEASIDENTIFIER___& getInstance();
    
    virtual ~___FILEBASENAMEASIDENTIFIER___();
protected:
    
    ___FILEBASENAMEASIDENTIFIER___();
    static ofPtr< ___FILEBASENAMEASIDENTIFIER___ > _this;
    virtual void init( ofPtr< ___FILEBASENAMEASIDENTIFIER___ > setThis );
};