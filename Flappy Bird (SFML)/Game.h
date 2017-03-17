#pragma once
#include <SFML/Graphics.hpp>
#include "InterfaceManager.h"
#include "State.h"

class Game 
{
public:
	Game();
	~Game();
	void Run();

private:
	// Pointer for count of scores
	int * score;
	
	// For better management of memory - used when state changes
	enum StateList
	{
		StateMenu,
		StatePause,
		StateRunning,
		StateLoad,
		StateLose,
		StateExit,
		StateOptions,
		FirstRun
	};

	// Window
	sf::RenderWindow m_window;

	// Handling events. Returns if game loop can be continued (for example, when event is sf::Event::EventType::Closed - HandleEvent returns false)
	bool HandleEvent(sf::Event &);

	// Current state of game, with sprites and graphcis
	State * gameState;

	// Saved - while pausing
	State * savedState;

	// Management of interface objects
	InterfaceManager * gui;

	// Size of window
	static constexpr unsigned int width = 800;
	static constexpr unsigned int height = 600;

	// Memory management
	void ChangeState(StateList newState);
};