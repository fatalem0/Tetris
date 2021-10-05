#include "game.h"
#include <algorithm>
#include <iostream>


// функция, которая создаёт шаблон окна игры
// Input: level - номер уровня
// Output: -
Game::Game(short level)
{
	// объявим переменную, в которой хранится массив векторов 
	matrix = std::vector<Block*>(220, nullptr);

	// создадим сетку матрицы
	matrixPicture.setSize(sf::Vector2f(432.f, 792.f));
	matrixPicture.setFillColor(sf::Color(0, 0, 0, 0));
	matrixPicture.setPosition(584.f, 54.f);
	
	// загрузим шрифт из папки
	ffont.loadFromFile("arcade.ttf");

	// создадим контейнер для показа следующего блока
	nextBlockRect.setSize(sf::Vector2f(200.f, 200.f));
	nextBlockRect.setPosition(1200.f, 54.f);
	nextBlockRect.setFillColor(sf::Color(0, 0, 0, 0));
	
	// создадим текст, который говорит о следующем блоке
	nextText = sf::Text("NEXT", ffont, 20);
	nextText.setPosition(1220.f, 60.f);
	nextText.setFillColor(sf::Color(255, 255, 255, 0));

	// создадим генератор тетрамино
	nextBlock = Tetromino(tetrominoTypes[randomNumberGenerator() % 7]);
	nextBlock.moveRight(14);
	
	// объявим текстуру одного блока тетрамино и загрузим его из папки
	sf::Texture frameMinoTexture;
	frameMinoTexture.loadFromFile("img/mino.png");

	for (int i = 0; i < 64; ++i)
	{
		// положим текстуру одного блока тетрамино в массив векторов
		frame.push_back(sf::Sprite(frameMinoTexture));
		// создадим текстуру на сетке
		frame[i].setTextureRect(sf::IntRect(0, 0, 36, 36));
		frame[i].setColor(sf::Color(255, 255, 255, 0));
	}
	// в следующих двух циклах мы создаем положение сетки
	for (int i = 0; i < 12; ++i)
	{
		frame[i].setPosition(584.f + 36.f*i, 54.f);
		frame[i + 12].setPosition(584.f + 36.f*i, 810.f);
	}
	for (int i = 0; i < 20; ++i)
	{
		frame[i + 24].setPosition(584.f, 90.f + 36.f*i);
		frame[i + 44].setPosition(980.f, 90.f + 36.f*i);
	}

	// устанавливаем текущее значение уровня
	this->level = level;

	// объявим номер текущего уровня
	std::string levelNumber;

	// если он больше или равен 15, то оставляем его равным 15
	if (level >= 15)
		levelNumber = "15";
	else
	{
		// если он нацело делится на 10, то ставим вперёд цифру
		if (level / 10)
			levelNumber.push_back('1');
		// и после в конец добавляем остаток от деления на 10
		levelNumber.push_back('0' + level % 10);
	}

	// загружаем из папки фон для уровня
	backgroundPicture.loadFromFile("img/backgrounds/level" + levelNumber + ".jpg");

	// создаём контейнер для фона, загружаем его
	backgroundRect.setSize(sf::Vector2f(1600.f, 900.f));
	backgroundRect.setTexture(&backgroundPicture);
	sf::Color color = backgroundRect.getFillColor();
	backgroundRect.setFillColor(sf::Color(color.r, color.g, color.b, 0));

	// на основании текущего уровня изменяем музыку
	if (level <= 1)
		bgm.openFromFile("music/lvl1.ogg");
	else if (level <= 4)
		bgm.openFromFile("music/lvl2.ogg");
	else if (level <= 8)
		bgm.openFromFile("music/lvl3.ogg");
	else if (level <= 11)
		bgm.openFromFile("music/lvl4.ogg");
	else if (level <= 14)
		bgm.openFromFile("music/lvl5.ogg");
	else
		bgm.openFromFile("music/lvl6.ogg");

	// запускаем её и ставим на повтор
	bgm.play();
	bgm.setLoop(true);

	// изначально обнуляем количество очков и уничтоженных линий
	score = 0;
	lines = 0;

	// создаём контейнер для количества очков и текста
	scoreRect.setSize(sf::Vector2f(300.f, 300.f));
	scoreRect.setPosition(150.f, 150.f);
	scoreRect.setFillColor(sf::Color(0, 0, 0, 200));
	scoreText = sf::Text("SCORE: \n0", ffont, 35);
	scoreText.setPosition(175.f, 175.f);

	// создаём текст и устанавливаем его
	levelText = sf::Text("LEVEL: \n" + std::to_string(level), ffont, 35);
	levelText.setPosition(175.f, 265.f);

	// создаём текст и устанавливаем его
	linesText = sf::Text("LINES: \n0", ffont, 35);
	linesText.setPosition(175.f, 355.f);

	// изначально объявляем задержку между достижением самого нижнего уровня,
	// где лежит собранная линия, и выпадением следующего тетрамино и количество блоков тетрамино нулями
	delayBetweenLockAndNext = 0;
	minoNum = 0;

	// объявим сообщение, которое будет появляться при поражении
	hiddenMessage = sf::Text("Go to the terminal for instructions", ffont);
	hiddenMessage.setPosition(200.f, 450.f);
	hiddenMessage.setFillColor(sf::Color(255, 255, 0, 0));
}

