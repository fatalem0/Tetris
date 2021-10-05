#pragma once
#include <SFML/Graphics.hpp>

// ��������� ��� �������� ���������� ������� ������� � ������� � ���������� ������
struct Point
{
	short row, col;
	Point() : row(0), col(0) {}
	Point(short r, short c) : row(r), col(c) {}

	// �������� ���������� �������� ������� � �������
	Point operator+ (Point other)
	{
		return Point(this->row + other.row, this->col + other.col);
	}

	// �������� ���������� �������� ������� � �������
	Point operator- (Point other)
	{
		return Point(this->row - other.row, this->col - other.col);
	}
};

// �����, ���������������� ������ � �������� ������ ����� ���������
class Block : public sf::Drawable
{
	sf::Texture texture;
	sf::Sprite sprite;

	Point point;

public:
	Block(short r, short c, sf::Color color);
	~Block();

	void setColor(sf::Color color);
	void setPoint(short r, short c);
	void setPositionAt(Point p);

	// ������� ���������� �������� �����
	// Input: -
	// Output: point
	Point getPoint()
	{
		return point;
	}

	// ������� ���������� ���� � ������ �����
	// Input: -
	// Output: sprite.getColor()
	sf::Color getColor()
	{
		return sprite.getColor();
	}

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};