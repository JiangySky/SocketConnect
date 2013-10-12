//
//  ___FILENAME___
//  ___PROJECTNAME___
//
//  Created by Jiangy on ___DATE___.
//  Copyright (c) 2013 35VI. All rights reserved.
//

/**
 * #include "___FILENAME___" in PacketAll.h
 * define PACKET_* in _RequestType of ConnectDefine.h
 * return INSTANCE(___FILEBASENAME___) at case in PacketManager::getPacket(uint packetId)
 */

#ifndef _____PROJECTNAME________FILEBASENAME_____
#define _____PROJECTNAME________FILEBASENAME_____

#include <iostream>
#include "Packet.h"

class ___FILEBASENAME___ : public Packet {
    
public:
    CLASS_INSTANCE(___FILEBASENAME___)
    
    // NOTE: declare variable
    
    virtual int write(SocketOutputStream * oStream);
    virtual int read(SocketInputStream * iStream);
    virtual int execute();
    
};

#endif /* defined(_____PROJECTNAME________FILEBASENAME_____) */