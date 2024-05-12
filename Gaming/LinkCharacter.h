#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Animation.h"

class LinkCharacter : public sf::Sprite
{
public:

	LinkCharacter(sf::Vector2u startPosition, std::string name);
	~LinkCharacter();

	const sf::Vector2i LinkSize { sf::Vector2i(32, 32)};

	Animation* moveDownAnim = new Animation(this, sf::Vector2i(8, 72), LinkSize, 1000,10, 10, 1, false);
	Animation* moveLeftAnim = new Animation(this, sf::Vector2i(344, 72), LinkSize, 1000, 10, 10, 1, false);
	Animation* moveRightAnim = new Animation(this, sf::Vector2i(344, 72), LinkSize, 1000, 10, 10, 1, true);
	Animation* moveUpAnim = new Animation(this, sf::Vector2i(674, 72), LinkSize, 1000, 10, 10, 1, false);

	Animation* atackDownAnim = new Animation(this, sf::Vector2i(8, 343), LinkSize, 500, 11, 11, 1, false);
	Animation* atackLeftAnim = new Animation(this, sf::Vector2i(379, 343), LinkSize, 500, 10, 10, 1, false);
	Animation* atackRightAnim = new Animation(this, sf::Vector2i(379, 343), LinkSize, 500, 10, 10, 1, true);
	Animation* atackUpAnim = new Animation(this, sf::Vector2i(719, 343), LinkSize, 500, 11, 11, 1, false);

	Animation* chargeDownAnim = new Animation(this, sf::Vector2i(8, 1456), LinkSize, 1000, 10, 10, 1, false);
	Animation* chargeLeftAnim = new Animation(this, sf::Vector2i(349, 1456), LinkSize, 1000, 10, 10, 1, false);
	Animation* chargeRightAnim = new Animation(this, sf::Vector2i(349, 1456), LinkSize, 1000, 10, 10, 1, true);
	Animation* chargeUpAnim = new Animation(this, sf::Vector2i(688, 1456), LinkSize, 1000, 10, 10, 1, false);

	Animation* grabDownAnim = new Animation(this, sf::Vector2i(8, 1411), LinkSize, 500, 5, 5, 1, false);
	Animation* grabLeftAnim = new Animation(this, sf::Vector2i(315, 1411), LinkSize, 500, 5, 5, 1, false);
	Animation* grabRightAnim = new Animation(this, sf::Vector2i(315, 1411), LinkSize, 500, 5, 5, 1, true);
	Animation* grabUpAnim = new Animation(this, sf::Vector2i(624, 1411), LinkSize, 500, 5, 5, 1, false);

	Animation* throwDownAnim = new Animation(this, sf::Vector2i(136, 1411), LinkSize, 500, 5, 5, 1, false);
	Animation* throwLeftAnim = new Animation(this, sf::Vector2i(444, 1411), LinkSize, 500, 5, 5, 1, false);
	Animation* throwRightAnim = new Animation(this, sf::Vector2i(444, 1411), LinkSize, 500, 5, 5, 1, true);
	Animation* throwUpAnim = new Animation(this, sf::Vector2i(752, 1411), LinkSize, 500, 5, 5, 1, false);

private:
	
	static sf::Texture* GetSpriteAtlas();
};

