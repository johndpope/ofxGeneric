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
#include "ofxGenericUtility.h"
#if TARGET_OS_IPHONE
#include "ofxGenericUtilityiOS.h"
#endif

#import "ofCommon.h"

std::vector< ofPtrWeak< ofxGenericStorePurchaser > > ofxGenericStorePurchaser::_allPurchasers;

ofxGenericStorePurchaser::~ofxGenericStorePurchaser()
{
#if TARGET_OS_IPHONE
    [ forwarder release ];
    forwarder = nil;
#endif
}

ofxGenericStorePurchaser::ofxGenericStorePurchaser() : _isFindingProducts( false ), _isObservingPurchases( false )
#if TARGET_OS_IPHONE
, forwarder( nil )
#endif
{
    
}

void ofxGenericStorePurchaser::init( ofPtrWeak< ofxGenericStorePurchaser > setThis )
{
    _this = setThis;
    
    forwarder = [ [ ofxGenericInAppPurchaseForwarder alloc ] initWithPurchaser:_this.lock() ];
}

//pass in a list of products we want to use
ofPtr< ofxGenericStorePurchaser > ofxGenericStorePurchaser::create( std::vector< string > products, ofPtrWeak< ofxGenericStorePurchaserDelegate > delegate )
{
    ofPtr< ofxGenericStorePurchaser > create = ofPtr< ofxGenericStorePurchaser >( new ofxGenericStorePurchaser() );
    create->init( create );
    create->setDelegate( delegate );
    if ( products.size() > 0 )
    {
        create->findProducts( products );
    }
    _allPurchasers.push_back( create );
    return create;
}

void ofxGenericStorePurchaser::beginObserving()
{
    if (_isObservingPurchases) {
        return;
    }
#if TARGET_OS_IPHONE
    [[SKPaymentQueue defaultQueue] addTransactionObserver:forwarder];
#else
#endif // TARGET_OS_IPHONE
    _isObservingPurchases = YES;
}

void ofxGenericStorePurchaser::findProducts( std::vector< string > products )
{
    if ( _isFindingProducts )
    {
        return;
    }
    _isFindingProducts = true;
    
#if TARGET_OS_IPHONE
    NSMutableSet* productIdentifiers = [ NSMutableSet set ];
    for ( unsigned int i = 0; i < products.size(); i++ )
    {
        [ productIdentifiers addObject: ofxStringToNSString( products[i] ) ];
    }
    SKProductsRequest* productsRequest = [ [ [ SKProductsRequest alloc ] initWithProductIdentifiers:productIdentifiers ] autorelease ];
    productsRequest.delegate = forwarder;
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
    
    ofPtr< ofxGenericStoreProduct > product = _products[ identifier ];
    if ( product )
    {
        SKPayment *payment = [ SKPayment paymentWithProductIdentifier: ofxStringToNSString( product->getIdentifier() )  ];
        [ [ SKPaymentQueue defaultQueue ] addPayment:payment ];
    }
#endif
}

void ofxGenericStorePurchaser::restorePurchases()
{
#if TARGET_OS_IPHONE
    
    [ [ SKPaymentQueue defaultQueue ] restoreCompletedTransactions ];
#endif
}

void ofxGenericStorePurchaser::finishAllTransactions()
{
#if TARGET_OS_IPHONE
    
    if ( [ [ SKPaymentQueue defaultQueue ].transactions count ] <= 0 )
    {
        return;
    }
    
    //if we still have transactions that are still being purchased, make sure to finish them by correctly crediting the user etc.
    for ( unsigned int i = 0; i < [ [ SKPaymentQueue defaultQueue ].transactions count ]; i++ )
    {
        SKPaymentTransaction *transaction = [ [ SKPaymentQueue defaultQueue ].transactions objectAtIndex:i ];
        [forwarder processTransaction:transaction queue:[SKPaymentQueue defaultQueue]];
    }
#endif
}


void ofxGenericStorePurchaser::finishPurchase( ofPtr< ofxGenericStoreTransaction > transaction )
{
    transaction->finishTransaction();
}

bool ofxGenericStorePurchaser::paymentsCanBeMade()
{
#if TARGET_OS_IPHONE
    return [ SKPaymentQueue canMakePayments ] == YES ? true : false;
#else
    return false;
#endif
}

#pragma mark Error Handling

void ofxGenericStorePurchaser::errorFetchingProducts(NSError *error)
{
    _isFindingProducts = false;
    if (error.localizedDescription.length > 0) {
        // IOSP-40 // ofLogError("Error in ofxGenericStorePurchaser: " + (string)error.localizedDescription.UTF8String);
    } else {
        // IOSP-40 // ofLogError("Error in ofxGenericStorePurchaser");
    }
    
    if ( _delegate )
    {
        _delegate.lock()->inApp_failedToFetchProducts(error);
    }
}

#pragma mark Product Related Callbacks

void ofxGenericStorePurchaser::productsResponseReceived( std::map< string, ofPtr< ofxGenericStoreProduct > > products, std::vector< string > identifiers )
{
    _isFindingProducts = false;
    _products = products;
    if ( _delegate )
    {
        _delegate.lock()->inApp_productsReceived( identifiers );
    }
}

