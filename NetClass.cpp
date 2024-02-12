#include <iostream>
#include <conio.h>
#include <SFML/Network.hpp>
#include "ConsoleControl.h"
#include "Chat.h"

#define PORT 3000

void RunClient();
void RunServer();

int main() {

	std::cout << "Select" << std::endl << "Client -> C" << std::endl << "Server -> S" << std::endl;

	char mode = ' ';

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

	Chat* chat = Chat::Client(PORT, ip);
}

void RunServer() {

	std::cout << "Server" << std::endl;

	Chat* chat = Chat::Server(PORT);

	
}