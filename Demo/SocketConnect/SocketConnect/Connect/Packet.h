//
//  Packet.h
//  NewProject
//
//  Created by Jiangy on 13-1-4.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#ifndef __NewProject__Packet__
#define __NewProject__Packet__

#include <iostream>
#include "SocketOutputStream.h"
#include "SocketInputStream.h"
#ifdef PACKET_WITH_JSON
#include "JSON/json.h"
#include "JSON/GZipUtils.h"
#endif

#define READ_STRING(_obj)  \
{ \
string s = iStream->readString(); \
FREE_SAFE(_obj); \
MALLOC(_obj, char, s.length()); \
memcpy(_obj, s.c_str(), s.length()); \
}

#if COCOS2D_ENGINE
#include "cocos2d.h"
class Packet : public cocos2d::CCObject
#else
class Packet
#endif
{
    
public:
    CLASS_INSTANCE(Packet)
    
    /** response from server */
    int             requestResult;
    int             errorId;
    std::string     requestErrMsg;
    
    /** commond id or packet id */
    ushort          receivedPacketId;
    ushort          packetId;
    
#ifdef PACKET_WITH_JSON
    /** JSON data */
    Json::Value *   jsonData;
#endif
    /** data state of compression(0:false; 1:true) */
    ushort      compression;
    
#pragma mark -
    
    virtual ~Packet();
    virtual bool init();
    virtual void packetData();
    virtual bool setData(std::string data, int compress, bool byServer);

    /**
     @brief     write data to stream
     @param     oStream : SocketOutputStream
     @return    state number
     */
    virtual int write(SocketOutputStream * oStream);
    virtual int writeJSON(SocketOutputStream * oStream);
    
    /**
     @brief     write data size
     @abstract  return actual size to avoid write back to stream
     @return    the size write to stream
     */
    virtual uint getPacketSize();
    
    /**
     @brief     read data from stream
     @param     iStream : SocketInputStream
     @return    read data size
     */
    virtual int read(SocketInputStream * iStream);
    virtual int readJSON(SocketInputStream * iStream);
    
    /**
     @brief     execute after read data from stream
     */
    virtual int execute();
};

#endif /* defined(__NewProject__Packet__) */
