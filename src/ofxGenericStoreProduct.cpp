//
//  ofxGenericStoreProduct.cpp
//  ofxGeneric
//
//  Created by Eli Delventhal on 1/31/13.
//  Copyright (c) 2013 Lumos Labs. All rights reserved.
//

#include "ofxGenericStoreProduct.h"

#if TARGET_OS_IPHONE
    
ofPtr< ofxGenericStoreProduct > ofxGenericStoreProduct::create( SKProduct *product )
{
    ofPtr< ofxGenericStoreProduct > create = ofPtr< ofxGenericStoreProduct > ( new ofxGenericStoreProduct() );
    create->init( create );
    create->setSKProduct( product );
    return create;
}

void ofxGenericStoreProduct::setSKProduct( SKProduct *product )
{
    _product = [ product retain ];
}

#endif

ofxGenericStoreProduct::~ofxGenericStoreProduct()
{
    [ _product release ];
    _product = nil;
}

ofxGenericStoreProduct::ofxGenericStoreProduct()
#if TARGET_OS_IPHONE
: _product( nil )
#endif
{
    
}

void ofxGenericStoreProduct::init( ofPtrWeak< ofxGenericStoreProduct > setThis )
{
    _this = setThis;
}

string ofxGenericStoreProduct::getIdentifier()
{
#if TARGET_OS_IPHONE
    return [ _product.productIdentifier cStringUsingEncoding:NSUTF8StringEncoding ];
#else
    return "";
#endif
}

string ofxGenericStoreProduct::getDescription()
{
#if TARGET_OS_IPHONE
    return [ _product.localizedDescription cStringUsingEncoding:NSUTF8StringEncoding ];
#else
    return "";
#endif
}

string ofxGenericStoreProduct::getTitle()
{
#if TARGET_OS_IPHONE
    return [ _product.localizedTitle cStringUsingEncoding:NSUTF8StringEncoding ];
#else
    return "";
#endif
}

string ofxGenericStoreProduct::getLocalizedPrice()
{
#if TARGET_OS_IPHONE
    NSNumberFormatter *formatter = [ [ NSNumberFormatter alloc ] init ];
    [ formatter setFormatterBehavior:NSNumberFormatterBehavior10_4 ];
    [ formatter setNumberStyle:NSNumberFormatterCurrencyStyle ];
    [ formatter setLocale: _product.priceLocale ];
    
    NSString *str = [ formatter stringFromNumber: _product.price ];
    [ formatter release ];
    return [ str cStringUsingEncoding:NSUTF8StringEncoding ];
#else
    return "";
#endif
}

string ofxGenericStoreProduct::getPriceSymbol()
{
#if TARGET_OS_IPHONE
    return [ [ _product.priceLocale objectForKey:NSLocaleCurrencySymbol ] cStringUsingEncoding:NSUTF8StringEncoding ];
#else
    return "";
#endif
}

float ofxGenericStoreProduct::getPrice()
{
#if TARGET_OS_IPHONE
    return [ _product.price floatValue ];
#else
    return 0.0f;
#endif
}