#include "Renderable.h"

MinimalDrawable::~MinimalDrawable()
{
}

Drawable::~Drawable()
{
}

// SPRITE
Sprite::Sprite(sf::RenderWindow * xwindow, sf::Texture * text) : window(xwindow)
{
	texture = text;
	sprite = new sf::Sprite(*texture);
}

Sprite::~Sprite()
{
	delete sprite;
}

void Sprite::Draw()
{
	window->draw(*sprite);
}

void Sprite::SetPosition(sf::Vector2f pos)
{
	sprite->setPosition(pos);
}

sf::Vector2f Sprite::GetPosition()
{
	return sprite->getPosition();
}

sf::FloatRect Sprite::GetBounds()
{
	return sprite->getGlobalBounds();
}

void Sprite::SetScale(float x, float y)
{
	sprite->setScale(sf::Vector2f(x, y));
}

sf::Vector2f Sprite::GetScale()
{
	return sprite->getScale();
}

sf::Vector2f Sprite::GetSize()
{
	return sf::Vector2f(sprite->getGlobalBounds().width, sprite->getGlobalBounds().height);
}

void Sprite::Move(sf::Vector2f offset)
{
	sprite->move(offset);
}

// TEXT
Text::Text(sf::RenderWindow * xwindow, sf::Font * xfont, std::string xtext) : window(xwindow), font(xfont)
{
	text = new sf::Text(sf::String(xtext), *font);
	length = xtext.length();
	SetColor(sf::Color::White);
	SetOutlineColor(sf::Color::Black);
}

Text::~Text()
{
	delete text;
}

void Text::SetSize(unsigned int size)
{
	text->setCharacterSize(size);
}

void Text::Draw()
{
	window->draw(*text);
}

void Text::SetPosition(sf::Vector2f pos)
{
	text->setPosition(pos);
}

sf::Vector2f Text::GetPosition()
{
	return text->getPosition();
}

sf::FloatRect Text::GetBounds()
{
	return text->getGlobalBounds();
}

void Text::SetScale(float x, float y)
{
	text->setScale(sf::Vector2f(x, y));
}

sf::Vector2f Text::GetScale()
{
	return text->getScale();
}

void Text::SetColor(sf::Color color)
{
	text->setColor(color);
}

void Text::SetOutlineColor(sf::Color color)
{
	text->setOutlineColor(color);
}

void Text::SetText(std::string _text)
{
	text->setString(sf::String(_text));
}

void Text::SetOutlineThickness(float size)
{
	text->setOutlineThickness(size);
}

void Text::Move(sf::Vector2f offset)
{
	text->move(offset);
}

sf::Vector2f Text::GetSize()
{
	return sf::Vector2f(text->getGlobalBounds().width, text->getGlobalBounds().height);
}

int Text::Length()
{
	return length;
}