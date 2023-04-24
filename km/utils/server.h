#pragma once
#include "../common.h"
#include "sockets.h"


/**
* @brief System thread to handle connections.
* @param pContext: The context.
*/
void NTAPI SocketThread(PVOID pContext);

/**
* @brief Initializes the sockets.
* @return error code.
*/
int CreateListenSocket( );

/**
* @brief Receives a packet from the client.
* @param ClientSocket: The client socket.
* @param Packet: The packet.
* @return true if successful, false otherwise.
*/
bool ReceivePacket(SOCKET ClientSocket, Packet_t* pPacket);
