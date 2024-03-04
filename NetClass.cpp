#include <iostream>
#include <conio.h>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include "ConnectionAbstraction/SocketsManager.h"
#include "WindowsHelpers/Window.h"
#include "ConsoleControl.h"
#include "Chat.h"

#define PORT 3000

void RunClient();
void RunServer();
void RunWindows();

enum PackagedIds : Packet::PacketKey { Message= 0 };

int main() {

	std::cout << "Select" << std::endl << "Client -> C" << std::endl << "Server -> S" << std::endl;

	char mode = ' ';

	RunWindows();

	do
	{
		mode = ConsoleControl::WaitForReadNextChar();

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

	std::cout << "Set server IP --> ";

	std::string ip;
	std::getline(std::cin, ip);

	SocketsManager* socketsManager = new SocketsManager([](TcpSocket* socket)
	{
		std::cout << "Socket connected: " << socket->getRemoteAddress().toString() << std::endl;

		socket->Subscribe(Message, [socket](Packet packet)
		{
			std::string message;
			packet >> message;
			std::cout << "New Message: " << message << std::endl;
		});

		socket->SubscribeOnDisconnect([](TcpSocket* socket)
		{
			std::cout << "Socket disconnected: " << socket->getRemoteAddress().toString() << std::endl;
		});

		std::string response = "Hola soy el cliente";
		Packet packet;
		packet << response;

		socket->Send(Message, packet);
	});

	if (socketsManager->ConnectToServer(ip, PORT))
	{
		socketsManager->StartLoop();
	}

}

void RunServer() {

	std::cout << "Server" << std::endl;

	SocketsManager* socketsManager = new SocketsManager([](TcpSocket* socket)
	{
		std::cout << "Socket connected: " << socket->getRemoteAddress().toString() << std::endl;

		socket->Subscribe(Message, [socket](Packet packet)
		{
			std::string message;
			packet >> message;
			std::cout << "New Message: " << message << std::endl;

			std::string response = "Pues yo soy el server";
			Packet responsePacket;
			responsePacket << response;

			socket->Send(Message, responsePacket);
		});

		socket->SubscribeOnDisconnect([](TcpSocket* socket)
		{
			std::cout << "Socket disconnected: " << socket->getRemoteAddress().toString() << std::endl;
		});

	});

	if (socketsManager->StartListener(PORT))
	{
		sf::IpAddress ipAddress = sf::IpAddress::getLocalAddress();
		std::cout << "Listening on IP: " << ipAddress.toString();
		socketsManager->StartLoop();

	}
}

void RunWindows() 
{
	/*sf::RenderWindow windows;

	windows.create(sf::VideoMode(800, 600), "chat");
	windows.setFramerateLimit(60);

	sf::Text label;

	sf::Font font;

	font.loadFromFile("Minecraft.ttf");

	label.setFont(font);
	label.setCharacterSize(16);
	label.setFillColor(sf::Color::White);
	label.setString("Hello World!!");
	label.setOrigin(0.5, 0.5);
	sf::Vector2u windowsSize = windows.getSize();
	label.setPosition(windowsSize.x * 0.5, windowsSize.y * 0.5);

	while (windows.isOpen())
	{
		windows.clear(sf::Color::Black);
		windows.draw(label);
		windows.display();
	}*/

	Window window;

	Button* button = new Button(50, 20, "Pieces/GoldQueen.png");
	button->onClick = []() {
		std::cout << "Long Live The Queen" << std::endl;
	};

	window.AddButton(button);
	window.RunWindowsLoop();
}