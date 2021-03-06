//
//  ofxGenericStoreTransaction.h
//  ofxGeneric
//
//  Created by Eli Delventhal on 1/31/13.
//  Copyright (c) 2013 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"
#include "ofxGenericDate.h"

#if TARGET_OS_IPHONE
#include <StoreKit/StoreKit.h>
#endif

class ofxGenericStoreTransaction
{
public:
#if TARGET_OS_IPHONE
    static ofPtr< ofxGenericStoreTransaction > create( SKPaymentTransaction *transaction, SKPaymentQueue *queue );
#endif
    virtual ~ofxGenericStoreTransaction();
    
    //necessary on iOS but not on Android, tells Apple that the account has been credited properly
    void finishTransaction();
    
    bool hasError();
    string getError();
    
    bool isPurchasing();
    bool isPurchased();
    bool hasFailed();
    bool hasBeenRestored();
    
    string getIdentifier();
    ofPtr< ofxGenericDate > getDate();
    string getProductIdentifier();
    
    string getReceipt();
    
protected:
    void init( ofPtrWeak< ofxGenericStoreTransaction > setThis );
    ofxGenericStoreTransaction();
    ofPtrWeak< ofxGenericStoreTransaction > _this;
    
#if TARGET_OS_IPHONE
    void setSKPaymentTransaction( SKPaymentTransaction *transaction );
    SKPaymentTransaction *_transaction;
    void setSKPaymentQueue( SKPaymentQueue *queue );
    SKPaymentQueue *_queue;
#endif
};