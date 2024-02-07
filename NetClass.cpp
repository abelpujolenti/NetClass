#include <iostream>
#include <conio.h>
#include <SFML/Network.hpp>

void RunClient();
void RunServer();

int main() {

	std::cout << "Select" << std::endl << "Client -> C" << std::endl << "Server -> S" << std::endl;

	char mode = ' ';

	do
	{
		if (_kbhit())
		{
			mode = _getch();
		}
	} while (mode != 'C' && mode != 'c' && mode != 'S' && mode != 's');

	switch (mode)
	{
		case 'C':
		case 'c': {
		
			RunClient();
			break;
		}

		case 'S':
		case 's': {

			RunServer();
			break;
		}

	default:
		break;
	}

	while (true);

}

void RunClient() {

	std::cout << "Client" << std::endl;

	sf::TcpSocket socket;

	sf::Socket::Status status = socket.connect("10.40.2.214", 3000);

	if (status != sf::Socket::Done)
	{
		std::cout << "Error on connect To Server" << std::endl;
		return;
	}

	while (true)
	{
		std::cout << "Next Message" << std::endl;
		std::string message;
		std::getline(std::cin, message);
		
		char data[100];

		int stringSize = message.size();
		
		for (int i = 0; i < stringSize; i++)
		{
			data[i] = message[i];
		}
		
		if (socket.send(data, 100) != sf::Socket::Done)
		{
			std::cout << "Error sending message" << std::endl;
		}
	}
}

void RunServer() {

	std::cout << "Server" << std::endl;

	sf::TcpListener listener;

	if (listener.listen(3000) != sf::Socket::Done)
	{
		std::cout << "Error on start Listener" << std::endl;
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

	std::cout << "Client Connected: " << client.getRemoteAddress().toString() << std::endl;

	while (true)
	{
		char data[100];
		std::size_t received;

		std::string message;

		if (client.receive(data, 100, received) != sf::Socket::Done)
		{
			std::cout << "Error receiving Message" << std::endl;
		}
		else
		{
			for (size_t i = 0; i < received; i++)
			{
				message += data[i];
			}
			std::cout << message << std::endl;
		}
	}
}