// деструктор для линий
Game::~Game()
{
	for (int i = 0; i < 220; ++i)
	{
		if (matrix[i])
		{
			delete matrix[i];
			matrix[i] = nullptr;
		}
	}
}

// функция, проверяющая нажатие стрелочки влево
// Input: -
// Output: -
void Game::leftPressed()
{
	// цикл для движения тетрамино влево и изменения сетки
	for (Block* mino : currentBlock.getMinoes())
	{
		// устанавливаем положение каждого блока тетрамино и меняем положение тетрамино
		Point newPt = mino->getPoint() + currentBlock.getLocation() - Point(0, 1);

		// выполняем, пока можно двигаться влево или пока тетрамино не достигло дна
		if (newPt.col < 0 || matrix[MATRIX(newPt.row, newPt.col)] != nullptr)
		{
			return;
		}
	}

	// сдвигаем тетрамино
	currentBlock.moveLeft(1);
	buffer1.loadFromFile("sfx/movement.ogg");
	moveSound.setBuffer(buffer1);
	moveSound.play();
}

// функция, проверяющая нажатие стрелочки вправо
// Input: -
// Output: -
void Game::rightPressed()
{
	// цикл для движения тетрамино влево и изменения сетки
	for (Block* mino : currentBlock.getMinoes())
	{
		// устанавливаем положение каждого блока тетрамино и меняем положение тетрамино
		Point newPt = mino->getPoint() + currentBlock.getLocation() + Point(0, 1);

		// выполняем, пока можно двигаться вправо или пока тетрамино не достигло дна
		if (newPt.col > 9 || matrix[MATRIX(newPt.row, newPt.col)] != nullptr)
		{
			return;
		}
	}

	// сдвигаем тетрамино вправо
	currentBlock.moveRight(1);
	buffer1.loadFromFile("sfx/movement.ogg");
	moveSound.setBuffer(buffer1);
	moveSound.play();
}

// функция, проверяющая нажатие кнопки Z
// Input: -
// Output: -
void Game::zPressed()
{
	// поворачиваем тетрамино влево
	currentBlock.rotateLeft();

	// цикл для поворота каждого блока тетрамино
	for (Block* mino : currentBlock.getMinoes())
	{
		// устанавливаем положение каждого блока тетрамино
		Point newPt = mino->getPoint() + currentBlock.getLocation();

		// проверяем, можно ли повернуть тетрамино в другое направление
		if (newPt.col < 0 || newPt.col > 9 || newPt.row < -2 || newPt.row > 19 ||
			matrix[MATRIX(newPt.row, newPt.col)] != nullptr)
		{
			// если можно, то поворачиваем вправо
			currentBlock.rotateRight();
			buffer1.loadFromFile("sfx/movement.ogg");
			moveSound.setBuffer(buffer1);
			moveSound.play();
			return;
		}
	}
}

// функция, проверяющая нажатие кнопки X
// Input: -
// Output: -
void Game::xPressed()
{
	// поворачиваем тетрамино вправо
	currentBlock.rotateRight();

	// цикл для поворота каждого блока тетрамино
	for (Block* mino : currentBlock.getMinoes())
	{
		// устанавливаем положение каждого блока тетрамино
		Point newPt = mino->getPoint() + currentBlock.getLocation();

		// проверяем, можно ли повернуть тетрамино в другое направление
		if (newPt.col < 0 || newPt.col > 9 || newPt.row < -2 || newPt.row > 19 ||
			matrix[MATRIX(newPt.row, newPt.col)] != nullptr)
		{
			// если можно, то поворачиваем влево
			currentBlock.rotateLeft();
			buffer1.loadFromFile("sfx/movement.ogg");
			moveSound.setBuffer(buffer1);
			moveSound.play();
			return;
		}
	}
}

