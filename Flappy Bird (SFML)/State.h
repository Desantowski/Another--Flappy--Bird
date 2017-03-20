#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Renderable.h"
#include "InterfaceManager.h"
#include "ObstacleManager.h"
#include "Player.h"

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

	// Destructor
	virtual ~State();

	// Implementation of Draw()
	virtual void Draw();

	// Logic, for example for moving
	virtual Player::EventType Logic() = 0;

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
	virtual Player::EventType Logic() override;
	virtual InputHandlerReasons InputController(sf::Event & e) override;
	virtual void UpdateGUI(InterfaceManager * _gui) override;
};


// Page 'options' of menu
class OptionsMenu : public Menu
{
public:
	OptionsMenu(sf::RenderWindow * x, Player * birdie);
	~OptionsMenu();
	Player * bird;
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
	virtual Player::EventType Logic() override;
	virtual InputHandlerReasons InputController(sf::Event & e) override;
	virtual void UpdateGUI(InterfaceManager * _gui) override;
	virtual void Draw() override;
};


// General game logic
class Running : public State
{
	// Management of obstacles and GUI, pointer to instance of player
	ObstacleManager obstacles;
	sf::Clock gameClock;
	Player * bird;
	virtual void UpdateGUI(InterfaceManager * _gui) override;

public:
	Running(sf::RenderWindow * xwindow, Player * _bird);
	~Running();
	virtual Player::EventType Logic() override;
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