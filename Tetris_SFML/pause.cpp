#include "pause.h"

// �������, ��������� ������ ������ �����
// Input: -
// Output: -
Pause::Pause()
{
	// ��������� �� ����� ���
	image.loadFromFile("img/backgrounds/pause.jpg");

	// ������ ��������� ��� ����
	pauseRect.setSize(sf::Vector2f(1600.f, 900.f));
	pauseRect.setTexture(&image);

	// ��������� ����� ��� ������
	ffont.loadFromFile("arcade.ttf");

	// ������ �����, ������������� ��� ������� �� ������ � ����
	text = sf::Text("It's a chillzone.\nRelax! or\npress ESC to un-pause", ffont);
	text.setPosition(420.f, 100.f);
	text.setFillColor(sf::Color::Black);
}

// ������� ��� ��������� ���� �����
// Input: 
//		   target - ����������, ������� ������ ���������� ��� ���������
// 
//		   states - ��� ��� ���� ���������� �� �������� ��������� �������� �������, 
//		   �� �� ����� ���� ����������, ������� � ���� ���������� �������� 
//		   ��� ��������� �����������
//  
// Output: - 
void Pause::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(pauseRect);
	target.draw(text);
}