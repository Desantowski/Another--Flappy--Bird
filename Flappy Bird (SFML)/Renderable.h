#pragma once
#include <SFML/Graphics.hpp>

// Smallest interface which provides only drawing - for typical "Drawables" and for example sets of connected sprites, in which it could be hard to \
	 implement functions like GetPosition
class MinimalDrawable
{
public:
	virtual ~MinimalDrawable();
	virtual void Draw() = 0;
};

// Drawable does mean every basic visible item
class Drawable : public MinimalDrawable
{
public:
	virtual ~Drawable();
	virtual void Draw() = 0;
	virtual void Move(sf::Vector2f offset) = 0;


	//Setters and getters
	virtual void SetPosition(sf::Vector2f pos) = 0;
	virtual sf::Vector2f GetPosition() = 0;

	virtual void SetScale(float x, float y) = 0;
	virtual sf::Vector2f GetScale() = 0;

	virtual sf::FloatRect GetBounds() = 0;
	virtual sf::Vector2f GetSize() = 0;
};

class Sprite : public Drawable
{
	// TODO: find and/or implement pointers, which values cannot be deleted by operator delete to avoid \
		 accidential deleting global textures or windows from class like Sprite
protected:
	// Protected pointers
	// sf::Sprite is created in Sprite class, it's fully invisible outside!
	sf::Sprite * sprite;

	// RenderWindow is pointer to window, which should be managed outside. Used only for drawing
	sf::RenderWindow * window;

	// Pointer to texture - textures are shared, which means that texture isn't loaded in Sprite class and it wouldn't be deleted in Sprite class
	sf::Texture * texture;

public:
	Sprite(sf::RenderWindow * xwindow, sf::Texture * text);
	virtual ~Sprite();
	virtual void Draw() override;
	virtual void Move(sf::Vector2f offset) override;

	virtual void SetTexture(sf::Texture * texture);

	// Setters and getters
	virtual void SetPosition(sf::Vector2f pos) override;
	virtual sf::Vector2f GetPosition() override;

	virtual void SetScale(float x, float y) override;
	virtual sf::Vector2f GetScale() override;

	virtual sf::FloatRect GetBounds() override;
	virtual sf::Vector2f GetSize() override;
};

class Text : public Drawable
{
	// TODO: see: class Sprite
protected:
	// Protected pointers
	// text is local, hidden sf::Text, managed only from this class
	sf::Text * text;

	// window and font are analogical for window and texture in Sprite - they are only pointers to variables, that should be stored in classes like \
		TextureManager
	sf::RenderWindow * window;
	sf::Font * font;

	// Stores number of characters in label
	int length;
public:
	Text(sf::RenderWindow * xwindow, sf::Font * xfont, std::string xtext);
	virtual ~Text();
	virtual void Draw() override;
	virtual void Move(sf::Vector2f offset) override;

	// Setters and getters
	void SetSize(unsigned int size);
	virtual void SetPosition(sf::Vector2f pos) override;
	virtual sf::Vector2f GetPosition() override;

	virtual void SetScale(float x, float y) override;
	virtual sf::Vector2f GetScale() override;

	virtual sf::FloatRect GetBounds() override;
	virtual sf::Vector2f GetSize() override;

	// Text properties and methods
	// Returns private int length 
	int Length();
	// Sets properties of text, equivalent with sf::Text
	void SetColor(sf::Color color);
	void SetOutlineColor(sf::Color color);
	void SetOutlineThickness(float size);
	void SetText(std::string _text);
};