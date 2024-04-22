#pragma once
#include <string>

class UdpAddress
{
public:

	typedef unsigned short PortNumber;


public:
	std::string ip = "";
	PortNumber port = 0;

	std::string ToString();

};