#include "driver.h"


CDriver::~CDriver( )
{
	Shutdown( );
}

int CDriver::Initialize( )
{
	// We need to initialize the socket.
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return GetLastError( );

	// Create the socket.
	m_sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sSocket == INVALID_SOCKET)
		return GetLastError( );

	// Create the server address.
	sockaddr_in ServerAddress{ };
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_port = htons(17445);
	// The address is localhost, because we're using a local driver.
	ServerAddress.sin_addr.s_addr = htonl(0x7F000001);

	// Connect to the server.
	if (connect(m_sSocket, reinterpret_cast<const sockaddr*>( &ServerAddress ), sizeof(ServerAddress)) == SOCKET_ERROR)
		return GetLastError( );

	// We're connected.
	return ERROR_SUCCESS;
}

void CDriver::Shutdown( )
{
	if (m_sSocket != INVALID_SOCKET)
	{
		closesocket(m_sSocket);
		m_sSocket = INVALID_SOCKET;
	}

	WSACleanup( );
}

uint64_t CDriver::GetProcessByName(const char* szProcessName) const
{
	Packet_t Packet = { };
	Packet.m_Header.m_uMagicValue = g_uMagicValue;
	Packet.m_Header.m_uPacketType = PacketType_Process;
	Packet.m_Data.m_Process.m_szProcessName = szProcessName;
	Packet.m_Header.m_uPacketSize = sizeof(Packet_t);

	// Send the packet.
	if (uint64_t uProcessID = 0ULL; SendPacket(Packet, uProcessID))
		return uProcessID;

	return 0ULL;
}
