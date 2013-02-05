//
//  ofxGenericStoreProduct.h
//  ofxGeneric
//
//  Created by Eli Delventhal on 1/31/13.
//  Copyright (c) 2013 Lumos Labs. All rights reserved.
//

#pragma once

#include "ofxGenericMain.h"

#if TARGET_OS_IPHONE
#include <StoreKit/StoreKit.h>
#endif

class ofxGenericStoreProduct
{
public:
#if TARGET_OS_IPHONE
    static ofPtr< ofxGenericStoreProduct > create( SKProduct *product );
#endif
    virtual ~ofxGenericStoreProduct();
    
    string getIdentifier();
    string getDescription();
    string getTitle();
    string getLocalizedPrice();
    string getPriceSymbol();
    float getPrice();
    
protected:
    void init( ofPtrWeak< ofxGenericStoreProduct > setThis );
    ofxGenericStoreProduct();
    ofPtrWeak< ofxGenericStoreProduct > _this;
    
#if TARGET_OS_IPHONE
    void setSKProduct( SKProduct *product );
    SKProduct *_product;
#endif
};