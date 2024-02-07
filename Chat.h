#pragma once
#include <string>
#include <mutex>
#include <SFML/Network.hpp>
#include <vector>


class Chat
{
private:

	std::vector<std::string> _messages;

	std::mutex _messagesMutex;

	sf::IpAddress _serverAddress;

	Chat() = default;

	void ShowMesssage(std::string message);
	void ShowWarning(std::string message);
	void ShowError(std::string message);

	void ListenClientsConnections(unsigned short port);

public:

	static Chat* Server(unsigned short port);
	static Chat* Client(unsigned short port, std::string ip);

};

