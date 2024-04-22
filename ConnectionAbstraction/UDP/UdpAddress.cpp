#include "UdpAddress.h"

#include <sstream>

std::string UdpAddress::ToString()
{
	std::stringstream str;
	str << ip << ":" << port;

	return str.str();
}