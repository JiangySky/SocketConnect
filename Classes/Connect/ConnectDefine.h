//
//  ConnectDefine.h
//  NewProject
//
//  Created by Jiangy on 13-1-5.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#ifndef CrazyDice_ConnectDefine_h
#define CrazyDice_ConnectDefine_h

#pragma once

#include <math.h>
#include <sys/types.h>
#ifdef WIN32
#include <winsock.h>
#pragma comment(lib, "wsock32")
#else
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#ifdef __IPHONE_OS_VERSION_MAX_ALLOWED
#include <dispatch/dispatch.h>
#endif
#endif

#ifndef __IPHONE_OS_VERSION_MAX_ALLOWED
typedef	unsigned short		ushort;
typedef	unsigned int		uint;
#endif

#define COCOS2D_ENGINE      1

#define STRING_LENGTH(_str)     ([_str length] * 2)
#define STRING_PACKETSIZE(_str) (_str.length() == 0 ? 0 : _str.length() + sizeof(ushort) + 1)

#ifndef INSTANCE
#define INSTANCE(_cName)        _cName::getInstance()
#define CLASS_INSTANCE(_cName) \
static _cName * getInstance() \
{ \
    static _cName * pIns = NULL; \
    if (pIns == NULL) { \
        pIns = new _cName(); \
        pIns->init(); \
    } \
    return pIns; \
}
#endif

#ifndef FREE_SAFE
#define FREE_SAFE(_obj)             if (_obj) { free(_obj); _obj = NULL; }
#define DELETE_SAFE(_obj)           if (_obj) { delete _obj; _obj = NULL; }
#define MALLOC(_obj, _type, _count) { \
int mallocSize = _count * sizeof(_type); \
_obj = (_type *)malloc(mallocSize); \
memset(_obj, 0, mallocSize); \
}
#define BZERO(_obj, _type, _count)  bzero(_obj, _count * sizeof(_type))
#endif

#define ENCRYPT(x, xlen, KEY, BeginPlace )	if( (x) != NULL ) \
{ \
char* t_pBuffer = (x);   \
char* pKey	    = {KEY}; \
uint  KeyLen    = (uint)strlen( pKey );	\
for ( uint i = 0; i < (xlen); i++ )		\
{ \
*t_pBuffer ^= pKey[(i+BeginPlace)%KeyLen];	\
t_pBuffer++;	\
}\
}

#define PACKET(_cName, _pId)    _cName * packet = (_cName *)PacketManager::getInstance()->getPacket(_pId); \
                                packet->packetId = _pId;

//#define PACKET_WITH_JSON
#define CLIENT_BYTE_ORDER           1234
#define SERVER_BYTE_ORDER           1234
#define GET_PACKET_INDEX(a)         ((a) >> 24)
#define GET_PACKET_LEN(a)           ((a) & 0xffffff)
#define SET_PACKET_INDEX(a,index)   ((a) = ((index<<24) + ((a) & 0xffffff)))
#define SET_PACKET_LEN(a,len)       ((a) = ((len) + ((a)&0xff000000)))

#define DAY_TIME_24                 86400   // One day

#define kResultSuccess              0
#define kResultFailure              1

// size of packet head
#define PACKET_HEADER_SIZE          12
// packet version
#define PACKET_VERSION              1

#define DEFAULTINPUTBUFFERSIZE      800 * 1024
#define DISCONNECTINPUTSIZE         1024 * 1024

// NOTE: 0.Server; 1.Jiangy; 2.Test; 3.Other
#define CONNECT_TO_LS               1

// MARK: Server IP and port
#if CONNECT_TO_LS == 0
#define SERVERIP        "218.15.113.98"
#define SERVERPROT      9810
#elif CONNECT_TO_LS == 1
#define SERVERIP        "192.168.187.129"
#define SERVERPROT      30000
#elif CONNECT_TO_LS == 2
#define SERVERIP        "112.90.226.226"
#define SERVERPROT      9810
#elif CONNECT_TO_LS == 3
#define SERVERIP        "192.168.1.170"
#define SERVERPROT      9810
#endif

// MARK: Data encryption
#define GAMESERVER_TO_CLIENT_KEY	"abc123"
#define CLIENT_TO_GAMESERVER_KEY	"abc123"
#define LOGIN_TO_CLIENT_KEY			"abc123"
#define CLIENT_TO_LOGIN_KEY			"abc123"
#define OSTREAM_KEY					"Unknown"

#define NETNOT              0       // disable
#define NETWIFI             1       // wifi
#define NETWWAN             2       // 3G

#define CHECK_CONNECT
#define CONNECTTIMEOUT      100
#define CONNECTSUCCEED      101

// MARK: declare limit
#define MAX_NICKNAME        12
#define IP_SIZE             24
#define HEART_BEAT          5

// MARK: declare key
#define kKeyRequestType             "requestType"               // request type
#define kKeyRequestResult           "result"                    // response result
#define kKeyRequestErrMsg           "rContent"                  // response message

// MARK: list packet id
typedef enum _RequestType {
    PACKET_NONE                 = 10000,    // null
    PACKET_HEARTBEAT            = 9999,     // heart beat
    PACKET_SEND_ONLY            = 9998,     // send only
    PACKET_RECEIVE_ONLY         = 9997,     // receive only
    PACKET_DEBUG                = 9996,     // debug
    PACKET_TEST                 = 11023,    // test
    PACKET_MAX                  = 50000,    // upper limit
    PACKET_ERROR                = 49991,    // error
    
    // NOTE: other packet id
    /*
     PACKET_***       C -> S  主动
     PACKET_***_RES   S -> C  被动
     PACKET_RES_***   S -> C  主动/被动
     */
    
    /* Request */
    
    
    /* Response */
    
    
} RequestType;

#pragma mark -
#ifndef __IPHONE_OS_VERSION_MAX_ALLOWED
inline uint16_t _OSSwapInt16(uint16_t arg) {
    uint16_t result;
    result = (uint16_t)(((arg << 8) & 0xFF00) | ((arg >> 8) & 0xFF));
    return result;
}

inline uint32_t _OSSwapInt32(uint32_t arg) {
    uint32_t result;
    result = ((arg & 0xFF) << 24) | ((arg & 0xFF00) << 8) | ((arg >> 8) & 0xFF00) | ((arg >> 24) & 0xFF);
    return result;
}

inline uint64_t _OSSwapInt64(uint64_t arg) {
    union CFSwap {
        uint64_t sv;
        uint32_t ul[2];
    } tmp, result;
    tmp.sv = arg;
    result.ul[0] = _OSSwapInt32(tmp.ul[1]);
    result.ul[1] = _OSSwapInt32(tmp.ul[0]);
    return result.sv;
}
#endif
#endif
