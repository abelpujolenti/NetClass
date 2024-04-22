#pragma once
#include <SFML/Network.hpp>
#include "../ICodable.h"

class UdpPacket : public sf::Packet, public ICodable
{
public:

	typedef sf::Uint8 PacketKey;
	typedef sf::Uint64 CriticalPacketId;

public:
	enum PacketType : sf::Uint8 //Comment putot ;)
	{
		NORMAL = 0,
		CRITICAL = 1,
		CRITICAL_RESPONSE = 2,
		ACUMULATED = 3
	};

	static UdpPacket NormalPacket(PacketKey key, ICodable& codable);
	static UdpPacket CriticalPacket(PacketKey key, ICodable& codable);
	static UdpPacket CriticalResponsePacket(PacketKey key, CriticalPacketId criticalPacketId, ICodable& codable);
	static UdpPacket AcumulatedPacket(CodablePointerVector<UdpPacket> packetsList);

	// Inherited via ICodable
	void Code(sf::Packet& packet) override;
	void Decode(sf::Packet& packet) override;
};