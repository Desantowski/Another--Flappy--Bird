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

int Game::Load()
{
	int(*GetDec)(char x) = [](char x)->int { if (x < 58) return static_cast<int>(x - 48); else return static_cast<int>(x - 65); };
	std::fstream data;
	data.open("data1.bin",std::ios::in);
	if (data.is_open() == false)
	{
		std::cerr << "Cannot load data binaries\n";
		interfaceColor = 0;
		globalScore = 0;
		spriteID = 0;
		return 0;
	}

	char loadingChar;
	for(int i = 0; i < 10; i++)
		data >> loadingChar;
	data >> loadingChar;
	interfaceColor = GetDec(loadingChar);

	int loadingInt;
	data >> std::hex >> loadingInt;
	globalScore = loadingInt;
	data >> std::hex >> loadingInt;
	spriteID = loadingInt;
	return interfaceColor;
}

void Game::Save()
{
	char(*RandomHex)() = []()->char { int x = rand() % 16; if (x < 10) return static_cast<char>(x + 48); else return static_cast<char>(x % 9 + 65); };
	char(*GetHex)(int x) = [](int x)->char { if (x < 10) return static_cast<char>(x + 48); else return static_cast<char>(x % 9 + 65);  };
	std::fstream data;
	data.open("data1.bin",std::ios::out);
	if (data.is_open() == false)
	{
		std::cerr << "Cannot open data binaries\n";
		return;
	}
	for (int i = 0; i < 10; i++)
	{
		// Random hex numbers
		data << RandomHex();
	}
	data << GetHex(interfaceColor);
	data << ' ';
	data << std::hex << globalScore;
	data << ' ';
	data << std::hex << spriteID;
	data.close();
}

void Game::Run()
{

	sf::Texture backgroundTexture;
	backgroundTexture.loadFromFile("images/background.png");

	sf::Sprite backgroundSprite(backgroundTexture);
	backgroundSprite.setPosition(sf::Vector2f(0, 0));

	bird = new Player(&m_window);

	gui = new InterfaceManager(width, height, &m_window, bird);

	sf::Event windowEvent;
	gui->SetScorePointer(score);
	gui->SetColorPointer(&interfaceColor);
	gui->LoadInterface(Load());


	bird->ForceSprite(&spriteID);

	ChangeState(FirstRun);

	bool forceBreak = false;
	while (m_window.isOpen())
	{
		m_window.clear();
		m_window.draw(backgroundSprite);
		gameState->Draw();
		m_window.display();

		while (m_window.pollEvent(windowEvent))
			if (HandleEvent(windowEvent) == false)
			{
				forceBreak = true;
				break;
			}

		if (forceBreak == true)
			break;

		switch(gameState->Logic())
		{
		case Player::EventType::ELose:
			ChangeState(StateList::StateLose);
			break;
		case Player::EventType::EScore:
			(*score)++;
			globalScore++;
			gui->NoticePoint();
			break;
		default:
			break;
		}
	}
	Save();
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
		ChangeState(StateOptions);
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
		gameState = new Running(&m_window,bird);
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
		gameState = new OptionsMenu(&m_window,bird);
	}

	else
	{
		throw "State was not implemented";
	}
	gameState->UpdateGUI(gui);
}