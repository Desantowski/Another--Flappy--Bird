#pragma once
#include <vector>
#include <sstream>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Renderable.h"
#include "Player.h"

// Button class
class Button : public MinimalDrawable
{
protected:
	// Pointers for sprite and specific textures
	Sprite * buttonSprite;
	sf::Texture * textureFree;
	sf::Texture * textureSelected;

	// Drawing window and any of textures
	sf::RenderWindow * window;

	// Label of button and current sprite (depends on selected item)
	Text * label;

public:
	Button(sf::Texture * _texture, sf::Texture * _selected, sf::RenderWindow * _window, Text * text);

	virtual void Draw() override;

	// Changes selected / free
	void ChangeState();

	// Setters and getters
	virtual void SetPosition(sf::Vector2f pos);
	sf::Vector2f GetPosition();

	sf::FloatRect GetBounds();
	void SetScale(float x, float y);

	void SetLabel(std::string newlabel);

	enum
	{
		Selected,
		Free
	} State;
};

class ArrowedButton : public Button
{
protected:
	Sprite * leftArrow;
	Sprite * rightArrow;

public:
	ArrowedButton(sf::Texture * _texture, sf::Texture * _selected, sf::Texture * left, sf::Texture * right, sf::RenderWindow * _window, Text * text);
	~ArrowedButton();
	virtual void SetPosition(sf::Vector2f pos) override;
	virtual void Draw() override;
};

// Panel class - background with some texts
class Panel : public Sprite
{
private:
	// For background
	sf::RenderWindow * window;
	sf::Texture * texture;

	// Vector of texts
	std::vector<Text *> label;

	// Sets specific position of given label. Label must be in vector!
	void SetTextPos(Text * tlabel, sf::Vector2f pos);

public:
	Panel(sf::Texture * _texture, sf::RenderWindow * _window, std::vector<Text *> _label);
	void Draw() override;
	// Sets position of panel and labels. Set useAnormal if you want to define your own sizes and spaces between texts. YES, you want
	void SetPosition(sf::Vector2f newpos, bool useAnormal);
};

class InterfaceManager
{
public:
	// Refreshes selected item / score text
	void NoticeSelection(bool goDown);
	void NoticePoint();

	// Modes of drawing
	enum InterfaceMode
	{
		Null,
		MainMenu,
		OptionsMenu,
		SystemPause,
		Pause,
		Running
	};

	// Constructor and destructor
	InterfaceManager(unsigned int _width, unsigned int _height, sf::RenderWindow * _window, Player * _bird);
	~InterfaceManager();

	// Drawing functions - mode of drawing management and drawing function
	void SetMode(InterfaceMode mode);
	void Draw();

	// Pointer to score variable, shared with Game and State
	void SetScorePointer(int * ptr);
	int * GetScorePointer();
	void SetColorPointer(int * ptr);

	// Reloads interface in specified color, in next (if increasing) or previous (if false) color
	void LoadInterface(int colorID);
	void LoadInterface(bool increasing);

private:
	// Size of window, pointer to window
	unsigned int width, height;
	sf::RenderWindow * window;

	// Pointer to player
	Player * bird;

	// Pointers to all drawables of InterfaceManager
	std::vector<Button *> gameObjects;
	Panel * systemPausePanel;

	// Vectors of key<->texture
	std::vector<std::string> keys;
	std::vector<sf::Texture*> textures;

	// Returns pointer to texture from textures by key
	sf::Texture * getTexture(std::string key);

	// Current mode of drawing
	InterfaceMode currentMode;

	// Pointer to kenvector font
	sf::Font * kenvector;

	// Text with scores (while playing) and credits (menu)
	Text * gameText;

	// Score
	int * score;

	// ID of current color of interface
	int * currentColorID; 

	// Determines if player will be drawen as GUI part
	bool drawingPlayer;
};