// функция, проверяющая, нужно ли ставить новый тетрамино
// Input: -
// Output: bool
bool Game::spawnNextTetromino()
{
	// 
	currentBlock = nextBlock;
	currentBlock.moveLeft(14);

	// запускаем цикл, в котором будем осуществлять проверку касания низа сетки
	for (Block *mino : currentBlock.getMinoes())
	{
		// сохраняем значение каждого блока тетрамино
		Point p = mino->getPoint() + currentBlock.getLocation();

		// если ни один из них не достиг дна, то продолжаем игру
		if (matrix[MATRIX(p.row, p.col)] != nullptr)
		{
			return false;
		}
	}

	// иначе создаём новый тетрамино
	nextBlock = Tetromino(tetrominoTypes[randomNumberGenerator() % 7]);
	nextBlock.moveRight(14);

	return true;
}

// функция, которая возвращает количество уничтоженных линий
// Input: row - номер колонки
// Output: lines = количество уничтоженных линий
short Game::checkLines(short row)
{
	// изначально обнуляем количество уничтоженных линий
	short lines = 0;

	// создадим массив векторов, в котором будут храниться линии
	std::vector<short> lineRows;

	// цикл, проверяющий, что линия находится в самой нижней позиции
	for (short r = row; r > row - 4 && r >= -2; --r)
	{
		int i = 0;
		while (i < 10)
		{
			if (matrix[MATRIX(r, i)] == nullptr)
				break;
			++i;
		}
		if (i == 10)
		{
			lineRows.push_back(r);
			++lines;
		}
	}

	// если количество уничтоженных линий больше нуля, то обновляем количество очков
	if (lines)
	{
		switch (lines)
		{
			case 1:
				buffer3.loadFromFile("sfx/line_clear.ogg");
				lineClearSound.setBuffer(buffer3);
				lineClearSound.play();
				updateScore(40 * (level + 1));
				break;
			case 2:
				buffer3.loadFromFile("sfx/line_clear.ogg");
				lineClearSound.setBuffer(buffer3);
				lineClearSound.play();
				updateScore(100 * (level + 1));
				break;
			case 3:
				buffer3.loadFromFile("sfx/line_clear.ogg");
				lineClearSound.setBuffer(buffer3);
				lineClearSound.play();
				updateScore(300 * (level + 1));
				break;
			case 4:
				buffer3.loadFromFile("sfx/line_clear.ogg");
				lineClearSound.setBuffer(buffer3);
				lineClearSound.play();
				updateScore(1200 * (level + 1));
		}

		// пока сложенная линия не пуста
		while (!lineRows.empty())
		{
			// изменяем массив векторов для линий
			short row = lineRows.back();
			lineRows.pop_back();

			// удаляем нижнюю часть матрицы сетки
			for (int i = 4; i >= 0; --i)
			{
				delete matrix[MATRIX(row, i)];
				matrix[MATRIX(row, i)] = nullptr;
				delete matrix[MATRIX(row, 9 - i)];
				matrix[MATRIX(row, 9 - i)] = nullptr;
			}

			// и сдвигаем её до стандартного положения вместе со всеми существующими на экране тетрамино
			for (int j = row - 1; j >= -1; --j)
			{
				for (int k = 0; k < 5; ++k)
				{
					matrix[MATRIX(j + 1, k)] = matrix[MATRIX(j, k)];
					matrix[MATRIX(j + 1, 9 - k)] = matrix[MATRIX(j, 9 - k)];

					if (matrix[MATRIX(j, k)] != nullptr)
					{
						matrix[MATRIX(j, k)]->setPoint(j + 1, k);
						matrix[MATRIX(j, k)]->setPositionAt(Point(j + 1, k));
					}
					if (matrix[MATRIX(j, 9 - k)] != nullptr)
					{
						matrix[MATRIX(j, 9 - k)]->setPoint(j + 1, 9 - k);
						matrix[MATRIX(j, 9 - k)]->setPositionAt(Point(j + 1, 9 - k));
					}
				}
			}
		}
	}

	// уменьшаем количество блоков тетрамино
	minoNum -= 10 * lines;

	return lines;
}


