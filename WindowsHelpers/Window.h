#pragma once

#include <list>
#include <mutex>

#include "Button.h"

class Window
{
public:

	/// <summary>
	/// 
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="title"></param>

	Window(unsigned int width = 800, unsigned int height = 600, std::string title = "Window");

	void AddButton(Button* button);
	void AddDrawable();
	void RunWindowsLoop();

private:

	std::mutex _mutex;
	sf::RenderWindow _window;

	Button* _lastClickedDownButton;
	std::list<Button*> _buttons;
	std::list<sf::Drawable*> _objectsToDraw;
};