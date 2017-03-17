#include "InterfaceManager.h"

	#pragma region InterfaceObjects
		#pragma region Button
Button::Button(sf::Texture * _texture, sf::Texture * _selected, sf::RenderWindow * _window, Text * text) : State(Free), window(_window), label(text)
{
	selected = new Sprite(_window, _selected);
	free = new Sprite(_window, _texture);
	current = free;
	texture = _texture;
}

void Button::Draw()
{
	current->Draw();
	label->Draw();
}

void Button::ChangeState()
{
	if (State == Selected)
	{
		State = Free;
		current = free;
	}
	else
	{
		State = Selected;
		current = selected;
	}
}

void Button::SetLengthInfo(int l)
{
	length = l;
}

void Button::SetPosition(sf::Vector2f pos)
{
	free->SetPosition(pos);
	selected->SetPosition(pos);
	pos.x += (texture->getSize().x - label->GetSize().x)/2;
	label->SetPosition(pos);
}

sf::Vector2f Button::GetPosition()
{
	return free->GetPosition();
}

sf::FloatRect Button::GetBounds()
{
	return free->GetBounds();
}

void Button::SetScale(float x, float y)
{
	free->SetScale(x,y);
	selected->SetScale(x, y);
	label->SetScale(x, y);
}
		#pragma endregion

Panel::Panel(sf::Texture * _texture, sf::RenderWindow * _window, std::vector<Text *> _label) : Sprite(_window, _texture), label(_label), texture(_texture)
{
	Sprite::SetScale(3.0, 3.0);
}

void Panel::Draw()
{
	Sprite::Draw();
	for (Text * x : label)
		x->Draw();
}

void Panel::SetPosition(sf::Vector2f newpos, bool useAnormal = false)
{
	sf::Vector2f temppos(newpos);
	newpos.x -= texture->getSize().x * Sprite::GetScale().x / 2;
	newpos.y -= texture->getSize().y * Sprite::GetScale().y / 2;
	Sprite::SetPosition(newpos);

	int fixHeight;
	if (!useAnormal)
	{
		switch (label.size())
		{
		case 3:
			fixHeight = 80;
			break;
		default:
			fixHeight = 0;
			break;
		}

		temppos.y -= fixHeight;
		for (Text * x : label)
		{
			SetTextPos(x, temppos);
			temppos.y += fixHeight;
		}
	}
	else
	{

		temppos.y -= 100;

		// Definitions
		// [Enter x2, Exit x2, Pause, Score] => [Enter x2, Pause, Score, Exit x2]
		SetTextPos(label[0], temppos + sf::Vector2f(0, 0));
		SetTextPos(label[1], temppos + sf::Vector2f(0, 30));

		SetTextPos(label[4], temppos + sf::Vector2f(0, 80));
		SetTextPos(label[5], temppos + sf::Vector2f(0, 110));

		SetTextPos(label[2], temppos + sf::Vector2f(0, 160));
		SetTextPos(label[3], temppos + sf::Vector2f(0, 190));

	}
}

void Panel::SetTextPos(Text * tlabel, sf::Vector2f pos)
{
	pos.x -= tlabel->GetSize().x / 2;
	pos.y -= tlabel->GetSize().y / 2;
	tlabel->SetPosition(pos);
}
	#pragma endregion

InterfaceManager::InterfaceManager(unsigned int _width, unsigned int _height, sf::RenderWindow * _window) : width(_width), height(_height), window(_window)
{
	sf::Texture * loader;

	systemPausePanel = nullptr;
	gameText = nullptr;

	kenvector = new sf::Font();
	kenvector->loadFromFile("fonts/kenvector_future.ttf");

	loader = new sf::Texture();
	loader->loadFromFile("images/GreenButtonSelected.png");
	textures.push_back(loader);
	keys.push_back("GreenButtonSelected");

	loader = new sf::Texture();
	loader->loadFromFile("images/GreenButton.png");
	textures.push_back(loader);
	keys.push_back("GreenButton");

	loader = new sf::Texture();
	loader->loadFromFile("images/GreenPanel.png");
	textures.push_back(loader);
	keys.push_back("GreenPanel");
}

InterfaceManager::~InterfaceManager()
{
	for (MinimalDrawable * x : gameObjects)
		delete x;
	gameObjects.clear();

	for (sf::Texture * x : textures)
		delete x;
	textures.clear();
}

