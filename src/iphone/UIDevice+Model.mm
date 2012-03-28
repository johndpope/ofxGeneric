//
//  UIDevice+Model.m
//  BrainTrainer
//
//  Created by Ian Grossberg on 1/9/12.
//  Copyright (c) 2012 Lumos Labs. All rights reserved.
//

#import "UIDevice+Model.h"
#include <sys/types.h>
#include <sys/sysctl.h>

@implementation UIDevice( Model )

-( NSString* )modelVersion
{
    size_t size;
    
    // Set 'oldp' parameter to NULL to get the size of the data returned so we can allocate appropriate amount of space
    sysctlbyname( "hw.machine", NULL, &size, NULL, 0 ); 
    
    char* systemNameBuffer = malloc( size );    
    sysctlbyname( "hw.machine", systemNameBuffer, &size, NULL, 0 );
    NSString* systemName = [ NSString stringWithCString:systemNameBuffer encoding: NSUTF8StringEncoding ];
    free( systemNameBuffer );
    
    return systemName;
}

// https://gist.github.com/1323251
-( NSString* )modelVersionPretty
{
    NSString* modelVersion = [ self modelVersion ];
    
    if ( [ modelVersion isEqualToString:@"iPhone1,1" ] )    return @"iPhone 1G";
    
    if ( [ modelVersion isEqualToString:@"iPhone1,2" ] )    return @"iPhone 3G";
    if ( [ modelVersion isEqualToString:@"iPhone2,1" ] )    return @"iPhone 3GS";
    
    //    if ( [ modelVersion isEqualToString:@"iPhone3,1" ] )    return @"iPhone 4/AT&T";
    //    if ( [ modelVersion isEqualToString:@"iPhone3,3" ] )    return @"iPhone 4/Verizon";
    if ( [ modelVersion hasPrefix:@"iPhone3" ] ) return @"iPhone 4";    
    
    //    if ( [ modelVersion isEqualToString:@"iPhone4,1" ] )    return @"iPhone 4S";
    //    if ( [ modelVersion isEqualToString:@"iPhone4,2" ] )    return @"iPhone 4S";
    if ( [ modelVersion hasPrefix:@"iPhone4" ] ) return @"iPhone 4S";
    
    if ( [ modelVersion isEqualToString:@"iPod1,1" ] )      return @"iPod Touch 1G";
    if ( [ modelVersion isEqualToString:@"iPod2,1" ] )      return @"iPod Touch 2G";
    if ( [ modelVersion isEqualToString:@"iPod3,1" ] )      return @"iPod Touch 3G";
    if ( [ modelVersion isEqualToString:@"iPod4,1" ] )      return @"iPod Touch 4G";
    
    if ( [ modelVersion isEqualToString:@"iPad1,1" ] )      return @"iPad";
    
    if ( [ modelVersion isEqualToString:@"iPad2,1" ] )      return @"iPad 2 (WiFi)";
    if ( [ modelVersion isEqualToString:@"iPad2,2" ] )      return @"iPad 2 (GSM)";
    if ( [ modelVersion isEqualToString:@"iPad2,3" ] )      return @"iPad 2 (CDMA)";

    if ( [ modelVersion isEqualToString:@"iPad3,1" ] )      return @"iPad 3";
    if ( [ modelVersion isEqualToString:@"iPad3,2" ] )      return @"iPad 3";

    if ( [ modelVersion isEqualToString:@"i386" ] )         return @"Simulator";
    if ( [ modelVersion isEqualToString:@"x86_64" ] )       return @"Simulator";

    return [ NSString stringWithFormat:@"Unknown (%@)", modelVersion ];
}

@end