#include "block.h"

// �������, ������� ������ ������ ������ ����� ���������
// Input: 
//		   r - ����� �������, ��� ���������� ���� ���� ���������
//		   c - ����� �������, ��� ���������� ���� ���� ���������
//		   color - ���� ������ ����� ���������
// Output: -
Block::Block(short r, short c, sf::Color color)
{
	// ��������� ��������, ������ ��������� ��� ��, �������� �� ������ ���������� � ����������� �� ������
	texture.loadFromFile("img/mino.png");
	sprite.setTextureRect(sf::IntRect(0, 0, 36, 36));
	sprite.setTexture(texture);
	sprite.setPosition(620.f + c * 36.f, 90.f + r * 36.f);

	// ������������� ���� �����
	sprite.setColor(color);

	// ������������� ��������� ������ ����� ���������
	point = Point(r, c);
}

// ����������, ������� ������ ���� ���� ��������� ����������
Block::~Block()
{
	sprite.setColor(sf::Color::Transparent);
}

// �������, ������� ������������� ������ ���� ������ ����� ���������
// Input: - 
// Output: -
void Block::setColor(sf::Color color)
{
	sprite.setColor(color);
}

// �������, ������� ������������� ������������ ������ ����� ���������
// Input: 
//		   r - ����� �������
//		   c - ����� �������
// Output: -
void Block::setPoint(short r, short c)
{
	point.row = r;
	point.col = c;
}

// �������, ������� ������ ������������ ������ ����� ���������
// Input: p - ����� �������, ���� ������ ������������� ����
// Output: -
void Block::setPositionAt(Point p)
{
	sprite.setPosition(620.f + p.col * 36.f, 90.f + p.row * 36.f);
}

// ������� ��� ��������� ������ ����� ���������
// Input: 
//		   target - ����������, ������� ������ ���������� ��� ���������
// 
//		   states - ��� ��� ���� ���������� �� �������� ��������� �������� �������, 
//		   �� �� ����� ���� ����������, ������� � ���� ���������� �������� 
//		   ��� ��������� �����������
//  
// Output: - 
void Block::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite);
}