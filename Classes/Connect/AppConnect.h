//
//  AppConnect.h
//  NewProject
//
//  Created by Jiangy on 13-1-5.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#ifndef __NewProject__AppConnect__
#define __NewProject__AppConnect__

#include <iostream>

#include "AppClient.h"
#include "AppServer.h"

class AppConnect {
    
public:
    virtual ~AppConnect();
    static AppClient * client();
    static AppServer * server();
    static void stopConnect();
};

#endif /* defined(__NewProject__AppConnect__) */
