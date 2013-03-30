//
//  ___FILENAME___
//  ___PROJECTNAME___
//
//  Created by ___FULLUSERNAME___ on ___DATE___.
//___COPYRIGHT___
//

#include "___FILEBASENAME___.h"

___VARIABLE_subclassOf___::Type ___FILEBASENAMEASIDENTIFIER___::getValueStoreType()
{
    return ___VARIABLE_subclassOf___::ofxGenericValueStoreType___VARIABLE_valueStoreType___;
}

ofPtr< ___FILEBASENAMEASIDENTIFIER___ > ___FILEBASENAMEASIDENTIFIER___::create()
{
    ofPtr< ___FILEBASENAMEASIDENTIFIER___ > create( new ___FILEBASENAMEASIDENTIFIER___() );
    create->init( create, ___FILEBASENAMEASIDENTIFIER___::getValueStoreType() );
    if ( create->_this )
    {
        return create;
    }
    return ofPtr< ___FILEBASENAMEASIDENTIFIER___ >();
}

ofPtr< ___FILEBASENAMEASIDENTIFIER___ > ___FILEBASENAMEASIDENTIFIER___::create( ofPtr< ___VARIABLE_subclassOf___ > from )
{
    ofPtr< ___FILEBASENAMEASIDENTIFIER___ > create;
    if ( from )
    {
        if ( from->getType() == ___FILEBASENAMEASIDENTIFIER___::getValueStoreType() )
        {
            create = ___FILEBASENAMEASIDENTIFIER___::create();
            if ( create )
            {
                create->write( from );
            }
        } else
        {
            ofxGLogError( "Unable to ___FILEBASENAMEASIDENTIFIER___::create( from ), from is not of required type ofxGenericValueStoreType___VARIABLE_valueStoreType___" );
        }
    }
    return create;
}

ofPtr< ___FILEBASENAMEASIDENTIFIER___ > ___FILEBASENAMEASIDENTIFIER___::createIfNot( ofPtr< ___VARIABLE_subclassOf___ > from )
{
    ofPtr< ___FILEBASENAMEASIDENTIFIER___ > createTyped;
    if ( from )
    {
        createTyped = dynamic_pointer_cast< ___FILEBASENAMEASIDENTIFIER___ >( from );
        if ( !createTyped )
        {
            createTyped = create( from );
        }
    }
    return createTyped;
}

___FILEBASENAMEASIDENTIFIER___::___FILEBASENAMEASIDENTIFIER___()
{
}