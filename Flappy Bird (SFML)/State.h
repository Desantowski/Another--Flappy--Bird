#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Renderable.h"
#include "InterfaceManager.h"
#include "ObstacleManager.h"

// Abstract state class
class State
{
public:
	// Returned signal from input handler
	enum InputHandlerReasons
	{
		None,
		GameStarted,
		GamePaused,
		GameResumed,
		GameEnded,
		ShowOptions,
		Exit
	};

	// Returned signal from logic processing
	enum EventType
	{
		ENone,
		ELose,
		EScore
	};

	// Destructor
	virtual ~State();

	// Implementation of Draw()
	virtual void Draw();

	// Logic, for example for moving
	virtual EventType Logic() = 0;

	// Handling input
	virtual InputHandlerReasons InputController(sf::Event & e) = 0;

	// Pointer for interface
	InterfaceManager * gui;

	// Updating and redraws gui
	virtual void UpdateGUI(InterfaceManager * _gui);
};


class Menu : public State
{
protected:
	// Selected item
	int selection;

public:
	Menu(sf::RenderWindow * x);
	~Menu();
	virtual void Draw() override;
	virtual EventType Logic() override;
	virtual InputHandlerReasons InputController(sf::Event & e) override;
	virtual void UpdateGUI(InterfaceManager * _gui) override;
};


// Page 'options' of menu
class OptionsMenu : public Menu
{
private:
	int suboption;
public:
	OptionsMenu(sf::RenderWindow * x);
	~OptionsMenu();
	virtual InputHandlerReasons InputController(sf::Event & e) override;
	virtual void UpdateGUI(InterfaceManager * _gui) override;
};


// SystemPause does mean pauses like end of game - state between game and menu
class SystemPause : public State
{
protected:
	// Gained score
	int score;
public:
	SystemPause(sf::RenderWindow * x);
	~SystemPause();
	virtual EventType Logic() override;
	virtual InputHandlerReasons InputController(sf::Event & e) override;
	virtual void UpdateGUI(InterfaceManager * _gui) override;
	virtual void Draw() override;
};


// General game logic
class Running : public State
{
	// Management of obstacles
	ObstacleManager obstacles;

	// Player sprite & texture - managed here
	sf::Texture * playerTexture;
	Sprite * playerSprite;

	// Vectors for movement and game clock
	sf::Vector2f gravityForce;
	sf::Vector2f playerForce;
	sf::Vector2f playerVelocity;
	sf::Clock gameClock;

	virtual void UpdateGUI(InterfaceManager * _gui) override;

public:
	Running(sf::RenderWindow * xwindow);
	~Running();
	virtual EventType Logic() override;
	virtual InputHandlerReasons InputController(sf::Event & e) override;
	virtual void Draw() override;

	// Used when player resumes game - difference of time before pause and after can be very long, so FixClock updates clock before logic runs
	void FixClock();
};


// Typical pause
class Pause : public SystemPause
{
public:
	Pause(sf::RenderWindow * xwindow);
	~Pause();
	State::InputHandlerReasons InputController(sf::Event & e)override;
	void UpdateGUI(InterfaceManager * gui) override;
};