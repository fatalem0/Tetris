#include "loading.h"

// ������� �������� ������
// Input: -
// Output: -
Loading::Loading()
{	
	// ��������� ��� ��� ������ �� �����, ��������� ��� � ����� � ������������� ������
	bgTexture.loadFromFile("img/backgrounds/loading.jpg");
	bgRect.setTexture(&bgTexture);
	bgRect.setSize(sf::Vector2f(1600.f, 900.f));
}

// ������� �������� ������
// Input: -
// Output: ������ ����� �������� ����������
bool Loading::fadeAway()
{
	// �������� ����������� ��� ��������, � ����� ������ ��� ����������
	sf::Color bgColor = bgRect.getFillColor();
	bgRect.setFillColor(sf::Color(bgColor.r, bgColor.g, bgColor.b, bgColor.a - 5));

	return bgRect.getFillColor().a == 0;
}

// ������� ��� ��������� ���� ��������
// Input: 
//		   target - ����������, ������� ������ ���������� ��� ���������
// 
//		   states - ��� ��� ���� ���������� �� �������� ��������� �������� �������, 
//		   �� �� ����� ���� ����������, ������� � ���� ���������� �������� 
//		   ��� ��������� �����������
//  
// Output: - 
void Loading::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(bgRect);
}