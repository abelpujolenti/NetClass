#include "UdpPacket.h"

#include <iostream>
#include <cstring>

UdpPacket UdpPacket::NormalPacket(PacketKey key, ICodable& codable)
{
	UdpPacket newPacket;
	newPacket << PacketType::NORMAL << key << codable;
	return newPacket;
}

UdpPacket UdpPacket::CriticalPacket(PacketKey key, ICodable& codable)
{
	UdpPacket newPacket;
	static CriticalPacketId newId = 0;
	newPacket << PacketType::CRITICAL << key << newId << codable;
	//if (newId >= sf::)
	{

	}
	newId++;
	return newPacket;
}

UdpPacket UdpPacket::CriticalResponsePacket(PacketKey key, CriticalPacketId criticalPacketId, ICodable& codable)
{
	UdpPacket newPacket;
	newPacket << PacketType::CRITICAL_RESPONSE << key << criticalPacketId << codable;
	return newPacket;
}

UdpPacket UdpPacket::AcumulatedPacket(CodablePointerVector<UdpPacket> packetsList)
{
	UdpPacket newPacket;
	newPacket << PacketType::ACUMULATED << packetsList;
	return newPacket;
}

void UdpPacket::Code(sf::Packet& packet)
{
	size_t size = getDataSize();
	packet << size;
	packet.append(getData(), size);
}

void UdpPacket::Decode(sf::Packet& packet)
{
	size_t size;
	packet >> size;

	void* newData = malloc(size);

	if (newData == nullptr)
	{
		return;
	}

	const void* fullData = packet.getData();
	const char* fullDataChar = static_cast<const char*>(fullData);

	std::memcpy(newData, fullDataChar + packet.getReadPosition(), size);
	for (size_t i = 0; i < size; ++i)
	{
		bool unit;
		packet >> unit;
	}

	append(newData, size);

	free(newData);
}