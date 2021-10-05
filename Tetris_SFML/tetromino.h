#pragma once
#include <vector>
#include "block.h"

// �����, ���������������� ������ � �������� ������ ���������
class Tetromino : public sf::Drawable
{
	std::vector<Block*> minoes;
	char type;
	bool rotation;
	Point location;

public:
	Tetromino();
	Tetromino(char type);

	// �������, ���������� � ������� ��������, � ������� ��������� ������ ���� ���������
	// Input: -
	// Output: minoes
	std::vector<Block*>& getMinoes()
	{
		return minoes;
	}

	// ������� ���������� ������������ ������ ���������
	// Input: -
	// Output: location
	Point getLocation()
	{
		return location;
	}

	void moveLeft(short x);
	void moveRight(short x);
	void moveDown(short y);
	void rotateLeft();
	void rotateRight();

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};