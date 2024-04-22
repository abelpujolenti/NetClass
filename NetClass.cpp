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


}

void RunServer() {

	std::cout << "Server" << std::endl;

	
}

void RunWindows() 
{
	Window window;

	window.RunWindowsLoop();
}