#include "Game.h"

Game::Game() : m_window(sf::VideoMode(width, height), "Another Bird"), gameState(nullptr), savedState(nullptr), score(new int(0))
{
}

Game::~Game()
{
	delete gui;
	delete score;
	if (m_window.isOpen())
		m_window.close();
}

void Game::Run()
{
	sf::Texture backgroundTexture;
	backgroundTexture.loadFromFile("images/background.png");

	sf::Sprite backgroundSprite(backgroundTexture);
	backgroundSprite.setPosition(sf::Vector2f(0, 0));

	sf::Event windowEvent;
	gui = new InterfaceManager(width, height, &m_window);
	gui->SetScorePointer(score);

	ChangeState(FirstRun);
	while (m_window.isOpen())
	{
		m_window.clear();
		m_window.draw(backgroundSprite);
		gameState->Draw();
		m_window.display();

		while (m_window.pollEvent(windowEvent))
			if (HandleEvent(windowEvent) == false) 
				return;

		switch(gameState->Logic())
		{
		case State::EventType::ELose:
			ChangeState(StateList::StateLose);
			break;
		case State::EventType::EScore:
			(*score)++;
			gui->NoticePoint();
			break;
		default:
			break;
		}
	}
}

bool Game::HandleEvent(sf::Event & e)
{
	switch (gameState->InputController(e))
	{
	case State::InputHandlerReasons::None:
		break;

	case State::InputHandlerReasons::GameStarted:
		(*score) = 0;
		ChangeState(StateRunning);
		break;

	case State::InputHandlerReasons::GamePaused:
		ChangeState(StatePause);
		break;

	case State::InputHandlerReasons::GameResumed:
		ChangeState(StateLoad);
		break;

	case State::InputHandlerReasons::Exit:
		ChangeState(StateExit);
		return false;
	
	case State::InputHandlerReasons::GameEnded:
		ChangeState(FirstRun);
		break;

	case State::InputHandlerReasons::ShowOptions:
		//ChangeState(StateOptions);
		break;

	default:
		ChangeState(StateExit);
		throw "Not implemented handler!";
	}
	return true;
}

void Game::ChangeState(Game::StateList newState)
{
 	if (newState == StateExit)
	{
		if (gameState != nullptr)
			delete gameState;
		if (savedState != nullptr)
			delete savedState;
		m_window.close();
		return;
	}

	else if (newState == StatePause)
	{
		if (savedState != nullptr) 
			delete savedState;
		savedState = gameState;
		gameState = new Pause(&m_window);
	}

	else if (newState == StateLoad)
	{
		if (gameState != nullptr) 
			delete gameState;
		gameState = savedState;
		State * tempState = (gameState);
		dynamic_cast<Running*>(tempState)->FixClock();
		savedState = nullptr;
	}

	else if (newState == StateLose)
	{
		if (gameState != nullptr)
			delete gameState;
		gameState = new SystemPause(&m_window);
	}

	else if (newState == StateRunning)
	{
		if(gameState != nullptr) 
			delete gameState;
		gameState = new Running(&m_window);
	}

	else if (newState == FirstRun)
	{
		(*score) = 0;
		if (gameState != nullptr)
			delete gameState;
		if (savedState != nullptr)
			delete savedState;
		savedState = nullptr;
		gameState = new Menu(&m_window);
	}

	else if (newState == StateOptions)
	{
		delete gameState;
		gameState = new OptionsMenu(&m_window);
	}

	else
	{
		throw "State was not implemented";
	}
	gameState->UpdateGUI(gui);
}