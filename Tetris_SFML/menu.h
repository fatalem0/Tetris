#pragma once
#include "Interface.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

// класс, инициализирующий методы и свойства главного меню
class Menu : public Interface
{
	sf::Texture background;
	sf::RectangleShape backgroundRect;

	std::string bgmFile;
	
	sf::Text title;

	sf::Font ffont;

	sf::RectangleShape titleRect;
	sf::RectangleShape menuRect;
	std::vector<sf::Text> menuItems;

	short menuCursor;
	short levelCursor;
	
public:
	Menu();

	void upPressed() override;
	short downPressed() override;
	void leftPressed() override;
	void rightPressed() override;
	short enterPressed() override;

	std::string& getMusic()
	{
		return bgmFile;
	}

	short getLevelCursor()
	{
		return levelCursor;
	}

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};