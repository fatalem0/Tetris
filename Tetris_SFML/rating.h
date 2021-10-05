#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include "Interface.h"

// класс для инициализации свойств и методов в окне рейтинга
class Rating : public Interface
{	
	sf::RectangleShape backgroundRect;
	sf::Texture bgImage;

	// инициализируем объекты классов ifstream и ofstream для чтения
	std::ifstream readFs;
	std::ofstream writeFs;

	sf::Font ffont;

	sf::Text ratingHeader;


	sf::RectangleShape scoresRect;
	std::vector<sf::Text> nameTexts;
	std::vector<sf::Text> rankNumberTexts;
	std::vector<sf::Text> linesTexts;
	std::vector<sf::Text> scoreTexts;
	std::vector<std::string> names;
	std::vector<unsigned int> scores;
	std::vector<unsigned int> lines;

	sf::Music bgm;

public:
	Rating();
	short determineRank(unsigned int score); 
	void updateRating(std::string &name, unsigned int score, unsigned int lineNum, short rank);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};