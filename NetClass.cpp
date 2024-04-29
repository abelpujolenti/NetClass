#include <iostream>
#include <conio.h>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include "ConnectionAbstraction/SocketsManager.h"
#include "ConnectionAbstraction/UDP/UdpSocket.h"
#include "WindowsHelpers/Window.h"
#include "ConsoleControl.h"
#include "Chat.h"

#define PORT 3000

void RunClient();
void RunServer();
void RunWindows();

enum PackagedIds : Packet::PacketKey { Message= 0 };

class SimpleCodable : public ICodable
{

public:

	std::string text = "";

	SimpleCodable() = default;
	SimpleCodable(std::string text) : text(text)
	{		
	};

	void Code(sf::Packet& packet) override
	{
		packet << text;
	}

	void Decode(sf::Packet& packet) override
	{
		packet >> text;
	}

};


int main() {

	SimpleCodable c1 = SimpleCodable("Hola");
	SimpleCodable c2 = SimpleCodable("Dios");
	std::shared_ptr<UdpPacket> p1 = std::make_shared<UdpPacket>(UdpPacket::NormalPacket(1, c1));
	std::shared_ptr<UdpPacket> p2 = std::make_shared<UdpPacket>(UdpPacket::NormalPacket(2, c2));

	/*SimpleCodable cd1;
	UdpPacket::PacketKey cd1Key;
	sf::Uint8 cd1Type;
	SimpleCodable cd2;
	UdpPacket::PacketKey cd2Key;
	sf::Uint8 cd2Type;

	p1 >> cd1Type >> cd1Key >> cd1;
	p2 >> cd2Type >> cd2Key >> cd2;*/

	CodablePointerVector<UdpPacket> udpPackets;
	udpPackets.emplace_back(p1);
	udpPackets.emplace_back(p2);

	UdpPacket packetAcumulated { UdpPacket::AcumulatedPacket(udpPackets) };

	sf::Uint8 type;
	packetAcumulated >> type;

	CodablePointerVector<UdpPacket> udpPacketsRead;
	packetAcumulated >> udpPacketsRead;

	std::vector<SimpleCodable> codablesReaded;

	CodablePointerVector<UdpPacket>::iterator itEnd { udpPacketsRead.end() };

	for (auto it = udpPacketsRead.begin(); it != itEnd; ++it)
	{
		sf::Uint8 typePacket;
		**it >> typePacket;

		UdpPacket::PacketKey keyPacket;
		**it >> keyPacket;

		SimpleCodable codable;
		**it >> codable;

		codablesReaded.emplace_back(codable);
	}

	std::cout << "Select" << std::endl << "Client -> C" << std::endl << "Server -> S" << std::endl;

	char mode = ' ';

	//RunWindows();

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

	std::cout << "Set server IP --> " << std::endl;

	std::string ip;

	std::getline(std::cin, ip);

	std::shared_ptr<UdpSocket> clientSocket{ std::make_shared<UdpSocket>(PORT + 1, [](std::shared_ptr<UdpConnection> connection) {

			std::cout << "New Connection " << connection->GetAddress().ToString() << std::endl;
		})
	};

	std::cout << "Client On: " << clientSocket->GetAddress().ToString() << std::endl;
	clientSocket->ConnectTo(UdpAddress(ip, PORT));
	
}

void RunServer() {

	std::cout << "Server" << std::endl;


	std::shared_ptr<UdpSocket> serverSocket{ std::make_shared<UdpSocket>(PORT, [](std::shared_ptr<UdpConnection> connection) {

			std::cout << "New Connection " << connection->GetAddress().ToString() << std::endl;
		})
	};

	std::cout << "Server On: " << serverSocket->GetAddress().ToString() << std::endl;
	
}

void RunWindows() 
{
	Window window;

	window.RunWindowsLoop();
}