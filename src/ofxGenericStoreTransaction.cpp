//
//  ofxGenericStoreTransaction.cpp
//  ofxGeneric
//
//  Created by Eli Delventhal on 1/31/13.
//  Copyright (c) 2013 Lumos Labs. All rights reserved.
//

#include "ofxGenericStoreTransaction.h"

#if TARGET_OS_IPHONE

ofPtr< ofxGenericStoreTransaction > ofxGenericStoreTransaction::create( SKPaymentTransaction *transaction )
{
    ofPtr< ofxGenericStoreTransaction > create = ofPtr< ofxGenericStoreTransaction > ( new ofxGenericStoreTransaction() );
    create->init( create );
    create->setSKPaymentTransaction( transaction );
    return create;
}

void ofxGenericStoreTransaction::setSKPaymentTransaction( SKPaymentTransaction *transaction )
{
    _transaction = [ transaction retain ];
}

#endif

ofxGenericStoreTransaction::~ofxGenericStoreTransaction()
{
    [ _transaction release ];
    _transaction = nil;
}

ofxGenericStoreTransaction::ofxGenericStoreTransaction()
#if TARGET_OS_IPHONE
: _transaction( nil )
#endif
{
    
}

void ofxGenericStoreTransaction::init( ofPtrWeak< ofxGenericStoreTransaction > setThis )
{
    _this = setThis;
}

bool ofxGenericStoreTransaction::hasError()
{
#if TARGET_OS_IPHONE
    return !!( _transaction.error );
#else
    return false;
#endif
}

string ofxGenericStoreTransaction::getError()
{
#if TARGET_OS_IPHONE
    if ( _transaction.error )
    {
        return [ _transaction.error.description cStringUsingEncoding:NSUTF8StringEncoding ];
    }
    return "";
#else
    return "";
#endif
}

bool ofxGenericStoreTransaction::isPurchasing()
{
#if TARGET_OS_IPHONE
    return _transaction.transactionState == SKPaymentTransactionStatePurchasing;
#else
    return false;
#endif
}

bool ofxGenericStoreTransaction::isPurchased()
{
#if TARGET_OS_IPHONE
    return _transaction.transactionState == SKPaymentTransactionStatePurchased;
#else
    return false;
#endif
}

bool ofxGenericStoreTransaction::hasFailed()
{
#if TARGET_OS_IPHONE
    return _transaction.transactionState == SKPaymentTransactionStateFailed;
#else
    return false;
#endif
}

bool ofxGenericStoreTransaction::hasBeenRestored()
{
#if TARGET_OS_IPHONE
    return _transaction.transactionState == SKPaymentTransactionStateRestored;
#else
    return false;
#endif
}

string ofxGenericStoreTransaction::getIdentifier()
{
#if TARGET_OS_IPHONE
    return [ _transaction.transactionIdentifier cStringUsingEncoding:NSUTF8StringEncoding ];
#else
    return "";
#endif
}

string ofxGenericStoreTransaction::getProductIdentifier()
{
#if TARGET_OS_IPHONE
    return [ _transaction.payment.productIdentifier cStringUsingEncoding:NSUTF8StringEncoding ];
#else
    return "";
#endif
}

ofPtr< ofxGenericDate > ofxGenericStoreTransaction::getDate()
{
#if TARGET_OS_IPHONE
    return ofxGenericDate::createFromNSDate( _transaction.transactionDate );
#else
    return ofPtr< ofxGenericDate > ();
#endif
}

string ofxGenericStoreTransaction::getReceipt()
{
#if TARGET_OS_IPHONE
    return string ( [ [ [ [ NSString alloc ] initWithData:_transaction.transactionReceipt encoding:NSUTF8StringEncoding ] autorelease ] cStringUsingEncoding:NSUTF8StringEncoding ] );
#else
    return "";
#endif
}
