//
//  ClientConnect.cpp
//  NewProject
//
//  Created by Jiangy on 13-1-4.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#include "ClientConnect.h"
#include "AppConnect.h"
#ifdef __IPHONE_OS_VERSION_MAX_ALLOWED
#import <CoreFoundation/CoreFoundation.h>
#endif
#if DATA_LOCK
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
#endif
ClientConnect::~ClientConnect()
{
    this->closeSocket();
}

bool ClientConnect::init()
{
    m_socket = -1;
    m_gamesocket = -1;
    netDelay = 0;
    reconnectCount = 0;
    this->setConnected(false);
    inputStream = INSTANCE(SocketInputStream);
    outputStream = INSTANCE(SocketOutputStream);
#ifdef WIN32
    WSADATA wsaData;
	WORD version = MAKEWORD(2, 0);
	int ret = WSAStartup(version, &wsaData); // win sock start up
	if (ret) {
        return false;
	}
#endif
    return true;
}

bool ClientConnect::connected()
{
    return isConnect && (m_socket >= 0 || m_gamesocket >= 0);
}

void ClientConnect::setConnected(bool conn, bool needTip)
{
    isConnect = conn;
    if (needTip) {
#ifdef __IPHONE_OS_VERSION_MAX_ALLOWED
        dispatch_async(dispatch_get_main_queue(), ^{
            // TODO: check net & show error
        });
#elif COCOS2D_ENGINE
        // TODO: check net & show error
#else
        // other platform or engine
#endif
    }
}

