#include <iostream>
#include <conio.h>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include "ConsoleControl.h"
#include "Chat.h"

#define PORT 3000

void RunClient();
void RunServer();
void RunWindows();

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

	Chat* chat = Chat::Client(PORT, ip);
}

void RunServer() {

	std::cout << "Server" << std::endl;

	Chat* chat = Chat::Server(PORT);	
}

void RunWindows() 
{
	sf::RenderWindow windows;

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
	}
}