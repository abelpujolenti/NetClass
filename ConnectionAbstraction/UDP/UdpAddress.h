#pragma once
#include <string>

#include "../ICodable.h"

class UdpAddress : public ICodable
{
public:

	typedef unsigned short PortNumber;


public:
	std::string ip = "";
	PortNumber port = 0;

	UdpAddress() = default;
	UdpAddress(std::string ip, PortNumber port);

	std::string ToString();

	void Code(sf::Packet& packet) override;
	void Decode(sf::Packet& packet) override;

};