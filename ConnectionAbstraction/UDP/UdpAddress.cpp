#include "UdpAddress.h"

#include <sstream>

UdpAddress::UdpAddress(std::string ip, PortNumber port) : ip(ip), port(port)
{}

std::string UdpAddress::ToString()
{
	std::stringstream str;
	str << ip << ":" << port;

	return str.str();
}

void UdpAddress::Code(sf::Packet& packet)
{
	packet << ip << port;
}

void UdpAddress::Decode(sf::Packet& packet)
{
	packet >> ip >> port;
}