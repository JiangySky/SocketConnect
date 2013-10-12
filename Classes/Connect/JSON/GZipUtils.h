//
//  GZipUtils.h
//  NewProject
//
//  Created by Jiangy on 13-1-14.
//  Copyright (c) 2013年 35VI. All rights reserved.
//

#ifndef __NewProject__GZipUtils__
#define __NewProject__GZipUtils__

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <zlib.h>

class GZipUtils {
    
public:
    static int zcompress(Bytef *data, uLong ndata, Bytef *zdata, uLong *nzdata);
    static int gzcompress(Bytef *data, uLong ndata, Bytef *zdata, uLong *nzdata);
    static int zdecompress(Byte *zdata, uLong nzdata, Byte *data, uLong *ndata);
    static int gzdecompress(Byte *zdata, uLong nzdata, Byte *data, uLong *ndata);
    static int httpgzdecompress(Byte *zdata, uLong nzdata, Byte *data, uLong *ndata);
};

#endif /* defined(__NewProject__GZipUtils__) */
