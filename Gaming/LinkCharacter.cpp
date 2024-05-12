#include "LinkCharacter.h"
#include "../ConsoleControl.h"

LinkCharacter::LinkCharacter(sf::Vector2u startPosition, std::string name)
{
	this->setPosition(startPosition.x, startPosition.y);
	this->setTexture(*GetSpriteAtlas());
}

LinkCharacter::~LinkCharacter()
{
	delete moveRightAnim;
	delete moveLeftAnim;
	delete moveUpAnim;
	delete moveDownAnim;

	delete atackRightAnim;
	delete atackLeftAnim;
	delete atackUpAnim;
	delete atackDownAnim;

	delete chargeRightAnim;
	delete chargeLeftAnim;
	delete chargeUpAnim;
	delete chargeDownAnim;

	delete grabRightAnim;
	delete grabLeftAnim;
	delete grabUpAnim;
	delete grabDownAnim;

	delete throwRightAnim;
	delete throwLeftAnim;
	delete throwUpAnim;
	delete throwDownAnim;
}

sf::Texture* LinkCharacter::GetSpriteAtlas()
{
	static sf::Texture* spriteAtlas = nullptr;

	if (spriteAtlas == nullptr)
	{
		spriteAtlas = new sf::Texture();
		if (!spriteAtlas->loadFromFile("AA2Mats/Link.png"))
		{
			ConsoleControl::LockMutex();
			std::cout << "Error al cargar el atlas de link";
			ConsoleControl::UnlockMutex();
		}
	}

	return spriteAtlas;
}
