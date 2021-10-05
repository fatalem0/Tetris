#pragma once
#include "Interface.h"
#include <SFML/Graphics.hpp>

// �����, ������� �������������� ������ � �������� ������ �����
class Pause : public Interface
{
	sf::RectangleShape pauseRect;
	sf::Texture image;
	sf::Text text;
	sf::Font ffont;

public:
	Pause();

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};