#include "UdpPacket.h"

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
	//TODO LEER CORRECTAMENTE Y QUITAR DEL BUFFER DE MEMORIA
	append(getData(), size);
}