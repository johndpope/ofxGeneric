//
//  ofxGenericStorePurchaser.cpp
//  ofxGeneric
//
//  Created by Eli Delventhal on 1/22/13.
//  Copyright (c) 2013 Lumos Labs. All rights reserved.
//

#include "ofxGenericStorePurchaser.h"
#include "ofxGenericStoreProduct.h"
#include "ofxGenericStoreTransaction.h"

ofxGenericStorePurchaser::~ofxGenericStorePurchaser()
{
#if TARGET_OS_IPHONE
    [ forwarder release ];
    forwarder = nil;
#endif
}

ofxGenericStorePurchaser::ofxGenericStorePurchaser()
#if TARGET_OS_IPHONE
: forwarder( nil )
#endif
{
    
}

void ofxGenericStorePurchaser::init( ofPtrWeak< ofxGenericStorePurchaser > setThis )
{
    _this = setThis;
}

//pass in a list of products we want to use
ofPtr< ofxGenericStorePurchaser > ofxGenericStorePurchaser::create( std::vector< string > products, ofPtrWeak< ofxGenericStorePurchaserDelegate > delegate )
{
    ofPtr< ofxGenericStorePurchaser > create = ofPtr< ofxGenericStorePurchaser >( new ofxGenericStorePurchaser() );
    create->init( create );
    create->setDelegate( delegate );
    create->findProducts( products );
    return create;
}

void ofxGenericStorePurchaser::findProducts( std::vector< string > products )
{
#if TARGET_OS_IPHONE
    if ( !forwarder )
    {
        forwarder = [ [ ofxGenericInAppPurchaseForwarder alloc ] initWithPurchaser:_this.lock() ];
    }
    NSMutableSet* productIdentifiers = [ NSMutableSet set ];
    for ( unsigned int i = 0; i < products.size(); i++ )
    {
        [ productIdentifiers addObject:[ NSString stringWithCString:products[i].c_str() encoding:NSUTF8StringEncoding ] ];
    }
    SKProductsRequest* productsRequest = [ [ [ SKProductsRequest alloc ] initWithProductIdentifiers:productIdentifiers ] autorelease ];
    productsRequest.delegate = forwarder;
    forwarder.productsRequest = productsRequest;
    [ productsRequest start ];
#endif
}

ofPtr< ofxGenericStoreProduct > ofxGenericStorePurchaser::getStoreProduct( string identifier )
{
    return _products[ identifier ];
}

//performs an actual purchase of a product, careful, only do this if the user hit a button
void ofxGenericStorePurchaser::purchaseProduct( string identifier )
{
#if TARGET_OS_IPHONE
    if ( !forwarder )
    {
        forwarder = [ [ ofxGenericInAppPurchaseForwarder alloc ] initWithPurchaser:_this.lock() ];
    }
    
    ofPtr< ofxGenericStoreProduct > product = _products[ identifier ];
    if ( product )
    {
        SKPayment *payment = [ SKPayment paymentWithProductIdentifier: [ NSString stringWithCString:product->getIdentifier().c_str() encoding:NSUTF8StringEncoding ]  ];
        [ [ SKPaymentQueue defaultQueue ] addTransactionObserver:forwarder ];
        [ [ SKPaymentQueue defaultQueue ] addPayment:payment ];
    }
#endif
}

void ofxGenericStorePurchaser::productsResponseReceived( std::map< string, ofPtr< ofxGenericStoreProduct > > products, std::vector< string > identifiers )
{
    if ( _delegate )
    {
        _delegate.lock()->inApp_productsReceived( identifiers );
    }
}

void ofxGenericStorePurchaser::paymentReceived( ofPtr< ofxGenericStoreTransaction > transaction )
{
    if ( _delegate )
    {
        _delegate.lock()->inApp_purchaseComplete( transaction->getProductIdentifier(), transaction );
    }
}

void ofxGenericStorePurchaser::paymentFailed( ofPtr< ofxGenericStoreTransaction > transaction, string error )
{
    if ( _delegate )
    {
        _delegate.lock()->inApp_purchaseFailed( transaction->getProductIdentifier(), error );
    }
}

void ofxGenericStorePurchaser::paymentRestored( ofPtr< ofxGenericStoreTransaction > transaction )
{
    if ( _delegate )
    {
        _delegate.lock()->inApp_purchaseRestored( transaction->getProductIdentifier(), transaction );
    }
}

void ofxGenericStorePurchaser::errorReceived( string error )
{
    ofLogError("Error in ofxGenericStorePurchaser: " + error );
}

void ofxGenericStorePurchaser::setDelegate( ofPtrWeak< ofxGenericStorePurchaserDelegate > delegate )
{
    _delegate = delegate;
}

#if TARGET_OS_IPHONE
@implementation ofxGenericInAppPurchaseForwarder

@synthesize productsRequest;

- (id) initWithPurchaser: ( ofPtrWeak< ofxGenericStorePurchaser > ) purchaser
{
    if ( self = [ self init ] )
    {
        _purchaser = purchaser;
        return self;
    }
    return nil;
}

- (void) productsRequest:(SKProductsRequest*)request didReceiveResponse:(SKProductsResponse *)response
{
    std::map< string, ofPtr< ofxGenericStoreProduct > > products = std::map< string, ofPtr< ofxGenericStoreProduct > >();
    std::vector< string > identifiers = std::vector< string >();
    
    for ( unsigned int i = 0; i < [ response.products count ]; i++ )
    {
        ofPtr< ofxGenericStoreProduct > product = ofxGenericStoreProduct::create( [ response.products objectAtIndex:i ] );
        string identifier = product->getIdentifier();
        products[ identifier ] = product;
        identifiers.push_back( identifier );
    }
    
    _purchaser.lock()->productsResponseReceived( products, identifiers );
}

- (void) request:(SKRequest*)request didFailWithError:(NSError*)error
{
    _purchaser.lock()->errorReceived( string ( [ error.description cStringUsingEncoding:NSUTF8StringEncoding ] ) );
}

- (void) paymentQueue:(SKPaymentQueue *)queue updatedTransactions: (NSArray * )transactions
{
    for ( SKPaymentTransaction* transaction in transactions )
    {
        switch ( transaction.transactionState )
        {
            case SKPaymentTransactionStatePurchased:
                _purchaser.lock()->paymentReceived( ofxGenericStoreTransaction::create( transaction ) );
                break;
            case SKPaymentTransactionStateFailed:
                _purchaser.lock()->paymentFailed( ofxGenericStoreTransaction::create( transaction ), string ( [ transaction.error.description cStringUsingEncoding:NSUTF8StringEncoding ] ) );
                break;
            case SKPaymentTransactionStateRestored:
                _purchaser.lock()->paymentRestored( ofxGenericStoreTransaction::create( transaction ) );
                break;
            default:
                break;
        }
    }
    
    [ [ SKPaymentQueue defaultQueue ] removeTransactionObserver:self ];
}

- (void) paymentQueue:(SKPaymentQueue *)queue updatedDownloads:(NSArray *)downloads
{
    //not sure if I need to do anything in here, it's just a required method
}

-(void) dealloc
{
    self.productsRequest = nil;
    
    [ super dealloc ];
}

@end
#endif