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
OptionsMenu::OptionsMenu(sf::RenderWindow * x, Player * birdie) : Menu(0), bird(birdie)
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
			if(selection > 2) selection = selection % 3;
			if (selection == 0) return InputHandlerReasons::None;// Colors
			if (selection == 1) return InputHandlerReasons::None;// Sprite
			if (selection == 2) return InputHandlerReasons::GameEnded;// Back
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
				gui->LoadInterface(false);
			}
			else if (selection == 1)
			{
				bird->NoticeSpriteChanged(false);
			}
		}
		else if (e.key.code == sf::Keyboard::Right)
		{
			if (selection == 0)
			{
				gui->LoadInterface(true);
			}
			else if (selection == 1)
			{
				bird->NoticeSpriteChanged(true);
			}
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
	gui->SetMode(InterfaceManager::InterfaceMode::OptionsMenu);
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

Player::EventType Menu::Logic()
{
	return Player::EventType::ENone;
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
Running::Running(sf::RenderWindow * xwindow, Player * _bird) : obstacles(xwindow), bird(_bird)
{
	bird->SetPosition(sf::Vector2f(50, 300));
	bird->SetObstacleManager(&obstacles);
	bird->ResetVectors();
	gameClock.restart();
}

Running::~Running()
{
}

Player::EventType Running::Logic()
{
	sf::Time deltaTime = gameClock.getElapsedTime();
	Player::EventType out(bird->Logic(deltaTime));
	gameClock.restart();
	return out;
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
			bird->NoticeMove();
		}
	}
	else if (e.type == sf::Event::EventType::MouseButtonPressed)
	{
		bird->NoticeMove();
	}
	return InputHandlerReasons::None;
}

void Running::Draw()
{
	bird->Draw();
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
	bird->ResetVectors();
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

Player::EventType SystemPause::Logic()
{
	return Player::EventType::ENone;
}

State::InputHandlerReasons SystemPause::InputController(sf::Event & e)
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