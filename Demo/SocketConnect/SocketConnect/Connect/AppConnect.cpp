//
//  AppConnect.cpp
//  NewProject
//
//  Created by Jiangy on 13-1-5.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#include "AppConnect.h"

static AppClient * appClient = NULL;
static AppServer * appServer = NULL;

AppConnect::~AppConnect()
{
    delete appClient;
    delete appServer;
}

AppClient * AppConnect::client()
{
    if (appClient == NULL) {
        appClient = new AppClient();
    }
    return appClient;
}

AppServer * AppConnect::server()
{
    if (appServer == NULL) {
        appServer = new AppServer();
    }
    return appServer;
}

void AppConnect::stopConnect()
{
    
}