#pragma mark - Payment Related Callbacks

void ofxGenericStorePurchaser::paymentReceived( ofPtr< ofxGenericStoreTransaction > transaction )
{
    // IOSP-40 // ofLogVerbose("ofxGenericStorePurchaser: paymentReceived - " + transaction->getProductIdentifier() );
    if ( _delegate )
    {
        _delegate.lock()->inApp_purchaseComplete( transaction->getProductIdentifier(), transaction );
    }
}

void ofxGenericStorePurchaser::paymentFailed( ofPtr< ofxGenericStoreTransaction > transaction, NSError *error )
{
    if (error.localizedDescription.length > 0) {
        // IOSP-40 // ofLogError("ofxGenericStorePurchaser: paymentFailed - " + (string)error.localizedDescription.UTF8String );
    } else {
        // IOSP-40 // ofLogError("ofxGenericStorePurchaser: paymentFailed");
    }
    if ( _delegate )
    {
        _delegate.lock()->inApp_purchaseFailed(transaction, error);
    }
    
    finishPurchase(transaction);
}

void ofxGenericStorePurchaser::paymentRestored( ofPtr< ofxGenericStoreTransaction > transaction )
{
    // IOSP-40 // ofLogVerbose("ofxGenericStorePurchaser: paymentRestored - " + transaction->getProductIdentifier() );
    if ( _delegate )
    {
        _delegate.lock()->inApp_purchaseRestored( transaction->getProductIdentifier(), transaction );
    }
}


void ofxGenericStorePurchaser::setDelegate( ofPtrWeak< ofxGenericStorePurchaserDelegate > delegate )
{
    _delegate = delegate;
}

#if TARGET_OS_IPHONE
#pragma mark - ofxGenericInAppPurchaseForwarder Implementation
@implementation ofxGenericInAppPurchaseForwarder

- (id) initWithPurchaser: ( ofPtrWeak< ofxGenericStorePurchaser > ) purchaser
{
    if ( self = [ self init ] )
    {
        _purchaser = purchaser;
        return self;
    }
    return nil;
}

- (void)productsRequest:(SKProductsRequest*)request didReceiveResponse:(SKProductsResponse *)response
{
    if ( !_purchaser )
    {
        return;
    }
    
    std::map< string, ofPtr< ofxGenericStoreProduct > > products = std::map< string, ofPtr< ofxGenericStoreProduct > >();
    std::vector< string > identifiers = std::vector< string >();
    
    for ( unsigned int i = 0; i < [ response.products count ]; i++ )
    {
        ofPtr< ofxGenericStoreProduct > product = ofxGenericStoreProduct::create( [ response.products objectAtIndex:i ] );
        string identifier = product->getIdentifier();
        products[ identifier ] = product;
        identifiers.push_back( identifier );
        // IOSP-40 // ofLogVerbose(" ofxGenericStorePurchaser: Recieved product: " + identifier );
    }
    
    for ( unsigned int i = 0; i < [ response.invalidProductIdentifiers count ]; i++ )
    {
        //NSString *invalidId = [ response.invalidProductIdentifiers objectAtIndex:i ];
        // IOSP-40 // ofLogWarning( "ofxGenericStorePurchaser: Apple returned invalid product identifier from products request: \"" + ofxGToString( invalidId ) + "\"" );
    }
    
    _purchaser.lock()->productsResponseReceived( products, identifiers );
}

- (void)request:(SKRequest*)request didFailWithError:(NSError*)error
{
    if ( !_purchaser )
    {
        return;
    }
    
    _purchaser.lock()->errorFetchingProducts(error);
 
}

- (void) paymentQueue:(SKPaymentQueue *)queue updatedTransactions: (NSArray * )transactions
{
    if ( !_purchaser )
    {
        return;
    }
    
    for ( SKPaymentTransaction* transaction in transactions )
    {
        [self processTransaction:transaction queue:queue];
    }
}

- (void)processTransaction:(SKPaymentTransaction *)transaction queue:(SKPaymentQueue *)queue;
{
    switch ( transaction.transactionState )
    {
        case SKPaymentTransactionStatePurchasing:
            break;
        case SKPaymentTransactionStatePurchased:
            _purchaser.lock()->paymentReceived( ofxGenericStoreTransaction::create( transaction, queue ) );
            break;
        case SKPaymentTransactionStateFailed:
            _purchaser.lock()->paymentFailed(ofxGenericStoreTransaction::create( transaction, queue ), transaction.error);
            break;
        case SKPaymentTransactionStateRestored:
            _purchaser.lock()->paymentRestored( ofxGenericStoreTransaction::create( transaction, queue ) );
            break;
     
        default:
            break;
    }
}

- (void) paymentQueue:(SKPaymentQueue *)queue updatedDownloads:(NSArray *)downloads
{
    //not sure if I need to do anything in here, it's just a required method
}

@end

#endif // TARGET_OS_IPHONE