// функция, увеличивающая уровень
// Input: -
// Output: -
void Game::levelUp()
{
	// проигрываем звук повышения уровня
	buffer5.loadFromFile("sfx/level_up.ogg");
	levelUpSound.setBuffer(buffer5);
	levelUpSound.play();

	// проверяем, не стал ли уровень больше максимального
	if (++level <= 15)
	{
		// изменяем число уровня на экране
		std::string levelNumber;
		if (level / 10)
			levelNumber.push_back('1');
		levelNumber.push_back('0' + level % 10);

		// загружаем новый фон
		backgroundPicture.loadFromFile("img/backgrounds/level" + levelNumber + ".jpg");

		// в операторе выбора сменяем музыку
		switch (level)
		{
			case 3:
				bgm.stop();
				bgm.openFromFile("music/lvl2.ogg");
				bgm.play();
				break;
			case 6:
				bgm.stop();
				bgm.openFromFile("music/lvl3.ogg");
				bgm.play();
				break;
			case 9:
				bgm.stop();
				bgm.openFromFile("music/lvl4.ogg");
				bgm.play();
				break;
			case 12:
				bgm.stop();
				bgm.openFromFile("music/lvl5.ogg");
				bgm.play();
				break;
			case 15:
				bgm.stop();
				bgm.openFromFile("music/lvl6.ogg");
				bgm.play();
		}
	}

	// выводим название нового уровня
	levelText.setString("LEVEL: \n" + std::to_string(level));
}

// функция, возвращающая количество набранных очков и уничтоженных линий после проигрыша
// Input: -
// Output: scoreAndLines
unsigned int* Game::gameOver()
{
	// меняем музыку
	bgm.stop();
	bgm.openFromFile("music/gameover.ogg");
	bgm.play();


	// выводим сообщение о проигрыше
	gameOverMessage = sf::Text("Game over!\nPlease, push ENTER", ffont);
	gameOverMessage.setCharacterSize(72);
	gameOverMessage.setPosition(400.f, 250.f);
	gameOverMessage.setFillColor(sf::Color(255, 255, 0, 255));

	// затемняем экран
	matrixPicture.setFillColor(sf::Color(0, 0, 0, 255));
	nextBlockRect.setFillColor(sf::Color(0, 0, 0, 255));
	nextText.setFillColor(sf::Color(0, 0, 0, 255));
	scoreRect.setFillColor(sf::Color(0, 0, 0, 255));
	scoreText.setFillColor(sf::Color(0, 0, 0, 255));
	levelText.setFillColor(sf::Color(0, 0, 0, 255));
	linesText.setFillColor(sf::Color(0, 0, 0, 255));
	backgroundRect.setFillColor(sf::Color(0, 0, 0, 255));

	// меняем цвет каждого блока тетрамино
	for (Block *mino : matrix)
	{
		if (mino != nullptr)
		{
			mino->setColor(sf::Color(128, 128, 128, 255));
		}
	}

	// создаём новую переменную для хранения количества набранных очков и уничтоженных линий
	unsigned int *scoreAndLines = new unsigned int[2] { score, lines };
	return scoreAndLines;
}

// функция, устанавливающая громкость музыки
// Input: -
// Output: -
void Game::setBGMVolume(float volume)
{
	bgm.setVolume(volume * .8f);
}

// функция, устанавливающая громкость эффектов
// Input: -
// Output: -
void Game::setSFXVolume(float volume)
{
	moveSound.setVolume(volume);
	lineClearSound.setVolume(volume);
	levelUpSound.setVolume(volume);
}

// функция, обновляющая количество набранных очков
// Input: delta - набранные очки
// Output: -
void Game::updateScore(unsigned int delta)
{
	// пока количество набранных очков меньше максимального целого числа, добавляем очки
	if (score + delta < UINT32_MAX)
		score += delta;

	// в противном случае количество очков равно максимальному целому числу
	else
		score = UINT32_MAX;

	// выводим количество набранных очков
	scoreText.setString("SCORE: \n" + std::to_string(score));
}

