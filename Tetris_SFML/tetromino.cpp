#include "tetromino.h"
#include <iostream>


// функция, располагающая целый тетрамино
// Input: -
// Output: -
Tetromino::Tetromino()
{
	location = Point(0, 4);
}

// функция, которая создаёт целый тетрамино в зависимости от типа
// Input: 
//		  type - тип целого тетрамино
// Output: - 
Tetromino::Tetromino(char type)
{
	location = Point(0, 4); // располагаем тетрамино
	rotation = 0; // устанавливаем угол поворота ноль
	this->type = type; // здесь мы будем менять тип тетрамино

	// запускаем оператор выбора
	switch (type)
	{
		case 'L':
			minoes.push_back(new Block(0, 0, sf::Color::Magenta));
			minoes.push_back(new Block(0, -1, sf::Color::Magenta));
			minoes.push_back(new Block(0, 1, sf::Color::Magenta));
			minoes.push_back(new Block(1, -1, sf::Color::Magenta));
			break;
		case 'J':
			minoes.push_back(new Block(0, 0, sf::Color::Yellow));
			minoes.push_back(new Block(0, -1, sf::Color::Yellow));
			minoes.push_back(new Block(0, 1, sf::Color::Yellow));
			minoes.push_back(new Block(1, 1, sf::Color::Yellow));
			break;
		case 'T':
			minoes.push_back(new Block(0, 0, sf::Color::Green));
			minoes.push_back(new Block(0, -1, sf::Color::Green));
			minoes.push_back(new Block(0, 1, sf::Color::Green));
			minoes.push_back(new Block(1, 0, sf::Color::Green));
			break;
		case 'O':
			minoes.push_back(new Block(0, 0, sf::Color(0, 100, 255)));
			minoes.push_back(new Block(0, 1, sf::Color(0, 100, 255)));
			minoes.push_back(new Block(1, 0, sf::Color(0, 100, 255)));
			minoes.push_back(new Block(1, 1, sf::Color(0, 100, 255)));
			break;
		case 'Z':
			minoes.push_back(new Block(0, 0, sf::Color(255, 128, 0)));
			minoes.push_back(new Block(0, -1, sf::Color(255, 128, 0)));
			minoes.push_back(new Block(1, 0, sf::Color(255, 128, 0)));
			minoes.push_back(new Block(1, 1, sf::Color(255, 128, 0)));
			break;
		case 'S':
			minoes.push_back(new Block(0, 0, sf::Color::Cyan));
			minoes.push_back(new Block(0, 1, sf::Color::Cyan));
			minoes.push_back(new Block(1, 0, sf::Color::Cyan));
			minoes.push_back(new Block(1, -1, sf::Color::Cyan));
			break;
		case 'I':
			minoes.push_back(new Block(0, 0, sf::Color::Red));
			minoes.push_back(new Block(0, -1, sf::Color::Red));
			minoes.push_back(new Block(0, 1, sf::Color::Red));
			minoes.push_back(new Block(0, 2, sf::Color::Red));
	}
}

// функция, которая сдвигает целый тетрамино влево при нажатии стрелочки влево
// Input: 
//		   x - количество нажатий стрелочки влево
// Output: -
void Tetromino::moveLeft(short x)
{
	// сдвигаем целый тетрамино влево по количество нажатий на стрелочку влево
	location = location - Point(0, x);

	// в цикле меняем расположение каждого блока тетрамино
	for (Block *mino : minoes)
	{
		mino->setPositionAt(mino->getPoint() + location);
	}
}

// функция, которая сдвигает целый тетрамино влево при нажатии стрелочки вправо
// Input: 
//		   x - количество нажатий стрелочки вправо
// Output: -
void Tetromino::moveRight(short x)
{
	// сдвигаем целый тетрамино вправо по количество нажатий на стрелочку вправо
	location = location + Point(0, x);

	// в цикле меняем расположение каждого блока тетрамино
	for (Block *mino : minoes)
	{
		mino->setPositionAt(mino->getPoint() + location);
	}
}

