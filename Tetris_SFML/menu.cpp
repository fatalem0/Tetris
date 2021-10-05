#include "menu.h"

// функция, создающая шаблон окна главного меню
// Input: -
// Output: -
Menu::Menu()
{
	// загружаем фон из папки, устанавливаем размер контейнера с фоном и помещаем фон внутрь
	background.loadFromFile("img/backgrounds/title.jpg");
	backgroundRect.setSize(sf::Vector2f(1600.f, 900.f));
	backgroundRect.setTexture(&background);

	// кладём в переменную файл с музыкой
	bgmFile = "music/arigato.ogg";

	// загружаем шрифт
	ffont.loadFromFile("arcade.ttf");

	// переменные для смены кнопок и для смены уровня
	menuCursor = 0;
	levelCursor = 0;

	// создаём контейнер для кнопок
	menuRect.setSize(sf::Vector2f(720.0f, 310.0f));
	menuRect.setFillColor(sf::Color(0, 0, 0, 155));
	menuRect.setPosition(440.0f, 380.0f);
	menuItems.push_back(sf::Text("START AT: level 0", ffont, 50));
	menuItems.push_back(sf::Text("OPTIONS", ffont, 50));
	menuItems.push_back(sf::Text("RATING", ffont, 50));
	
	// создаём контейнер, в котором будет лежать название игры
	titleRect.setSize(sf::Vector2f(720.0f, 100.0f));
	titleRect.setFillColor(sf::Color(0, 0, 0, 155));
	titleRect.setPosition(440.0f, 100.0f);

	// создаём название игры
	title = sf::Text("ANIME-TETRIS", ffont);
	title.setPosition(550.f, 105.f);
	title.setCharacterSize(84);
	title.setFillColor(sf::Color::White);

	// располагаем кнопки относительно друг друга
	for (int i = 0; i < 3; ++i)
	{
		menuItems[i].setFillColor(i ? sf::Color::White : sf::Color::Cyan);
		menuItems[i].setStyle(sf::Text::Bold);
		menuItems[i].setOutlineThickness(2);
		menuItems[i].setOutlineColor(sf::Color::Black);
		menuItems[i].setPosition(450.0f, 400.0f + i*100);
	}
}

// функция, которая проверяет нажатие кнопки вверх
// Input: - 
// Output: -
void Menu::upPressed()
{	
	// если курсор находится в нулевой позиции, и пользователь пытается нажать вверх, то он остаётся на кнопке
	if (--menuCursor < 0) 
		menuCursor = 0;
	// если же пользователь находится не в нулевой позиции, то его курсор поднимается вверх, и данная позиция
	// подсвечивается, а предыдущая кнопка изменяет цвет на первоначальный
	else
	{
		menuItems[menuCursor + 1].setFillColor(sf::Color::White);
		menuItems[menuCursor].setFillColor(sf::Color::Cyan);
	}
}

// функция, которая проверяет нажатие кнопки вниз
// Input: - 
// Output: 0
short Menu::downPressed()
{
	// если курсор находится в последней позиции, и пользователь нажимает кнопку вниз, то он остаётся в данной позиции
	if (++menuCursor > 2)
		menuCursor = 2;
	// в обратном случае он переходит на кнопку вниз, и она подсвечивается, а предыдущая кнопка изменяет цвет на
	// изначальный
	else
	{
		menuItems[menuCursor - 1].setFillColor(sf::Color::White);
		menuItems[menuCursor].setFillColor(sf::Color::Cyan);
	}
	return 0;
}

// функция, которая проверяет нажатие кнопки вправо
// Input: - 
// Output: -
void Menu::rightPressed()
{
	// проверка на нахождения на первой кнопке
	if (menuCursor)
		return;
	// проверка, сдвинулся ли пользователь дальше максимального уровня
	if (++levelCursor > 20)
		levelCursor = 20;
	menuItems[0].setString("START AT: level " + std::to_string(levelCursor));
}

// функция, которая проверяет нажатие кнопки влево
// Input: - 
// Output: -
void Menu::leftPressed()
{	
	// проверка на нахождение на первой кнопке
	if (menuCursor)
		return;
	// проверка, сдвинулся ли пользователь дальше минимального уровня
	if (--levelCursor < 0)
		levelCursor = 0;
	menuItems[0].setString("START AT: level " + std::to_string(levelCursor));
}

// функция проверяет, нажал ли пользователь кнопку Enter
// Input: -
// Output: menuCursor
short Menu::enterPressed()
{
	return menuCursor;
}

// функция для отрисовки окна и всех составляющих главного меню
// Input: 
//		   target - переменная, которая хранит контейнеры для отрисовки
// 
//		   states - так как наши контейнеры не являются объектами высокого уровная, 
//		   то их нужно явно определить, поэтому в этой переменной хранятся 
//		   все настройки контейнеров
//  
// Output: - 
void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(backgroundRect);
	target.draw(menuRect);
	target.draw(titleRect);
	target.draw(title);
	for (sf::Text text : menuItems)
		target.draw(text);
}