#include "InterfaceManager.h"

	#pragma region InterfaceObjects
		#pragma region Button
Button::Button(sf::Texture * _texture, sf::Texture * _selected, sf::RenderWindow * _window, Text * text) : State(Free), window(_window), label(text)
{
	buttonSprite = new Sprite(_window, _texture);
	textureFree = _texture;
	textureSelected = _selected;
}

void Button::Draw()
{
	buttonSprite->Draw();
	label->Draw();
}

void Button::ChangeState()
{
	if (State == Selected)
	{
		State = Free;
		buttonSprite->SetTexture(textureFree);
	}
	else
	{
		State = Selected;
		buttonSprite->SetTexture(textureSelected);
	}
}

void Button::SetPosition(sf::Vector2f pos)
{
	buttonSprite->SetPosition(pos);
	pos.x += (textureFree->getSize().x - label->GetSize().x)/2;
	label->SetPosition(pos);
}

sf::Vector2f Button::GetPosition()
{
	return buttonSprite->GetPosition();
}

sf::FloatRect Button::GetBounds()
{
	return buttonSprite->GetBounds();
}

void Button::SetScale(float x, float y)
{
	buttonSprite->SetScale(x,y);
	label->SetScale(x, y);
}

void Button::SetLabel(std::string text)
{
	label->SetText(text);
}
		#pragma endregion

		#pragma	region ArrowedButton
ArrowedButton::ArrowedButton(sf::Texture * _texture, sf::Texture * _selected, sf::Texture * left, sf::Texture * right, sf::RenderWindow * _window, Text * text)
	: Button(_texture, _selected, _window, text)
{
	leftArrow = new Sprite(_window, left);
	rightArrow = new Sprite(_window, right);
}

ArrowedButton::~ArrowedButton()
{
	delete rightArrow;
	delete leftArrow;
}

void ArrowedButton::Draw()
{
	Button::Draw();
	leftArrow->Draw();
	rightArrow->Draw();
}

