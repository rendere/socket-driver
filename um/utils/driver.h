#pragma once
#include "../common.h"
constexpr static unsigned int g_uMagicValue = 0x6641231;

// Enums.
enum EPacketType : unsigned int
{
	PacketType_Process = 0x14423,
	PacketType_Module = 0x14424,
	PacketType_Result = 0x14443,
};

// Structs.
struct PacketHeader_t
{
	unsigned int m_uMagicValue;
	unsigned int m_uPacketSize;
	unsigned int m_uPacketType;
};

struct Process_t
{
	const char* m_szProcessName;
};

struct Result_t
{
	uint64_t m_uResult;
};

struct Packet_t
{
	PacketHeader_t m_Header;
	union {
		Process_t m_Process;
		Result_t m_Result;
	} m_Data;
};


class CDriver
{
public:
	CDriver( ) = default;
	~CDriver( );

	/**
	* @brief Initializes the driver.
	* @return error code.
	*/
	int Initialize( );

	/**
	* @brief Close the socket, and cleanup.
	*/
	void Shutdown( );

	/**
	* @brief Gets a process by name.
	* @param ProcessName: The name of the process to get.
	* @return The process' ID.
	*/
	uint64_t GetProcessByName(const char* szProcessName) const;
private:
	SOCKET m_sSocket = INVALID_SOCKET;

	/**
	* @brief Send a packet to the driver.
	* @param Packet: The packet to send.
	* @param Data: The resulting data.
	* @return true/false if the packet was sent successfully.
	*/
	__forceinline bool SendPacket(const Packet_t& Packet, uint64_t& data) const
	{
		Packet_t Result = { };
		if (send(m_sSocket, (const char*)&Packet, Packet.m_Header.m_uPacketSize, 0) == SOCKET_ERROR)
			return false;

		if (const int iRead = recv(m_sSocket, (char*)&Result, sizeof(Packet_t), 0); iRead == SOCKET_ERROR || iRead < sizeof(Packet_t))
			return false;

		if (Packet.m_Header.m_uMagicValue != g_uMagicValue ||
			Result.m_Header.m_uMagicValue != g_uMagicValue ||
			Result.m_Header.m_uPacketType != PacketType_Result ||
			Result.m_Header.m_uMagicValue != Packet.m_Header.m_uMagicValue)
			return false;

		data = Result.m_Data.m_Result.m_uResult;
		return true;
	}
};

static inline CDriver g_Driver = CDriver( );
