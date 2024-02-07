#include <iostream>
#include <conio.h>
#include <SFML/Network.hpp>
#include "ConsoleControl.h"

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

	
}

void RunServer() {

	std::cout << "Server" << std::endl;

	
}