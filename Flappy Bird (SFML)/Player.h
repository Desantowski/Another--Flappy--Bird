#pragma once
#include <vector>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "Renderable.h"
#include "Exception.h"
#include "ObstacleManager.h"

class Player
{
	// Player sprite & texture - managed here
	Sprite * playerSprite;
	std::vector<sf::Texture *> playerTextures;

	// Vectors for movement and game clock
	sf::Vector2f gravityForce;
	sf::Vector2f playerForce;
	sf::Vector2f playerVelocity;

	// Player calls obstacle manager to check possible collisions of playerSprite
	ObstacleManager * obstacles;

	// Pointer to global variable of textureID, necessary for saving
	int * idPtr;

public:
	void Draw();
	// Returned signal from logic processing
	enum EventType
	{
		ENone,
		ELose,
		EScore
	};

	Player(sf::RenderWindow * xwindow, int * idptr);
	~Player();
	EventType Logic(sf::Time deltaTime);

	// Input handler - passed by Running-State class when player moves (mouse key pressed)
	void NoticeMove();

	// Sets pointer to instance of obstacle managers, which is necessary for collision detection
	void SetObstacleManager(ObstacleManager * obstmngr);

	// Input handler - passed by Menu-State class when player changes texture in options, trueIfNext, false otherwise
	void NoticeSpriteChanged(bool trueIfNext);

	// Sets position
	void SetPosition(sf::Vector2f pos);

	// Forces to set specific sprite and sets pointer to sprite ID global variable
	void ForceSprite(int *spriteID);

	// Resets player velocity when game starts
	void ResetVectors();
};