int ClientConnect::createSocket()
{
    if ((m_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << &"Socket Error: " [ m_socket] << std::endl;
        this->closeSocket();
        return -1;
    }
    
    return 0;
}

int ClientConnect::createGameSocket()
{
    if((m_gamesocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << &"Socket Error: " [ m_socket] << std::endl;
        this->closeSocket();
        return -1;
    }
    
    return 0;
}

void ClientConnect::closeSocket()
{
    if (inputStream != NULL) {
        inputStream->cleanUp();
    }
    if (outputStream != NULL) {
        outputStream->cleanUp();
    }
#ifdef WIN32
    closesocket(m_socket);
    closesocket(m_gamesocket);
    WSACleanup();
#else
    close(m_socket);
    close(m_gamesocket);
#endif
    
    this->setConnected(false);
}

bool ClientConnect::connectServer(std::string ip, int port)
{
    return this->connectServer(ip, port, false);
}

bool ClientConnect::connectServer(std::string ip, int port, bool nonblock)
{
    if (this->createSocket() < 0) {
        return false;
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET; // family
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str()); // address
    server_addr.sin_port = htons(port); // port
    if (nonblock) { // set to non-blocking
        unsigned long ul = 1;
        ioctl(m_socket, FIONBIO, &ul);
        int flags = fcntl(m_socket, F_GETFL, 0);
        fcntl(m_socket, F_SETFL, flags | O_NONBLOCK);
    }
    if (connect(m_socket, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) < 0 && !nonblock) {
        std::cout << "Cannot connect to " + ip + ":" << port << std::endl;
        return false;
    }
    this->switchSocket(0);
    return true;
}

void ClientConnect::connectGame(std::string ip, int port)
{
    this->connectGame(ip, port, false);
}

void ClientConnect::connectGame(std::string ip, int port, bool nonblock)
{
    if (this->createGameSocket() < 0) {
        return;
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET; // family
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str()); // address
    server_addr.sin_port = htons(port); // port
    if (nonblock) { // set to non-blocking
        unsigned long ul = 1;
        ioctl(m_gamesocket, FIONBIO, &ul);
        int flags = fcntl(m_gamesocket, F_GETFL, 0);
        fcntl(m_gamesocket, F_SETFL, flags | O_NONBLOCK);
    }
    if (connect(m_gamesocket, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) < 0 && !nonblock) {
        std::cout << "Cannot connect to " + ip + ":" << port << std::endl;
    } else {
        this->switchSocket(1);
    }
}

bool ClientConnect::writeDataFinish()
{
    if (outputStream->isEmpty()) {
        return false;
    }
    return true;
}

void ClientConnect::switchSocket(short flag)
{
    if (flag == 0) { // Login server
        inputStream->inputSocket(m_socket);
        outputStream->outputSocket(m_socket);
    } else if (flag == 1) { // Game server
        inputStream->inputSocket(m_gamesocket);
        outputStream->outputSocket(m_gamesocket);
    }
#ifdef __IPHONE_OS_VERSION_MAX_ALLOWED
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &sa, 0);
#endif
}

#pragma mark - == Process packet == -

int ClientConnect::processSend()
{
    int issend = 100;
#if DATA_LOCK
    pthread_mutex_lock(&mutex);
#endif
    int retSize = outputStream->size();
    if (retSize > 0 && outputStream->isCloseKey) {
        issend = outputStream->flush();
    }
    if (issend <= 0) {
        if (reconnectCount <= 0) {
            reconnectCount = RECONNECT_COUNT;
        }
        this->setConnected(false, true);
    }
#if DATA_LOCK
    pthread_mutex_unlock(&mutex);
#endif
    return issend;
}

int ClientConnect::processRecv()
{
    int isrecv;
    isrecv = inputStream->receive();
    return isrecv;
}

bool ClientConnect::processCommand()
{
#if PROCESS_LOCK
        pthread_mutex_lock(&mutex);
#endif
        int retSize = inputStream->size();
        if (retSize > 0) {
            bool bRet = false;
            char header[PACKET_HEADER_SIZE] = {0};
            uint packetSize = 0;
            ushort packetId = 0;
            ushort errorId = 0;
            ushort encrypt = 0;
            ushort version = 0;
            ushort reserved = 0;
#ifdef PACKET_WITH_JSON
            int compression = 0;
#else
            int readSize = 0;
#endif
            Packet * packet = NULL;
            for (; ; ) {
                if (!inputStream->peek(&header[0], PACKET_HEADER_SIZE)) { // broken
                    break;
                }
                if (!inputStream->decrypt()) {
                    std::cout << "decrypt head error\n";
                    break;
                }
                // head
                memcpy(&packetSize, &header[0], sizeof(uint));
                memcpy(&packetId, &header[sizeof(uint)], sizeof(ushort));
                memcpy(&errorId, &header[sizeof(uint) + sizeof(ushort)], sizeof(ushort));
                memcpy(&encrypt, &header[sizeof(uint) + sizeof(ushort) * 2], sizeof(ushort));
                memcpy(&version, &header[sizeof(uint) + sizeof(ushort) * 3], sizeof(ushort));
                memcpy(&reserved, &header[sizeof(uint) + sizeof(ushort) * 4], sizeof(ushort));
#if DEBUG_PACKET != 0
                if (packetId == DEBUG_PACKET) {
                    std::cout << "DEBUG_PACKET\n";
                }
#endif
                if (packetSize <= 0) {
#if PROCESS_LOCK
                    pthread_mutex_unlock(&mutex);
#endif
                    return false;
                }
                if (inputStream->size() < packetSize) { // broken
                    break;
                }
                if (packetId >= PACKET_MAX) {
                    std::cout << "packetId out range " << packetId << "(" << PACKET_MAX << ")\n";
#if PROCESS_LOCK
                    pthread_mutex_unlock(&mutex);
#endif
                    return false;
                }
                if (!inputStream->skip(PACKET_HEADER_SIZE)) {
                    std::cout << "Failed to skip message head\n";
#if PROCESS_LOCK
                    pthread_mutex_unlock(&mutex);
#endif
                    return false;
                }
                packetSize -= PACKET_HEADER_SIZE;
                // NOTE: process message
                std::cout << "<< Begin process message " << packetId << " >>\n";
                packet = INSTANCE(PacketManager)->getPacket(packetId);
                if (NULL == packet) {
#if PROCESS_LOCK
                    pthread_mutex_unlock(&mutex);
#endif
                    return false;
                }
                packet->receivedPacketId = packetId;
                if (errorId != 0) {
                    packet->requestErrMsg = ErrMsg[errorId];
                }
                // body
#ifdef PACKET_WITH_JSON
                compression = inputStream->readInt();
                bRet = packet->setData(inputStream->readData(packetSize - 4), compression, true);
#else
                readSize = packet->read(inputStream);
                // correction packet size
                if (readSize < packetSize) {
                    bRet = inputStream->skip(packetSize - fmax(0, readSize));
                } else {
                    bRet = (readSize == packetSize);
                }
#endif
                if (!bRet) {
#if PROCESS_LOCK
                    pthread_mutex_unlock(&mutex);
#endif
                    return false;
                }
                packet->execute();
                std::cout << "<<  End  process message " << packetId << " >>\n";
                if (inputStream->size() == 0) {
                    break;
                }
            }
#if PROCESS_LOCK
            pthread_mutex_unlock(&mutex);
#endif
            return true;
        }
#if PROCESS_LOCK
        pthread_mutex_unlock(&mutex);
#endif
        return true;
}

bool ClientConnect::sendPacket(Packet * packet)
{
    if (!isConnect) {
        return false;
    }
#ifdef CHECK_NETWORK
    if (netDelay > NET_RECONNECT) {
        if (packet->packetId != PACKET_HEARTBEAT && reconnectCount == 0) {
            std::cout << "请检查您的网络连接, 连接失败" << std::endl;
        }
        return false;
    } else if (netDelay > NET_WARNNING && packet->packetId != PACKET_HEARTBEAT) {
        std::cout << "网络不稳定，延迟：" << netDelay << std::endl;
    }
#endif
    reconnectCount = 0;
    
    // Send packet
#if DATA_LOCK
    pthread_mutex_lock(&mutex);
#endif
    // head
    uint packetSize = 0;
    ushort packetId = 0;
    ushort errorId = 0;
    ushort encrypt = 0;
    ushort version = 0;
    ushort reserved = 0;
    
    packetSize = packet->getPacketSize() + PACKET_HEADER_SIZE;
    packetId = packet->packetId;
    version = PACKET_VERSION;
    reserved = time(0) & 0xffff;
    encrypt = ((packetSize * packetSize) / m_key1
               + (packetId / m_key2) * (packetId / m_key2)
               + (packetSize + packetId) / m_key3
               + version / m_key4 + reserved / m_key5) & 0xffff;
    
    outputStream->writeUInt(packetSize);
    outputStream->writeUShort(packetId);
    outputStream->writeUShort(errorId);
    outputStream->writeUShort(encrypt);
    outputStream->writeUShort(version);
    outputStream->writeUShort(reserved);
    
    // body
    int writeSize = outputStream->write(packet);
    
    // correction packet size
    if (packetSize != writeSize + PACKET_HEADER_SIZE) {
        packetSize = writeSize + PACKET_HEADER_SIZE;
        outputStream->om_Tail -= packetSize;
        outputStream->writeUInt(packetSize);
        outputStream->om_Tail += packetSize - sizeof(uint);
    }
#if DATA_LOCK
    pthread_mutex_unlock(&mutex);
#endif
    return true;
}
