#include "Player.h"

#include <iostream>

void Player::SetPosition(sf::Vector2f pos)
{
	playerSprite->SetPosition(pos);
}

Player::Player(sf::RenderWindow * xwindow) : gravityForce(0.f, 10.f), playerVelocity(0.f, 0.f), playerForce(0.f, 5.f)
{
	sf::Texture * playerTextureLoader;
	playerTextureLoader = new sf::Texture();
	playerTextureLoader->loadFromFile("images/green_bird/frame-4.png");
	playerTextures.push_back(playerTextureLoader);

	playerSprite = new Sprite(xwindow, playerTextureLoader); // default selection is 0


	playerTextureLoader = new sf::Texture();
	playerTextureLoader->loadFromFile("images/second_bird/frame-4-1.png");
	playerTextures.push_back(playerTextureLoader);

	playerSprite->SetScale(0.2, 0.2);
	playerSprite->SetPosition(sf::Vector2f(50, 300));
}

Player::EventType Player::Logic(sf::Time deltaTime)
{
	playerVelocity += deltaTime.asSeconds() * gravityForce;
	playerSprite->Move(playerVelocity);
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

void Player::NoticeSpriteChanged(bool trueIfNext)
{
	if (trueIfNext) *idPtr++;
	else *idPtr--;
	if (*idPtr < 0 || *idPtr >= playerTextures.size())
		*idPtr % (playerTextures.size() - 1);
	if (*idPtr >= playerTextures.size())
		*idPtr = 0;
	playerSprite->SetTexture(playerTextures[*idPtr]);
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
