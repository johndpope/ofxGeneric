//
//  ofxGenericStorePurchaser.h
//  ofxGeneric
//
//  There are two major steps to payments:
//  1) Send our product IDs to the app store and request back the actual products. Then display the products..
//  2) After the user has chosen to purchase a product, send a payment request to the app store.
//
//  Specific steps are:
//  1) Get the app IDs from either the server or just a file somewhere.
//  2) Send the IDs to the app store
//  3) The app store sends back products
//  4) Get the products and display the store page to the user with them
//  5) The user chooses to purchase an item
//  6) Send a payment request to the app store
//  7) The app store processes the payment, then sends back a transaction receipt
//  8) Get the transaction receipt and send it to the server in order to give credit
//  9) The server gives credit, sends an OK back to the client
//  10) The client sends a completed transaction call back to the app store to wrap things up
//
//  Created by Eli Delventhal on 1/22/13.
//  Copyright (c) 2013 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"

class ofxGenericStoreProduct;
class ofxGenericStoreTransaction;

#if TARGET_OS_IPHONE
#import <StoreKit/StoreKit.h>

class ofxGenericStorePurchaser;
@interface ofxGenericInAppPurchaseForwarder : NSObject < SKProductsRequestDelegate, SKPaymentTransactionObserver >
{
    ofPtrWeak< ofxGenericStorePurchaser > _purchaser;
    SKProductsRequest* productsRequest;
    BOOL _isTransactionObserver;
}
@property (retain) SKProductsRequest* productsRequest;
- (id) initWithPurchaser: ( ofPtrWeak< ofxGenericStorePurchaser > ) purchaser;
- (void) makeTransactionObserver;
- (void) removeAsTransactionObserver;
@end

#endif

class ofxGenericStorePurchaserDelegate;

class ofxGenericStorePurchaser
{
public:
    virtual ~ofxGenericStorePurchaser();
    //pass in a list of products we want to use
    static ofPtr< ofxGenericStorePurchaser > create( std::vector< string > products, ofPtrWeak< ofxGenericStorePurchaserDelegate > delegate = ofPtrWeak< ofxGenericStorePurchaserDelegate >() );
    
    //when the app is foregrounded, we should finish all transactions for all purchasers
    static void doForegroundedWork();
    
    //requests the NativeStoreProducts from the app store using the passed IDs
    void findProducts( std::vector< string > products );
    
    //returns a specific product, as found by its ID... use this for getting details
    ofPtr< ofxGenericStoreProduct > getStoreProduct( string identifier );
    
    //performs an actual purchase of a product, careful, only do this if the user hit a button
    void purchaseProduct( string identifier );
    
    //not necessary on Android, on iOS this is called to tell Apple that you processed the transaction successfully
    void finishPurchase( ofPtr< ofxGenericStoreTransaction > transaction );
    
    //don't think we have anything that needs to be restored, but just in case...
    void restorePurchases();
    
    //call this when coming in from sleep, etc. because we want to make sure any unresolved transactions are dealt with
    void finishAllTransactions();
    
    //the user may be able to turn off IAP, make sure payments are okay
    bool paymentsCanBeMade();
    
    //called by forwarder
    void productsResponseReceived( std::map< string, ofPtr< ofxGenericStoreProduct > > products, std::vector< string > identifiers );
    void paymentReceived( ofPtr< ofxGenericStoreTransaction > transaction );
    void paymentFailed( ofPtr< ofxGenericStoreTransaction > transaction, string error );
    void paymentRestored( ofPtr< ofxGenericStoreTransaction > transaction );
    void errorReceived( string error );
    
    void setDelegate( ofPtrWeak< ofxGenericStorePurchaserDelegate > delegate );
    
protected:
    ofxGenericStorePurchaser();
    void init( ofPtrWeak< ofxGenericStorePurchaser > setThis );
    ofPtrWeak< ofxGenericStorePurchaser > _this;
    
    std::map< string, ofPtr< ofxGenericStoreProduct > > _products;
    
    ofPtrWeak< ofxGenericStorePurchaserDelegate > _delegate;
    
    static std::vector< ofPtrWeak< ofxGenericStorePurchaser > > _allPurchasers;
    
#if TARGET_OS_IPHONE
    ofxGenericInAppPurchaseForwarder *forwarder;
#endif
};

class ofxGenericStorePurchaserDelegate
{
public:
    virtual ~ofxGenericStorePurchaserDelegate() {};
    virtual void inApp_productsReceived( std::vector< string > products ) {};
    virtual void inApp_purchaseComplete( string identifier, ofPtr< ofxGenericStoreTransaction > transaction ) {};
    virtual void inApp_purchaseRestored( string identifier, ofPtr< ofxGenericStoreTransaction > transaction ) {};
    virtual void inApp_purchaseFailed( string identifier, ofPtr< ofxGenericStoreTransaction > transaction, string error ) {};
};