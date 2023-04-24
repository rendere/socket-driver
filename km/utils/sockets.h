#pragma once
#include "ksocket/ksocket.h"
#include "ksocket/berkeley.h"

typedef int SOCKET;
#define INVALID_SOCKET  (SOCKET)(-1)
#define SOCKET_ERROR            (-1)

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
	char* m_szProcessName;
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
