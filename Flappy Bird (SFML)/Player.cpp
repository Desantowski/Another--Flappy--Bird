#include "Player.h"

#include <iostream>

void Player::SetPosition(sf::Vector2f pos)
{
	playerSprite->SetPosition(pos);
}

Player::Player(sf::RenderWindow * xwindow, int * idptr) : gravityForce(0.f, 10.f), playerVelocity(0.f, 0.f), playerForce(0.f, 5.f), idPtr(idptr)
{
	sf::Texture * playerTextureLoader;
	for (int i = 0; i < 8; i++)
	{
		std::stringstream nameBuffer;
		nameBuffer << "images/Birds/Bird0" << i << ".png";
		playerTextureLoader = new sf::Texture();
		if(playerTextureLoader->loadFromFile(nameBuffer.str()) == false) throw FileNotFound(nameBuffer.str());
		playerTextures.push_back(playerTextureLoader);
	}

	playerSprite = new Sprite(xwindow, playerTextureLoader); // last loaded texture is used in creator

	playerSprite->SetScale(0.2, 0.2);
	playerSprite->SetPosition(sf::Vector2f(50, 300));
}

Player::EventType Player::Logic(sf::Time deltaTime)
{
	playerVelocity += deltaTime.asSeconds() * gravityForce;
	playerSprite->Move(playerVelocity);
	if(playerSprite->GetPosition().y > 580) return EventType::ELose;
	if (playerSprite->GetPosition().y < 30)
	{
		playerVelocity = sf::Vector2f(0,0);
	}
	if (obstacles->MoveAndCheckCollisions(playerSprite) == true) return EventType::ELose;
	if (obstacles->CheckScore(playerSprite) == true) return EventType::EScore;
}

void Player::SetObstacleManager(ObstacleManager * obstmngr)
{
	obstacles = obstmngr;
}

void Player::NoticeMove()
{
	playerVelocity -= gravityForce;
}

void Player::ForceSprite(int *spriteID)
{
	idPtr = spriteID;
	playerSprite->SetTexture(playerTextures[*idPtr]);
}

void Player::ResetVectors()
{
	playerVelocity.x = 0;
	playerVelocity.y = 0;
}

void Player::NoticeSpriteChanged(bool trueIfNext)
{
	int x = *idPtr;
	if (trueIfNext) x++;
	else x--;
	if (x < 0 || x >= playerTextures.size())
		x = x % (playerTextures.size());
	if (x >= playerTextures.size())
		x = 0;
	playerSprite->SetTexture(playerTextures[x]);
	*idPtr = x;
}

void Player::Draw()
{
	playerSprite->Draw();
}

Player::~Player()
{
	for (sf::Texture * x : playerTextures)
		delete x;
	playerTextures.clear();
}
