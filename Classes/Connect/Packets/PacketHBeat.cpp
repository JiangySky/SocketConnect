//
//  PacketHBeat.cpp
//  NewProject
//
//  Created by Jiangy on 13-1-5.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#include "PacketHBeat.h"
#include "../AppConnect.h"

#ifdef PACKET_WITH_JSON
int PacketHBeat::writeJSON(SocketOutputStream *oStream)
{
    return Packet::writeJSON(oStream);
}
#else
int PacketHBeat::write(SocketOutputStream * oStream)
{
    int writeSize = oStream->getTail();
    // NOTE: write data to stream
    oStream->writeInt(PACKET_HEARTBEAT);
    
    return oStream->getTail() - writeSize;
}

ushort PacketHBeat::getPacketSize()
{
    return sizeof(int);
}
#endif