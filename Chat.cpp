#include "Chat.h"
#include <iostream>
#include <thread>
#include "ConsoleControl.h"

void Chat::ShowMesssage(std::string message)
{
	_messagesMutex.lock();

	_messages.push_back(message);

	_messagesMutex.unlock();

	ConsoleControl::LockMutex();

	std::cout << message << std::endl;

	ConsoleControl::UnlockMutex();
}

void Chat::ShowWarning(std::string message)
{
	ConsoleControl::LockMutex();
	ConsoleControl::SetColor(ConsoleControl::YELLOW);

	std::cout << message << std::endl;

	ConsoleControl::SetColor(ConsoleControl::WHITE);
	ConsoleControl::UnlockMutex();
}

void Chat::ShowError(std::string message)
{
	ConsoleControl::LockMutex();
	ConsoleControl::SetColor(ConsoleControl::RED);

	std::cout << message << std::endl;

	ConsoleControl::SetColor(ConsoleControl::WHITE);
	ConsoleControl::UnlockMutex();
}

void Chat::ListenClientsConnections(unsigned short port)
{

	sf::TcpListener listener;

	if (listener.listen(port) != sf::Socket::Done)
	{
		std::cout << "Error on start listener" << std::endl;
		return;
	}

	sf::IpAddress ipAddress = sf::IpAddress::getLocalAddress();
	std::cout << "Listening on IP: " << ipAddress.toString() << std::endl;

	sf::TcpSocket client;

	if (listener.accept(client) != sf::Socket::Done)
	{
		std::cout << "Error on accept Client" << std::endl;
		return;
	}
}

Chat* Chat::Server(unsigned short port)
{
	Chat* chat = new Chat();

	chat->_serverAddress = sf::IpAddress::getLocalAddress();



	return chat;
}

Chat* Chat::Client(unsigned short port, std::string ip)
{
	Chat* chat = new Chat();



	return chat;
}
