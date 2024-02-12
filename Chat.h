#pragma once
#include <string>
#include <mutex>
#include <SFML/Network.hpp>
#include <vector>
#include <list>


class Chat
{
private:

	bool _isServer;

	std::list<sf::TcpSocket*> _sockets;

	std::mutex _isServerMutex;
	std::mutex _socketMutex;
	std::mutex _messagesMutex;

	std::vector<std::string> _messages;

	sf::IpAddress _serverAddress;

	Chat() = default;

	void ShowMessage(std::string message);
	void ShowAlert(std::string message);
	void ShowError(std::string message);

	void ListenClientsConnections(unsigned short port);
	void ConnectToServer(std::string ip, unsigned short port);

	void OnClientEnter(sf::TcpSocket* client);

	void ListenMessages(sf::TcpSocket* socket);

	void ListenKeyboardToSendMessages();

	void SendMessage(std::string message);

public:

	static Chat* Server(unsigned short port);
	static Chat* Client(unsigned short port, std::string ip);

};

