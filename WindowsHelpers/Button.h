#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

class Button : public sf::Sprite
{
public:

	typedef std::function<void()> OnClick;
	OnClick onClick = [](){};

	Button(float x, float y, std::string texturePath);

	bool CheckBounds(unsigned int x, unsigned int y);

private:

	sf::Texture _texture;

};