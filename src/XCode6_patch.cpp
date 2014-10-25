//
//  XCode6_patch.cpp
//  ofxGeneric
//
//  Created by Mark Palange on 10/23/14.
//  Copyright (c) 2014 Lumos Labs. All rights reserved.
//

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "time.h"

extern "C"
{
    size_t fwrite$UNIX2003( const void *a, size_t b, size_t c, FILE *d )
    {
        return fwrite(a, b, c, d);
    }
    char* strerror$UNIX2003( int errnum )
    {
        return strerror(errnum);
    }
    time_t mktime$UNIX2003(struct tm * a)
    {
        return mktime(a);
    }
    double strtod$UNIX2003(const char * a, char ** b) {
        return strtod(a, b);
    }
}