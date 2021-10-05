#include "menu.h"

// �������, ��������� ������ ���� �������� ����
// Input: -
// Output: -
Menu::Menu()
{
	// ��������� ��� �� �����, ������������� ������ ���������� � ����� � �������� ��� ������
	background.loadFromFile("img/backgrounds/title.jpg");
	backgroundRect.setSize(sf::Vector2f(1600.f, 900.f));
	backgroundRect.setTexture(&background);

	// ����� � ���������� ���� � �������
	bgmFile = "music/arigato.ogg";

	// ��������� �����
	ffont.loadFromFile("arcade.ttf");

	// ���������� ��� ����� ������ � ��� ����� ������
	menuCursor = 0;
	levelCursor = 0;

	// ������ ��������� ��� ������
	menuRect.setSize(sf::Vector2f(720.0f, 310.0f));
	menuRect.setFillColor(sf::Color(0, 0, 0, 155));
	menuRect.setPosition(440.0f, 380.0f);
	menuItems.push_back(sf::Text("START AT: level 0", ffont, 50));
	menuItems.push_back(sf::Text("OPTIONS", ffont, 50));
	menuItems.push_back(sf::Text("RATING", ffont, 50));
	
	// ������ ���������, � ������� ����� ������ �������� ����
	titleRect.setSize(sf::Vector2f(720.0f, 100.0f));
	titleRect.setFillColor(sf::Color(0, 0, 0, 155));
	titleRect.setPosition(440.0f, 100.0f);

	// ������ �������� ����
	title = sf::Text("ANIME-TETRIS", ffont);
	title.setPosition(550.f, 105.f);
	title.setCharacterSize(84);
	title.setFillColor(sf::Color::White);

	// ����������� ������ ������������ ���� �����
	for (int i = 0; i < 3; ++i)
	{
		menuItems[i].setFillColor(i ? sf::Color::White : sf::Color::Cyan);
		menuItems[i].setStyle(sf::Text::Bold);
		menuItems[i].setOutlineThickness(2);
		menuItems[i].setOutlineColor(sf::Color::Black);
		menuItems[i].setPosition(450.0f, 400.0f + i*100);
	}
}

// �������, ������� ��������� ������� ������ �����
// Input: - 
// Output: -
void Menu::upPressed()
{	
	// ���� ������ ��������� � ������� �������, � ������������ �������� ������ �����, �� �� ������� �� ������
	if (--menuCursor < 0) 
		menuCursor = 0;
	// ���� �� ������������ ��������� �� � ������� �������, �� ��� ������ ����������� �����, � ������ �������
	// ��������������, � ���������� ������ �������� ���� �� ��������������
	else
	{
		menuItems[menuCursor + 1].setFillColor(sf::Color::White);
		menuItems[menuCursor].setFillColor(sf::Color::Cyan);
	}
}

// �������, ������� ��������� ������� ������ ����
// Input: - 
// Output: 0
short Menu::downPressed()
{
	// ���� ������ ��������� � ��������� �������, � ������������ �������� ������ ����, �� �� ������� � ������ �������
	if (++menuCursor > 2)
		menuCursor = 2;
	// � �������� ������ �� ��������� �� ������ ����, � ��� ��������������, � ���������� ������ �������� ���� ��
	// �����������
	else
	{
		menuItems[menuCursor - 1].setFillColor(sf::Color::White);
		menuItems[menuCursor].setFillColor(sf::Color::Cyan);
	}
	return 0;
}

// �������, ������� ��������� ������� ������ ������
// Input: - 
// Output: -
void Menu::rightPressed()
{
	// �������� �� ���������� �� ������ ������
	if (menuCursor)
		return;
	// ��������, ��������� �� ������������ ������ ������������� ������
	if (++levelCursor > 20)
		levelCursor = 20;
	menuItems[0].setString("START AT: level " + std::to_string(levelCursor));
}

// �������, ������� ��������� ������� ������ �����
// Input: - 
// Output: -
void Menu::leftPressed()
{	
	// �������� �� ���������� �� ������ ������
	if (menuCursor)
		return;
	// ��������, ��������� �� ������������ ������ ������������ ������
	if (--levelCursor < 0)
		levelCursor = 0;
	menuItems[0].setString("START AT: level " + std::to_string(levelCursor));
}

// ������� ���������, ����� �� ������������ ������ Enter
// Input: -
// Output: menuCursor
short Menu::enterPressed()
{
	return menuCursor;
}

// ������� ��� ��������� ���� � ���� ������������ �������� ����
// Input: 
//		   target - ����������, ������� ������ ���������� ��� ���������
// 
//		   states - ��� ��� ���� ���������� �� �������� ��������� �������� �������, 
//		   �� �� ����� ���� ����������, ������� � ���� ���������� �������� 
//		   ��� ��������� �����������
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