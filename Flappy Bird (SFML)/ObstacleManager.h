#pragma once
#include <SFML\Graphics.hpp>
#include "Renderable.h"

// Obstacle inherits from MinimalDrawable for Draw() - it allows to store obstacles and other graphics in one vector
class Obstacle : MinimalDrawable
{
	friend class ObstacleManager;
	// Top and down sprite of obstacle
	Sprite * up;
	Sprite * down;

	// To avoid multiple counting of points for one obstacle (when obstacle is behind player. true does mean this obstacle was scored. It resets with obstacle position
	bool crossMark;

	Obstacle(sf::Texture * upTexture, sf::Texture * downTexture, sf::RenderWindow * render);
	~Obstacle();

	// Drawes & moves both down and up part
	void Draw() override;
	bool Move(float distance);

	// Sets position of obstacle
	void ForceX(float x);
	void RandomizeY();

	// Checks if obstacle collides with player or was crossed
	bool Collision(sf::FloatRect bounds);
	bool Crossed(float left, float width);
};

class ObstacleManager
{
	std::vector<Obstacle*> obstacles;

	// Width & height of texture. Range defines height to load
	static constexpr int width = 138;
	static constexpr int range = 250;
	static constexpr int height = 793;

	// Pointers to textures. Managed by this class
	sf::Texture * upTexture;
	sf::Texture * downTexture;

public:
	ObstacleManager(sf::RenderWindow * xwindow);
	~ObstacleManager();

	// Calls specific functions for every obstacle in obstacles vector
	void Draw();
	bool MoveAndCheckCollisions(Sprite *& playerSprite);
	bool CheckScore(Sprite *& playerSprite);
};