// функция проверяющая нажатие стрелочки вниз
// Input: -
// Output: 1, если кнопка нажата, и 0, если нет
short Game::downPressed()
{
	// lock будет говорить нам о нажатии кнопки
	bool lock = false;

	// цикл, который будет сдвигать тетрамино и изменять сетку 
	for (Block *mino : currentBlock.getMinoes())
	{
		Point newPt = mino->getPoint() + currentBlock.getLocation() + Point(1, 0);
		if (newPt.row >= 20 || matrix[MATRIX(newPt.row, newPt.col)] != nullptr)
		{
			lock = true;
			break;
		}
	}

	// проверяем, что линия собралась
	if (lock)
	{
		// возвращаем ноль, если линия собралась без нажатия
		if (++delayBetweenLockAndNext < 1 + (frameRate[level] < 20) + (frameRate[level] < 5))
			return 0;

		// обнуляем задержку между сборкой линии и выпадением следующего тетрамино
		delayBetweenLockAndNext = 0;

		// изменяем сетку
		short row = currentBlock.getLocation().row;
		for (Block *mino : currentBlock.getMinoes())
		{
			row = std::max(row, static_cast<short>(mino->getPoint().row + currentBlock.getLocation().row));
			Point finalPt = mino->getPoint() + currentBlock.getLocation();
			matrix[MATRIX(finalPt.row, finalPt.col)] = mino;
		}

		// увеличиваем количество уничтоженных линий
		lines += checkLines(row);
		linesText.setString("LINES: \n" + std::to_string(lines));
		
		// изменяем уровень
		if (lines / 10 > level)
		{
			levelUp();
		}

		// увеличиваем количество блоков тетрамино
		minoNum += 4;

		// если не создалось следующее тетрамино, то возвращаем -1
		if (!spawnNextTetromino())
			return -1;

		return 1;
	}
	else
	{
		// сдвигаем тетрамино, если линия не собралась
		delayBetweenLockAndNext = 0;
		currentBlock.moveDown(1);
	}
	return 0;
}

// функция, которая изменяет составляющие экрана при нажатии ENTER после проигрыша
// Input: -
// Output: -
bool Game::fadeAway()
{
	// выводим текст в консоль
	sf::Color bgColor = backgroundRect.getFillColor();
	backgroundRect.setFillColor(sf::Color(bgColor.r, bgColor.g, bgColor.b, bgColor.a - 5));

	// прячем сообщение об проигрыше
	gameOverMessage.setFillColor(sf::Color(255, 255, 0, 0));
	// выводим сообщение о дальнейших действиях
	hiddenMessage.setFillColor(sf::Color(255, 255, 0, 255));

	// изменяем громкость музыки или останавливаем её
	//if (bgm.getVolume() > 5)
		//bgm.setVolume(bgm.getVolume() - 5.f);
	//else
		//bgm.stop();

	// делаем фон прозрачным
	return backgroundRect.getFillColor().a == 0;
}

// функция, выводящая на экран игру снова
// Input: -
// Output: -
bool Game::fadeIn()
{
	// делаем заливку фона
	sf::Color bgColor = backgroundRect.getFillColor();
	backgroundRect.setFillColor(sf::Color(bgColor.r, bgColor.g, bgColor.b, bgColor.a + 5));

	// делаем заливку сетки
	sf::Color matrixColor = matrixPicture.getFillColor();
	matrixPicture.setFillColor(sf::Color(matrixColor.r, matrixColor.g, matrixColor.b, matrixColor.a + 4));

	// делаем заливку следующего блока
	sf::Color nextBlockColor = nextBlockRect.getFillColor();
	nextBlockRect.setFillColor(sf::Color(nextBlockColor.r, nextBlockColor.g, nextBlockColor.b, nextBlockColor.a + 3));

	// делаем заливку текста, поясняющего следующий блок
	sf::Color nextTextColor = nextText.getFillColor();
	nextText.setFillColor(sf::Color(nextTextColor.r, nextTextColor.g, nextTextColor.b, nextTextColor.a + 5));

	// окрашиваем все тетрамино в прежние цвета
	for (sf::Sprite &frameMino : frame)
	{
		sf::Color minoColor = frameMino.getColor();
		frameMino.setColor(sf::Color(minoColor.r, minoColor.g, minoColor.b, minoColor.a + 5));
	}
	
	// делаем фон видимым
	return backgroundRect.getFillColor().a == 255;
}


// функция для отрисовки окна игры
// Input: 
//		   target - переменная, которая хранит контейнеры для отрисовки
// 
//		   states - так как наши контейнеры не являются объектами высокого уровная, 
//		   то их нужно явно определить, поэтому в этой переменной хранятся 
//		   все настройки контейнеров
//  
// Output: - 
void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(backgroundRect);
	target.draw(matrixPicture);
	target.draw(nextBlockRect);
	target.draw(nextText);
	target.draw(scoreRect);
	target.draw(scoreText);
	target.draw(levelText);
	target.draw(linesText);
	for (sf::Sprite frameMino : frame)
	{
		target.draw(frameMino);
	}
	target.draw(currentBlock);
	target.draw(nextBlock);
	for (Block *mino : matrix)
	{
		if (mino)
			target.draw(*mino);
	}
	target.draw(gameOverMessage);
	target.draw(hiddenMessage);
}
