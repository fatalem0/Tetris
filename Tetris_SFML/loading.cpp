#include "loading.h"

// функция загрузки экрана
// Input: -
// Output: -
Loading::Loading()
{	
	// загружаем фон для экрана из папки, загружаем его в форму и устанавливаем размер
	bgTexture.loadFromFile("img/backgrounds/loading.jpg");
	bgRect.setTexture(&bgTexture);
	bgRect.setSize(sf::Vector2f(1600.f, 900.f));
}

// функция угасания экрана
// Input: -
// Output: делаем экран загрузки прозрачным
bool Loading::fadeAway()
{
	// получаем изначальный фон загрузки, и после делаем его прозрачным
	sf::Color bgColor = bgRect.getFillColor();
	bgRect.setFillColor(sf::Color(bgColor.r, bgColor.g, bgColor.b, bgColor.a - 5));

	return bgRect.getFillColor().a == 0;
}

// функция для отрисовки окна загрузки
// Input: 
//		   target - переменная, которая хранит контейнеры для отрисовки
// 
//		   states - так как наши контейнеры не являются объектами высокого уровная, 
//		   то их нужно явно определить, поэтому в этой переменной хранятся 
//		   все настройки контейнеров
//  
// Output: - 
void Loading::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(bgRect);
}