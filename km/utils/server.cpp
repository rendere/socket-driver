#include "server.h"
#include "mem.h"


void SocketThread(PVOID pContext)
{
	NTSTATUS Status = KsInitialize( );
	if (!NT_SUCCESS(Status))
	{
		LOG("[ERROR] Failed to initialize the socket thread. Status: 0x%X\r\n", Status);
		return;
	}

	const SOCKET ListenSocket = CreateListenSocket( );
	if (ListenSocket == INVALID_SOCKET)
	{
		LOG("[ERROR] Failed to create the listen socket.\r\n");
		return;
	}

	LOG("[INFO] Listening on port 17445.\r\n");
	while (true)
	{
		SOCKADDR_IN ClientAddress = { 0 };
		int iClientAddressSize = sizeof(ClientAddress);
		SOCKET ClientSocket = accept(ListenSocket, reinterpret_cast<sockaddr*>( &ClientAddress ), &iClientAddressSize);
		if (ClientSocket == INVALID_SOCKET)
		{
			LOG("[ERROR] Failed to accept the client socket.\r\n");
			continue;
		}

		LOG("[INFO] Accepted a client.\r\n");

		// Receieve the client's request.
		Packet_t Packet{ };
		if (!ReceivePacket(ClientSocket, &Packet))
		{
			LOG("[ERROR] Failed to receive the client's packet.\r\n");
			continue;
		}

		// Log the packet data (for debugging purposes).
		LOG("[INFO] Packet type: %u, packet size: %u.\r\n", Packet.m_Header.m_uPacketType, Packet.m_Header.m_uPacketSize);
	}
}

int CreateListenSocket( )
{
	SOCKADDR_IN ServerAddress = { 0 };
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_port = htons(17445);

	const SOCKET ListenSocket = socket_listen(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET)
	{
		DbgPrintEx(0, 0, "[ERROR] Failed to create the listen socket.\r\n");
		return SOCKET_ERROR;
	}

	if (bind(ListenSocket, reinterpret_cast<const sockaddr*>( &ServerAddress ), sizeof(ServerAddress)) == SOCKET_ERROR)
	{
		DbgPrintEx(0, 0, "[ERROR] Failed to bind the listen socket.\r\n");
		return SOCKET_ERROR;
	}

	if (listen(ListenSocket, 10) == SOCKET_ERROR)
	{
		DbgPrintEx(0, 0, "[ERROR] Failed to listen on the listen socket.\r\n");
		return SOCKET_ERROR;
	}

	return ListenSocket;
}

bool ReceivePacket(SOCKET ClientSocket, Packet_t* pPacket)
{
	const int iBytesReceived = recv(ClientSocket, reinterpret_cast<char*>( pPacket ), sizeof(Packet_t), 0);
	if (iBytesReceived <= NULL)
	{
		// This request was malformed. We can't do anything with it.
		return false;
	}
	
	return true;
}
