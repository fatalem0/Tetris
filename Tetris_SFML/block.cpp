#include "block.h"

// функци€, котора€ создаЄт шаблон одного блока тетрамино
// Input: 
//		   r - номер строчки, где расположен один блок тетрамино
//		   c - номер колонки, где расположен один блок тетрамино
//		   color - цвет одного блока тетрамино
// Output: -
Block::Block(short r, short c, sf::Color color)
{
	// загружаем текстуру, создаЄм контейнер дл€ неЄ, загружаЄм ее внутрь контейнера и распологаем на экране
	texture.loadFromFile("img/mino.png");
	sprite.setTextureRect(sf::IntRect(0, 0, 36, 36));
	sprite.setTexture(texture);
	sprite.setPosition(620.f + c * 36.f, 90.f + r * 36.f);

	// устанавливаем цвет блока
	sprite.setColor(color);

	// устанавливаем положение одного блока тетрамино
	point = Point(r, c);
}

// деструктор, который делает один блок тетрамино прозрачным
Block::~Block()
{
	sprite.setColor(sf::Color::Transparent);
}

// функци€, котора€ устанавливает нужный цвет одного блока тетрамино
// Input: - 
// Output: -
void Block::setColor(sf::Color color)
{
	sprite.setColor(color);
}

// функци€, котора€ устанавливает расположение одного блока тетрамино
// Input: 
//		   r - номер строчки
//		   c - номер столбца
// Output: -
void Block::setPoint(short r, short c)
{
	point.row = r;
	point.col = c;
}

// функци€, котора€ мен€ет расположение одного блока тетрамино
// Input: p - номер строчки, куда должен переместитьс€ блок
// Output: -
void Block::setPositionAt(Point p)
{
	sprite.setPosition(620.f + p.col * 36.f, 90.f + p.row * 36.f);
}

// функци€ дл€ отрисовки одного блока тетрамино
// Input: 
//		   target - переменна€, котора€ хранит контейнеры дл€ отрисовки
// 
//		   states - так как наши контейнеры не €вл€ютс€ объектами высокого уровна€, 
//		   то их нужно €вно определить, поэтому в этой переменной хран€тс€ 
//		   все настройки контейнеров
//  
// Output: - 
void Block::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite);
}