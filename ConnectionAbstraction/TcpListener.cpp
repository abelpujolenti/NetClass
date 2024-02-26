#include "TcpListener.h"

bool TcpListener::Listen(unsigned short port)
{
    return listen(port) == sf::Socket::Done;
}

bool TcpListener::Accept(TcpSocket& socket)
{
    return accept(socket) == sf::Socket::Done;
}
