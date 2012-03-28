//
//  UIDevice+Model.h
//  BrainTrainer
//
//  Created by Ian Grossberg on 1/9/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#pragma once

#import <UIKit/UIKit.h>

@interface UIDevice( Model )

-( NSString* )modelVersion;
-( NSString* )modelVersionPretty;

@end