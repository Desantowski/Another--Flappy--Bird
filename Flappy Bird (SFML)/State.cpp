#include "State.h"

#pragma region StateAbstract
// GENERAL
State::~State()
{
}

void State::Draw()
{
}

void State::UpdateGUI(InterfaceManager * _gui)
{
	gui = _gui;
	gui->SetMode(InterfaceManager::InterfaceMode::Null);
}
#pragma endregion

#pragma region Menu
#pragma region OptionsMenu
OptionsMenu::OptionsMenu(sf::RenderWindow * x) : Menu(0), suboption(0)
{
}

OptionsMenu::~OptionsMenu()
{
}

State::InputHandlerReasons OptionsMenu::InputController(sf::Event & e)
{
	if (e.type == sf::Event::EventType::Closed) return InputHandlerReasons::Exit;
	if (e.type == sf::Event::EventType::KeyPressed)
	{
		if (e.key.code == sf::Keyboard::Return)
		{
			if (selection < 0 || selection > 2) selection = selection % 3;
			if (selection == 0) return InputHandlerReasons::GameStarted;// Colors
			if (selection == 1) return InputHandlerReasons::ShowOptions;// Bird sprite
			if (selection == 2) return InputHandlerReasons::Exit;// Back
		}
		else if (e.key.code == sf::Keyboard::Up)
		{
			selection--;
			gui->NoticeSelection(false);
		}
		else if (e.key.code == sf::Keyboard::Down)
		{
			selection++;
			gui->NoticeSelection(true);
		}
		else if (e.key.code == sf::Keyboard::Left)
		{
			if(selection == 0)
			{
			}
		}
		else if (e.key.code == sf::Keyboard::Right)
		{
			suboption++;
		}
		else if (e.key.code == sf::Keyboard::Escape)
		{
			return InputHandlerReasons::Exit;
		}
	}
	return InputHandlerReasons::None;
}

void OptionsMenu::UpdateGUI(InterfaceManager * _gui)
{
	gui = _gui;
	gui->SetMode(InterfaceManager::InterfaceMode::MainMenu);
}
#pragma endregion

Menu::Menu(sf::RenderWindow * x) : selection(0)
{
}

Menu::~Menu()
{
}

State::InputHandlerReasons Menu::InputController(sf::Event & e)
{
	if (e.type == sf::Event::EventType::Closed) return InputHandlerReasons::Exit;
	if (e.type == sf::Event::EventType::KeyPressed)
	{
		if (e.key.code == sf::Keyboard::Return)
		{
			if (selection < 0 || selection > 2) selection = selection % 3;
			if (selection == 0) return InputHandlerReasons::GameStarted;
			if (selection == 1) return InputHandlerReasons::ShowOptions;
			if (selection == 2) return InputHandlerReasons::Exit;
		}
		else if (e.key.code == sf::Keyboard::Up)
		{
			selection--;
			gui->NoticeSelection(false);
		}
		else if (e.key.code == sf::Keyboard::Down)
		{
			selection++;
			gui->NoticeSelection(true);
		}
		else if (e.key.code == sf::Keyboard::Escape)
		{
			return InputHandlerReasons::Exit;
		}
	}
	return InputHandlerReasons::None;
}

State::EventType Menu::Logic()
{
	return EventType::ENone;
}

void Menu::UpdateGUI(InterfaceManager * _gui)
{
	gui = _gui;
	gui->SetMode(InterfaceManager::InterfaceMode::MainMenu);
}

void Menu::Draw()
{
	gui->Draw();
}
#pragma endregion

#pragma region Running
Running::Running(sf::RenderWindow * xwindow) : gravityForce(0.f,10.f), playerVelocity(0.f,0.f), playerForce(0.f,5.f), obstacles(xwindow)
{
	playerTexture = new sf::Texture();
	playerTexture->loadFromFile("images/green_bird/frame-4.png");

	playerSprite = new Sprite(xwindow, playerTexture);
	playerSprite->SetScale(0.2, 0.2);
	playerSprite->SetPosition(sf::Vector2f(50, 300));
	gameClock.restart();
}

Running::~Running()
{
	delete playerTexture;
}

State::EventType Running::Logic()
{
	sf::Time deltaTime = gameClock.getElapsedTime();
	gameClock.restart();
	playerVelocity += deltaTime.asSeconds() * gravityForce;
	playerSprite->Move(playerVelocity);

	if (obstacles.MoveAndCheckCollisions(playerSprite) == true) return EventType::ELose;
	if(obstacles.CheckScore(playerSprite) == true) return EventType::EScore;
	else return EventType::ENone;
}

State::InputHandlerReasons Running::InputController(sf::Event & e)
{
	if (e.type == sf::Event::EventType::Closed) return InputHandlerReasons::Exit;
	if (e.type == sf::Event::EventType::KeyPressed)
	{
		if (e.key.code == sf::Keyboard::Escape)
		{
			return InputHandlerReasons::GamePaused;
		}
		else if (e.key.code == sf::Keyboard::Space)
		{
			playerVelocity -= playerForce;
		}
	}
	else if (e.type == sf::Event::EventType::MouseButtonPressed)
	{
		playerVelocity -= gravityForce;
	}
	return InputHandlerReasons::None;
}

void Running::Draw()
{
	playerSprite->Draw();
	obstacles.Draw();
	gui->Draw();
}

void Running::UpdateGUI(InterfaceManager * _gui)
{
	gui = _gui;
	gui->SetMode(InterfaceManager::InterfaceMode::Running);
}

void Running::FixClock()
{
	gameClock.restart();
}
#pragma endregion

#pragma region SystemPause
SystemPause::SystemPause(sf::RenderWindow * xwindow)
{
}

SystemPause::~SystemPause()
{
}

void SystemPause::Draw()
{
	gui->Draw();
}

State::EventType SystemPause::Logic()
{
	return State::ENone;
}

State::InputHandlerReasons SystemPause::InputController(sf::Event & e)
{
	if (e.type == sf::Event::EventType::Closed) return InputHandlerReasons::Exit;
	if (e.type == sf::Event::EventType::KeyPressed)
	{
		if (e.key.code == sf::Keyboard::Escape)
		{
			return InputHandlerReasons::Exit;
		}
		else if (e.key.code == sf::Keyboard::Return)
		{
			return InputHandlerReasons::GameStarted;
		}
	}
	return InputHandlerReasons::None;
}

void SystemPause::UpdateGUI(InterfaceManager * _gui)
{
	gui = _gui;
	gui->SetMode(InterfaceManager::InterfaceMode::SystemPause);
}
#pragma endregion

#pragma region PAUSE
Pause::Pause(sf::RenderWindow * xwindow) : SystemPause(xwindow)
{
}

Pause::~Pause()
{
}

State::InputHandlerReasons Pause::InputController(sf::Event & e)
{
	if (e.type == sf::Event::EventType::Closed) return InputHandlerReasons::Exit;
	if (e.type == sf::Event::EventType::KeyPressed)
	{
		if (e.key.code == sf::Keyboard::Escape)
		{
			return InputHandlerReasons::GameEnded;
		}
		else if (e.key.code == sf::Keyboard::Return)
		{
			return InputHandlerReasons::GameResumed;
		}
	}
	return InputHandlerReasons::None;
}

void Pause::UpdateGUI(InterfaceManager * _gui)
{
	gui = _gui;
	gui->SetMode(InterfaceManager::InterfaceMode::Pause);
}
#pragma endregion