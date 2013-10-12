//
//  ___FILENAME___
//  ___PROJECTNAME___
//
//  Created by Jiangy on ___DATE___.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#include "___FILEBASENAME___.h"
#include "../AppConnect.h"

int ___FILEBASENAME___::write(SocketOutputStream * oStream)
{
    int writeSize = oStream->getTail();
    // NOTE: write data to stream
    
    return oStream->getTail() - writeSize;
}

int ___FILEBASENAME___::read(SocketInputStream * iStream)
{
    int readSize = iStream->getHead();
    // NOTE: read data from stream
    
    return iStream->getHead() - readSize;
}

int ___FILEBASENAME___::execute()
{
    switch (requestResult) {
        case kResultSuccess: { // Success
#ifdef __IPHONE_OS_VERSION_MAX_ALLOWED
            dispatch_async(dispatch_get_main_queue(), ^{
                AppConnect::client()->onResponse(this);
            });
#elif COCOS2D_ENGINE
            AppConnect::client()->onResponse(this);
#else
            // other platform or engine
#endif
            return 1;
        }
        default: { // Failure or otherwise
            std::cout << "Response: " << requestResult << " (" + requestErrMsg + ")\n";
#ifdef __IPHONE_OS_VERSION_MAX_ALLOWED
            dispatch_async(dispatch_get_main_queue(), ^{
                AppConnect::client()->onResponseError(this);
            });
#elif COCOS2D_ENGINE
            AppConnect::client()->onResponseError(this);
#else
            // other platform or engine
#endif
        }
    }
    return -1;
}

