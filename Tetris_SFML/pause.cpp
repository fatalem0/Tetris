#include "pause.h"

// функция, создающая шаблон экрана паузы
// Input: -
// Output: -
Pause::Pause()
{
	// загружаем из папки фон
	image.loadFromFile("img/backgrounds/pause.jpg");

	// создаём контейнер для фона
	pauseRect.setSize(sf::Vector2f(1600.f, 900.f));
	pauseRect.setTexture(&image);

	// загружаем шрифт для текста
	ffont.loadFromFile("arcade.ttf");

	// создаём текст, устанавливаем его позицию на экране и цвет
	text = sf::Text("It's a chillzone.\nRelax! or\npress ESC to un-pause", ffont);
	text.setPosition(420.f, 100.f);
	text.setFillColor(sf::Color::Black);
}

// функция для отрисовки окна паузы
// Input: 
//		   target - переменная, которая хранит контейнеры для отрисовки
// 
//		   states - так как наши контейнеры не являются объектами высокого уровная, 
//		   то их нужно явно определить, поэтому в этой переменной хранятся 
//		   все настройки контейнеров
//  
// Output: - 
void Pause::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(pauseRect);
	target.draw(text);
}