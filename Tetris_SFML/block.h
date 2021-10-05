#pragma once
#include <SFML/Graphics.hpp>

// структура для хранения переменных номеров столбца и строчки и операторов сдвига
struct Point
{
	short row, col;
	Point() : row(0), col(0) {}
	Point(short r, short c) : row(r), col(c) {}

	// оператор увеличения значения столбца и строчки
	Point operator+ (Point other)
	{
		return Point(this->row + other.row, this->col + other.col);
	}

	// оператор уменьшения значения столбца и строчки
	Point operator- (Point other)
	{
		return Point(this->row - other.row, this->col - other.col);
	}
};

// класс, инициализирующий методы и свойства одного блока тетрамино
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

	// функция возвращает значение точки
	// Input: -
	// Output: point
	Point getPoint()
	{
		return point;
	}

	// функция возвращает цвет в данной точке
	// Input: -
	// Output: sprite.getColor()
	sf::Color getColor()
	{
		return sprite.getColor();
	}

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};