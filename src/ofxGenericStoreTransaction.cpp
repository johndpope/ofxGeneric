//
//  ofxGenericStoreTransaction.cpp
//  ofxGeneric
//
//  Created by Eli Delventhal on 1/31/13.
//  Copyright (c) 2013 Lumos Labs. All rights reserved.
//

#include "ofxGenericStoreTransaction.h"
#include "ofxGenericUtility.h"

#if TARGET_OS_IPHONE
#import "NSString+Base64.h"
#endif

#if TARGET_OS_IPHONE

ofPtr< ofxGenericStoreTransaction > ofxGenericStoreTransaction::create( SKPaymentTransaction *transaction, SKPaymentQueue *queue )
{
    ofPtr< ofxGenericStoreTransaction > create = ofPtr< ofxGenericStoreTransaction > ( new ofxGenericStoreTransaction() );
    create->init( create );
    create->setSKPaymentTransaction( transaction );
    create->setSKPaymentQueue( queue );
    return create;
}

void ofxGenericStoreTransaction::setSKPaymentTransaction( SKPaymentTransaction *transaction )
{
    _transaction = [ transaction retain ];
}

void ofxGenericStoreTransaction::setSKPaymentQueue( SKPaymentQueue *queue )
{
    _queue = [ queue retain ];
}

#endif

ofxGenericStoreTransaction::~ofxGenericStoreTransaction()
{
    [ _transaction release ];
    _transaction = nil;
    [ _queue release ];
    _queue = nil;
}

ofxGenericStoreTransaction::ofxGenericStoreTransaction()
#if TARGET_OS_IPHONE
: _transaction( nil ), _queue( nil )
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
        return ofxGToString( _transaction.error.description );
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
    return ofxGToString( _transaction.transactionIdentifier );
#else
    return "";
#endif
}

string ofxGenericStoreTransaction::getProductIdentifier()
{
#if TARGET_OS_IPHONE
    return ofxGToString( _transaction.payment.productIdentifier );
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
    //TODO perhaps cache this
    NSString *base64 = [ NSString base64StringFromData:_transaction.transactionReceipt length:-1 ];
    string strBase64 = ofxGToString( base64 );
    ofLogVerbose( "Transaction Raw:\n" + ofxGToString( _transaction.transactionReceipt ) );
    ofLogVerbose( "Transaction Base64:\n" + strBase64 );
    return strBase64;
    
    //uncomment if we needed to return raw instead
    //return ofxGToString( _transaction.transactionReceipt );
#else
    return "";
#endif
}

void ofxGenericStoreTransaction::finishTransaction()
{
#if TARGET_OS_IPHONE
    //I have a suspicion the default queue doesn't always work, so deal with that case
    if ( _queue )
    {
        [ _queue finishTransaction:_transaction ];
    }
    else
    {
        [ [ SKPaymentQueue defaultQueue ] finishTransaction:_transaction ];
    }
#else
    
#endif
}