// функция, которая сдвигает целый тетрамино по количеству нажатий на стрелочку вниз
// Input: 
//		   y - количество нажатий на стрелочку вниз
// Output: -
void Tetromino::moveDown(short y)
{
	// сдвигаем целый тетрамино по количеству нажатий на стрелочку вниз
	location = location + Point(y, 0);

	// в цикле меняем расположение каждого блока тетрамино
	for (Block *mino : minoes)
	{
		mino->setPositionAt(mino->getPoint() + location);
	}
}

// функция, которая вращает целый тетрамино влево
// Input: -
// Output: -
void Tetromino::rotateLeft()
{	
	// запускаем оператор выбора на основании типа
	switch (type)
	{
		case 'L':
		case 'J':
		case 'T':
			// в цикле меняем расположение каждого блока тетрамино
			for (int i = 1; i < 4; ++i)
			{
				// в переменные записываем номера строки и столбца, где расположен каждый блок тетрамино
				short r = minoes[i]->getPoint().row, c = minoes[i]->getPoint().col;
				minoes[i]->setPoint(-c, r); // определяем расположение одного блока тетрамино
				minoes[i]->setPositionAt(minoes[i]->getPoint() + location); // поворачиваем каждый блок тетрамино
			}
			break;
		case 'Z':
		case 'S':
		case 'I':
			// в цикле меняем расположение каждого блока тетрамино
			for (int i = 1; i < 4; ++i)
			{
				// в переменные записываем номера строки и столбца, где расположен каждый блок тетрамино
				short r = minoes[i]->getPoint().row, c = minoes[i]->getPoint().col;
				Point transform = rotation ? Point(-c, r) : Point(c, -r); // сохраняем изменённый тетрамино
				minoes[i]->setPoint(transform.row, transform.col); // определяем расположение одного блока тетрамино
				minoes[i]->setPositionAt(transform + location); // поворачиваем каждый блок тетрамино
			}
			rotation = !rotation; // устанавливаем поворот в исходное положение
	}
}

// функция, которая вращает целый тетрамино вправо
// Input: -
// Output: -
void Tetromino::rotateRight()
{
	// запускаем оператор выбора на основании типа
	switch (type)
	{
		case 'L':
		case 'J':
		case 'T':
			// в цикле меняем расположение каждого блока тетрамино
			for (int i = 1; i < 4; ++i)
			{
				// в переменные записываем номера строки и столбца, где расположен каждый блок тетрамино
				short r = minoes[i]->getPoint().row, c = minoes[i]->getPoint().col;
				minoes[i]->setPoint(c, -r); // определяем расположение одного блока тетрамино
				minoes[i]->setPositionAt(minoes[i]->getPoint() + location); // поворачиваем каждый блок тетрамино
			}
			break;
		case 'Z':
		case 'S':
		case 'I':
			// в цикле меняем расположение каждого блока тетрамино
			for (int i = 1; i < 4; ++i)
			{
				// в переменные записываем номера строки и столбца, где расположен каждый блок тетрамино
				short r = minoes[i]->getPoint().row, c = minoes[i]->getPoint().col; 
				Point transform = rotation ? Point(-c, r) : Point(c, -r); // сохраняем изменённый тетрамино
				minoes[i]->setPoint(transform.row, transform.col); // определяем расположение одного блока тетрамино
				minoes[i]->setPositionAt(transform + location); // поворачиваем каждый блок тетрамино
			}
			rotation = !rotation; // устанавливаем поворот в исходное положение
	}
}

// функция для отрисовки целого блока тетрамино
// Input: 
//		   target - переменная, которая хранит контейнеры для отрисовки
// 
//		   states - так как наши контейнеры не являются объектами высокого уровная, 
//		   то их нужно явно определить, поэтому в этой переменной хранятся 
//		   все настройки контейнеров
//  
// Output: - 
void Tetromino::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// в цикле отрисовываем каждый блок тетрамино
	for (int i = 0; i < minoes.size(); ++i)
	{
		target.draw(*minoes[i]);
	}
}