void InterfaceManager::SetMode(InterfaceManager::InterfaceMode mode)
{
	if (currentMode == mode) return;

	for (MinimalDrawable * x : gameObjects)
		delete x;
	gameObjects.clear();

	if (gameText != nullptr)
		delete gameText;
	if (systemPausePanel != nullptr)
		delete systemPausePanel;

	gameText = nullptr;
	systemPausePanel = nullptr;

	currentMode = mode;

	if (mode == InterfaceMode::Null)
		return;

	else if (mode == InterfaceMode::MainMenu)
	{

		sf::Texture * buttonFree = getTexture("GreenButton");
		sf::Texture * buttonSelected = getTexture("GreenButtonSelected");

		// Coordinates of center of window
		sf::Vector2f posv(width / 2, height / 2);

		// Position where center of window = center of button
		posv.x -= buttonFree->getSize().x / 2;
		posv.y -= buttonFree->getSize().y / 2;

		posv.y -= 80;

		Text * play_label = new Text(window, kenvector, "Graj");
		play_label->SetOutlineThickness(2.0);
		play_label->SetSize(30);
		Button * play = new Button(buttonFree, buttonSelected, window, play_label);
		play->SetLengthInfo(4);
		play->SetPosition(posv);
		play->ChangeState();
		gameObjects.push_back(play);

	
		posv.y += 80;
		play_label = new Text(window, kenvector, "Opcje");
		play_label->SetOutlineThickness(2.0);
		play_label->SetSize(30);
		play = new Button(buttonFree, buttonSelected, window, play_label);
		play->SetLengthInfo(7);
		play->SetPosition(posv);
		gameObjects.push_back(play);
		

		posv.y += 80;
		play_label = new Text(window, kenvector, "Wyjscie");
		play_label->SetOutlineThickness(2.0);
		play_label->SetSize(30);
		play = new Button(buttonFree, buttonSelected, window, play_label);
		play->SetLengthInfo(4);
		play->SetPosition(posv);
		gameObjects.push_back(play);
	}

	else if (mode == InterfaceMode::Pause)
	{
		std::vector<Text*> labels;

		std::string pointsstring("Score: ");
		pointsstring.append(std::to_string(*score));


		// Definitions
		// [Enter x2, Exit x2, Pause, Score]
		Text * pauseLabel = new Text(window, kenvector, "[Enter]");
		pauseLabel->SetOutlineThickness(2.0);
		pauseLabel->SetSize(20);
		labels.push_back(pauseLabel);
		pauseLabel = new Text(window, kenvector, "(powrot)");
		pauseLabel->SetOutlineThickness(2.0);
		pauseLabel->SetSize(20);
		labels.push_back(pauseLabel);

		pauseLabel = new Text(window, kenvector, "[Escape]");
		pauseLabel->SetOutlineThickness(2.0);
		pauseLabel->SetSize(20);
		labels.push_back(pauseLabel);
		pauseLabel = new Text(window, kenvector, "(menu)");
		pauseLabel->SetOutlineThickness(2.0);
		pauseLabel->SetSize(20);
		labels.push_back(pauseLabel);

		pauseLabel = new Text(window, kenvector, "Zatrzymano");
		pauseLabel->SetOutlineThickness(2.0);
		pauseLabel->SetSize(30);
		labels.push_back(pauseLabel);

		pauseLabel = new Text(window, kenvector, pointsstring);
		pauseLabel->SetOutlineThickness(2.0);
		pauseLabel->SetSize(15);
		labels.push_back(pauseLabel);

		systemPausePanel = new Panel(getTexture("GreenPanel"), window, labels);
		systemPausePanel->SetPosition(sf::Vector2f(width / 2, height / 2), true);
	}
	
	else if (mode == InterfaceMode::SystemPause)
	{
		std::vector<Text*> labels;

		std::string pointsstring("Score: ");
		pointsstring.append(std::to_string(*score));

		Text * pauseLabel = new Text(window, kenvector, "Przegrales");
		pauseLabel->SetOutlineThickness(2.0);
		pauseLabel->SetSize(20);
		labels.push_back(pauseLabel);

		pauseLabel = new Text(window, kenvector, pointsstring);
		pauseLabel->SetOutlineThickness(2.0);
		pauseLabel->SetSize(30);
		labels.push_back(pauseLabel);

		pauseLabel = new Text(window, kenvector, "[Enter]");
		pauseLabel->SetOutlineThickness(2.0);
		pauseLabel->SetSize(20);
		labels.push_back(pauseLabel);

		systemPausePanel = new Panel(getTexture("GreenPanel"),window,labels);
		systemPausePanel->SetPosition(sf::Vector2f(width / 2, height / 2));
	}

	else if (mode == InterfaceMode::Running)
	{
		std::string pointsstring("Score: ");
		pointsstring.append(std::to_string(*score));

		gameText = new Text(window, kenvector, pointsstring);
		gameText->SetPosition(sf::Vector2f(50, 50));
		gameText->SetOutlineThickness(2.0);
		gameText->SetSize(30);
	}
}

sf::Texture * InterfaceManager::getTexture(std::string key)
{
	int counter = 0;
	for (std::string & x : keys)
	{
		if (x == key)
			return textures[counter];
		counter++;
	}
	return nullptr;
}

void InterfaceManager::Draw()
{
	for (MinimalDrawable * x : gameObjects)
		x->Draw();
	if (systemPausePanel != nullptr)
		systemPausePanel->Draw();
	if (gameText != nullptr)
		gameText->Draw();
}

void InterfaceManager::NoticePoint()
{
	std::string pointsstring("Score: ");
	pointsstring.append(std::to_string(*score));
	gameText->SetText(pointsstring);
}

void InterfaceManager::NoticeSelection(bool goDown)
{
	for (unsigned int i = 0; i < gameObjects.size(); i++)
	{
		if (gameObjects[i]->State == Button::Selected)
		{
			gameObjects[i]->ChangeState();
			if (goDown)
			{
				if (i == gameObjects.size() - 1) gameObjects[0]->ChangeState();
				else gameObjects[i + 1]->ChangeState();
			}
			else
			{
				if (i == 0) gameObjects[gameObjects.size()-1]->ChangeState();
				else gameObjects[i - 1]->ChangeState();
			}
			return;
		}
	}
}

void InterfaceManager::SetScorePointer(int * ptr)
{
	score = ptr;
}

int * InterfaceManager::GetScorePointer()
{
	return score;
}