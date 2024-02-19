#pragma once
#include "TcpSocket.h"

class TcpListener : public sf::TcpListener
{
public:
	
	bool Listen(unsigned short port);
	bool Accept(TcpSocket& socket);
};

