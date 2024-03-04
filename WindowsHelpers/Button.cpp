#include "Button.h"

Button::Button(float x, float y, std::string texturePath)
{
	if (!_texture.loadFromFile(texturePath))
	{
		return;
	}
	this->setPosition(x, y);
	this->setTexture(_texture);
}

bool Button::CheckBounds(unsigned int x, unsigned int y)
{
	return this->getGlobalBounds().contains(x, y);
}
