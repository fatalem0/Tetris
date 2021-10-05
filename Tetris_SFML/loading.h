#pragma once
#include <SFML/Graphics.hpp>
#include "Interface.h"

// класс, инициализирующий методы и свойства экрана загрузки
class Loading : public Interface
{	
	sf::RectangleShape bgRect;
	sf::Texture bgTexture;
	
public:
	Loading(); 
	bool fadeAway(); 
	
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const; 
};