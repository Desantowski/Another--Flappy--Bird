#include "ObstacleManager.h"

Obstacle::Obstacle(sf::Texture * upTexture, sf::Texture * downTexture, sf::RenderWindow * render) : crossMark(false)
{
	sf::Vector2f pos(610.f, 0);
	up = new Sprite(render, upTexture);
	up->SetPosition(pos);

	pos.y += 500;
	down = new Sprite(render, downTexture);
	down->SetPosition(pos);
}

void Obstacle::RandomizeY()
{
	// -1 - 11
	float delta = 10 * (0 - (rand() % 11));

	sf::Vector2f posUp(up->GetPosition()); // 610, 1100
	
	up->SetPosition(sf::Vector2f(posUp.x, 0 + delta));
	down->SetPosition(sf::Vector2f(posUp.x, 500 + delta));
}

Obstacle::~Obstacle()
{
	delete up;
	delete down;
}

void Obstacle::Draw()
{
	up->Draw();
	down->Draw();
}

void Obstacle::ForceX(float x)
{
	sf::Vector2f position(up->GetPosition());
	position.x = x;
	up->SetPosition(position);
	
	position = down->GetPosition();
	position.x = x;
	down->SetPosition(position);
}

bool Obstacle::Move(float distance)
{
	up->Move(sf::Vector2f(-1 * distance, 0));
	down->Move(sf::Vector2f(-1 * distance, 0));
	if (up->GetPosition().x < -250)
	{
		float movement = 1000;
		up->Move(sf::Vector2f(movement, 0));
		down->Move(sf::Vector2f(movement, 0));
		crossMark = false;
		RandomizeY();
		return true;
	}
	return false;
}

bool Obstacle::Collision(sf::FloatRect bounds)
{
	if (up->GetBounds().intersects(bounds) || down->GetBounds().intersects(bounds))
	{
		return true;
	}
	else return false;
}

bool Obstacle::Crossed(float left, float width)
{
	if (crossMark == true) return false;
	if (up->GetPosition().x <= left + width / 2)
	{
		crossMark = true;
		return true;
	}
	return false;
}


void ObstacleManager::Draw()
{
	for (Obstacle * x : obstacles)
		x->Draw();
}

ObstacleManager::ObstacleManager(sf::RenderWindow * xwindow)
{
	upTexture = new sf::Texture();
	downTexture = new sf::Texture();

	sf::IntRect area(sf::Vector2i(0, height - range), sf::Vector2i(width, range));
	if (upTexture->loadFromFile("images/GreenObstacleTop.png", area) == false) throw FileNotFound("images/GreenObstacleTop.png");

	sf::IntRect areaLow(sf::Vector2i(0, 0), sf::Vector2i(width, range));
	if (downTexture->loadFromFile("images/GreenObstacleDown.png", areaLow) == false) throw FileNotFound("images/GreenObstacleDown.png");

	Obstacle * loader;

	for (int i = 0; i < 2; i++)
	{
		loader = new Obstacle(upTexture, downTexture, xwindow);
		obstacles.push_back(loader);
		obstacles[i]->ForceX(800 + (100+width) * (2*(i+1)));
		obstacles[i]->RandomizeY();
	}
}

ObstacleManager::~ObstacleManager()
{
	delete upTexture;
	delete downTexture;
}

bool ObstacleManager::MoveAndCheckCollisions(Sprite *& playerSprite)
{
	for(int i = 0; i < obstacles.size(); i++)
	{
		Obstacle *& x = obstacles[i];
		if (x->Move(15.f) == true) { }
		if (x->Collision(playerSprite->GetBounds()) == true)
		{
			return true;
		}
	}
	return false;
}

bool ObstacleManager::CheckScore(Sprite *& playerSprite)
{
	for (Obstacle *& x : obstacles)
	{
		if (x->Crossed(playerSprite->GetBounds().left, playerSprite->GetBounds().width))
		{
			return true;
		}
	}
	return false;
}