void ArrowedButton::SetPosition(sf::Vector2f pos)
{
	static constexpr float factor = 0.5;

	static constexpr float margin = 10.f;

	Button::SetPosition(pos);

	leftArrow->SetScale(factor, factor);
	rightArrow->SetScale(factor, factor);

	float arrowHeight = leftArrow->GetSize().y;
	float buttonHeight = textureFree->getSize().y;

	float arrowWidth = leftArrow->GetSize().x;
	float buttonRight = GetPosition().x + textureFree->getSize().x;

	arrowHeight = (buttonHeight - arrowHeight) / 2; // Arrow height offset 

	pos.y += arrowHeight;
	pos.x += margin;
	leftArrow->SetPosition(pos);
	
	pos.x = buttonRight - arrowWidth - margin;
	rightArrow->SetPosition(pos);

	label->Move(sf::Vector2f(0, 8));
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

void InterfaceManager::LoadInterface(bool increasing)
{
	if (increasing) LoadInterface(*currentColorID + 1);
	else LoadInterface(*currentColorID - 1);
}

void InterfaceManager::LoadInterface(int colorID)
{
	if (colorID > 2)
		colorID = colorID % 3;
	else if (colorID < 0)
		colorID = (3 * 500 + colorID) % 3;
	std::string Green[3] = { "images/GreenButtonSelected.png","images/GreenButton.png", "images/GreenPanel.png" }; 
	std::string Yellow[3] = { "images/YellowButtonSelected.png","images/YellowButton.png", "images/YellowPanel.png" };
	std::string Blue[3] = { "images/BlueButtonSelected.png","images/BlueButton.png", "images/BluePanel.png" };

	sf::Texture * loader;

	std::string * colored;
	switch (colorID)
	{
	case 1:
		colored = Yellow;
		break;
	case 2:
		colored = Blue;
		break;

	default:
		colored = Green;
	}

	loader = getTexture("ButtonSelected");
	loader->loadFromFile(colored[0]);

	loader = getTexture("Button");
	loader->loadFromFile(colored[1]);

	loader = getTexture("Panel");
	loader->loadFromFile(colored[2]);

	*currentColorID = colorID;
}

InterfaceManager::InterfaceManager(unsigned int _width, unsigned int _height, sf::RenderWindow * _window, Player * _bird) : width(_width), height(_height), window(_window), bird(_bird)
{
	drawingPlayer = false;
	systemPausePanel = nullptr;
	gameText = nullptr;

	kenvector = new sf::Font();
	kenvector->loadFromFile("fonts/kenvector_future.ttf");

	sf::Texture * loader;

	loader = new sf::Texture();
	loader->loadFromFile("images/GreenButtonSelected.png");
	textures.push_back(loader);
	keys.push_back("ButtonSelected");

	loader = new sf::Texture();
	loader->loadFromFile("images/GreenButton.png");
	textures.push_back(loader);
	keys.push_back("Button");

	loader = new sf::Texture();
	loader->loadFromFile("images/GreenPanel.png");
	textures.push_back(loader);
	keys.push_back("Panel");

	loader = new sf::Texture();
	loader->loadFromFile("images/LeftArrow.png");
	textures.push_back(loader);
	keys.push_back("LeftArrow");

	loader = new sf::Texture();
	loader->loadFromFile("images/RightArrow.png");
	textures.push_back(loader);
	keys.push_back("RightArrow");
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
	drawingPlayer = false;
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

	else if (mode == InterfaceMode::OptionsMenu)
	{
		sf::Texture * buttonFree = getTexture("Button");
		sf::Texture * buttonSelected = getTexture("ButtonSelected");
		sf::Texture * leftArrow = getTexture("LeftArrow");
		sf::Texture * rightArrow = getTexture("RightArrow");

		// Coordinates of center of window
		sf::Vector2f posv(width / 2, height / 2);

		// Position where center of window = center of button
		posv.x -= buttonFree->getSize().x / 2;
		posv.y -= buttonFree->getSize().y / 2;


		posv.y += 20;
		// TODO: recalculate and check position of sprite
		bird->SetPosition(sf::Vector2f(330, 150));

		Text * play_label;
		play_label = new Text(window, kenvector, "Kolor UI");
		play_label->SetOutlineThickness(2.0);
		play_label->SetSize(20);
		ArrowedButton * play = new ArrowedButton(buttonFree, buttonSelected, leftArrow, rightArrow, window, play_label);
		play->SetPosition(posv);
		play->ChangeState();
		gameObjects.push_back(play);

		play_label = new Text(window, kenvector, "Postac");
		play_label->SetOutlineThickness(2.0);
		play_label->SetSize(20);
		play = new ArrowedButton(buttonFree, buttonSelected, leftArrow, rightArrow, window, play_label);
		posv.y +=  60;
		play->SetPosition(posv);
		gameObjects.push_back(play);

		posv.y += 120;
		play_label = new Text(window, kenvector, "Powrot");
		play_label->SetOutlineThickness(2.0);
		play_label->SetSize(30);
		Button * play2 = new Button(buttonFree, buttonSelected, window, play_label);
		play2->SetPosition(posv);
		gameObjects.push_back(play2);

		drawingPlayer = true;
	}

	else if (mode == InterfaceMode::MainMenu)
	{

		sf::Texture * buttonFree = getTexture("Button");
		sf::Texture * buttonSelected = getTexture("ButtonSelected");

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
		play->SetPosition(posv);
		play->ChangeState();
		gameObjects.push_back(play);

	
		posv.y += 80;
		play_label = new Text(window, kenvector, "Opcje");
		play_label->SetOutlineThickness(2.0);
		play_label->SetSize(30);
		play = new Button(buttonFree, buttonSelected, window, play_label);
		play->SetPosition(posv);
		gameObjects.push_back(play);
		

		posv.y += 80;
		play_label = new Text(window, kenvector, "Wyjscie");
		play_label->SetOutlineThickness(2.0);
		play_label->SetSize(30);
		play = new Button(buttonFree, buttonSelected, window, play_label);
		play->SetPosition(posv);
		gameObjects.push_back(play);

		posv.y = 540;
		posv.x = 10;
		play_label = new Text(window, kenvector, "Grafika: opengameart.com, bevouliin.com\nCzcionki: oepngameart.com (Kenney)\nKod: desantowski");
		play_label->SetOutlineThickness(1.0);
		play_label->SetPosition(posv);
		play_label->SetSize(15);
		gameText = play_label;
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

		systemPausePanel = new Panel(getTexture("Panel"), window, labels);
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

		systemPausePanel = new Panel(getTexture("Panel"),window,labels);
		systemPausePanel->SetPosition(sf::Vector2f(width / 2, height / 2));
	}

	else if (mode == InterfaceMode::Running)
	{
		std::string pointsstring("Score: ");
		pointsstring.append(std::to_string(*score));

		if (gameText != nullptr)
			delete gameText;

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
	if (drawingPlayer)
		bird->Draw();
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

void InterfaceManager::SetColorPointer(int * ptr)
{
	currentColorID = ptr;
}

int * InterfaceManager::GetScorePointer()
{
	return score;
}