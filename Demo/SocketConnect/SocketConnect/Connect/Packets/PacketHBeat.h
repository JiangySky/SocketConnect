//
//  PacketHBeat.h
//  NewProject
//
//  Created by Jiangy on 13-1-5.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#ifndef __NewProject__PacketHBeat__
#define __NewProject__PacketHBeat__

#include <iostream>
#include "../Packet.h"

class PacketHBeat : public Packet {
    
public:
    CLASS_INSTANCE(PacketHBeat)
    
    bool init();
    int savedTime;
#ifdef PACKET_WITH_JSON
    virtual int writeJSON(SocketOutputStream * oStream);
#else
    virtual int write(SocketOutputStream * oStream);
    virtual uint getPacketSize();
#endif
    int execute();
};

#endif /* defined(__NewProject__PacketHBeat__) */
