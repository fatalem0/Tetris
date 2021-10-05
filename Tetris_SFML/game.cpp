#include "game.h"
#include <algorithm>
#include <iostream>


// �������, ������� ������ ������ ���� ����
// Input: level - ����� ������
// Output: -
Game::Game(short level)
{
	// ������� ����������, � ������� �������� ������ �������� 
	matrix = std::vector<Block*>(220, nullptr);

	// �������� ����� �������
	matrixPicture.setSize(sf::Vector2f(432.f, 792.f));
	matrixPicture.setFillColor(sf::Color(0, 0, 0, 0));
	matrixPicture.setPosition(584.f, 54.f);
	
	// �������� ����� �� �����
	ffont.loadFromFile("arcade.ttf");

	// �������� ��������� ��� ������ ���������� �����
	nextBlockRect.setSize(sf::Vector2f(200.f, 200.f));
	nextBlockRect.setPosition(1200.f, 54.f);
	nextBlockRect.setFillColor(sf::Color(0, 0, 0, 0));
	
	// �������� �����, ������� ������� � ��������� �����
	nextText = sf::Text("NEXT", ffont, 20);
	nextText.setPosition(1220.f, 60.f);
	nextText.setFillColor(sf::Color(255, 255, 255, 0));

	// �������� ��������� ���������
	nextBlock = Tetromino(tetrominoTypes[randomNumberGenerator() % 7]);
	nextBlock.moveRight(14);
	
	// ������� �������� ������ ����� ��������� � �������� ��� �� �����
	sf::Texture frameMinoTexture;
	frameMinoTexture.loadFromFile("img/mino.png");

	for (int i = 0; i < 64; ++i)
	{
		// ������� �������� ������ ����� ��������� � ������ ��������
		frame.push_back(sf::Sprite(frameMinoTexture));
		// �������� �������� �� �����
		frame[i].setTextureRect(sf::IntRect(0, 0, 36, 36));
		frame[i].setColor(sf::Color(255, 255, 255, 0));
	}
	// � ��������� ���� ������ �� ������� ��������� �����
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

	// ������������� ������� �������� ������
	this->level = level;

	// ������� ����� �������� ������
	std::string levelNumber;

	// ���� �� ������ ��� ����� 15, �� ��������� ��� ������ 15
	if (level >= 15)
		levelNumber = "15";
	else
	{
		// ���� �� ������ ������� �� 10, �� ������ ����� �����
		if (level / 10)
			levelNumber.push_back('1');
		// � ����� � ����� ��������� ������� �� ������� �� 10
		levelNumber.push_back('0' + level % 10);
	}

	// ��������� �� ����� ��� ��� ������
	backgroundPicture.loadFromFile("img/backgrounds/level" + levelNumber + ".jpg");

	// ������ ��������� ��� ����, ��������� ���
	backgroundRect.setSize(sf::Vector2f(1600.f, 900.f));
	backgroundRect.setTexture(&backgroundPicture);
	sf::Color color = backgroundRect.getFillColor();
	backgroundRect.setFillColor(sf::Color(color.r, color.g, color.b, 0));

	// �� ��������� �������� ������ �������� ������
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

	// ��������� � � ������ �� ������
	bgm.play();
	bgm.setLoop(true);

	// ���������� �������� ���������� ����� � ������������ �����
	score = 0;
	lines = 0;

	// ������ ��������� ��� ���������� ����� � ������
	scoreRect.setSize(sf::Vector2f(300.f, 300.f));
	scoreRect.setPosition(150.f, 150.f);
	scoreRect.setFillColor(sf::Color(0, 0, 0, 200));
	scoreText = sf::Text("SCORE: \n0", ffont, 35);
	scoreText.setPosition(175.f, 175.f);

	// ������ ����� � ������������� ���
	levelText = sf::Text("LEVEL: \n" + std::to_string(level), ffont, 35);
	levelText.setPosition(175.f, 265.f);

	// ������ ����� � ������������� ���
	linesText = sf::Text("LINES: \n0", ffont, 35);
	linesText.setPosition(175.f, 355.f);

	// ���������� ��������� �������� ����� ����������� ������ ������� ������,
	// ��� ����� ��������� �����, � ���������� ���������� ��������� � ���������� ������ ��������� ������
	delayBetweenLockAndNext = 0;
	minoNum = 0;

	// ������� ���������, ������� ����� ���������� ��� ���������
	hiddenMessage = sf::Text("Go to the terminal for instructions", ffont);
	hiddenMessage.setPosition(200.f, 450.f);
	hiddenMessage.setFillColor(sf::Color(255, 255, 0, 0));
}

// ���������� ��� �����
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

// �������, ����������� ������� ��������� �����
// Input: -
// Output: -
void Game::leftPressed()
{
	// ���� ��� �������� ��������� ����� � ��������� �����
	for (Block* mino : currentBlock.getMinoes())
	{
		// ������������� ��������� ������� ����� ��������� � ������ ��������� ���������
		Point newPt = mino->getPoint() + currentBlock.getLocation() - Point(0, 1);

		// ���������, ���� ����� ��������� ����� ��� ���� ��������� �� �������� ���
		if (newPt.col < 0 || matrix[MATRIX(newPt.row, newPt.col)] != nullptr)
		{
			return;
		}
	}

	// �������� ���������
	currentBlock.moveLeft(1);
	buffer1.loadFromFile("sfx/movement.ogg");
	moveSound.setBuffer(buffer1);
	moveSound.play();
}

// �������, ����������� ������� ��������� ������
// Input: -
// Output: -
void Game::rightPressed()
{
	// ���� ��� �������� ��������� ����� � ��������� �����
	for (Block* mino : currentBlock.getMinoes())
	{
		// ������������� ��������� ������� ����� ��������� � ������ ��������� ���������
		Point newPt = mino->getPoint() + currentBlock.getLocation() + Point(0, 1);

		// ���������, ���� ����� ��������� ������ ��� ���� ��������� �� �������� ���
		if (newPt.col > 9 || matrix[MATRIX(newPt.row, newPt.col)] != nullptr)
		{
			return;
		}
	}

	// �������� ��������� ������
	currentBlock.moveRight(1);
	buffer1.loadFromFile("sfx/movement.ogg");
	moveSound.setBuffer(buffer1);
	moveSound.play();
}

// �������, ����������� ������� ������ Z
// Input: -
// Output: -
void Game::zPressed()
{
	// ������������ ��������� �����
	currentBlock.rotateLeft();

	// ���� ��� �������� ������� ����� ���������
	for (Block* mino : currentBlock.getMinoes())
	{
		// ������������� ��������� ������� ����� ���������
		Point newPt = mino->getPoint() + currentBlock.getLocation();

		// ���������, ����� �� ��������� ��������� � ������ �����������
		if (newPt.col < 0 || newPt.col > 9 || newPt.row < -2 || newPt.row > 19 ||
			matrix[MATRIX(newPt.row, newPt.col)] != nullptr)
		{
			// ���� �����, �� ������������ ������
			currentBlock.rotateRight();
			buffer1.loadFromFile("sfx/movement.ogg");
			moveSound.setBuffer(buffer1);
			moveSound.play();
			return;
		}
	}
}

// �������, ����������� ������� ������ X
// Input: -
// Output: -
void Game::xPressed()
{
	// ������������ ��������� ������
	currentBlock.rotateRight();

	// ���� ��� �������� ������� ����� ���������
	for (Block* mino : currentBlock.getMinoes())
	{
		// ������������� ��������� ������� ����� ���������
		Point newPt = mino->getPoint() + currentBlock.getLocation();

		// ���������, ����� �� ��������� ��������� � ������ �����������
		if (newPt.col < 0 || newPt.col > 9 || newPt.row < -2 || newPt.row > 19 ||
			matrix[MATRIX(newPt.row, newPt.col)] != nullptr)
		{
			// ���� �����, �� ������������ �����
			currentBlock.rotateLeft();
			buffer1.loadFromFile("sfx/movement.ogg");
			moveSound.setBuffer(buffer1);
			moveSound.play();
			return;
		}
	}
}

// �������, �����������, ����� �� ������� ����� ���������
// Input: -
// Output: bool
bool Game::spawnNextTetromino()
{
	// 
	currentBlock = nextBlock;
	currentBlock.moveLeft(14);

	// ��������� ����, � ������� ����� ������������ �������� ������� ���� �����
	for (Block *mino : currentBlock.getMinoes())
	{
		// ��������� �������� ������� ����� ���������
		Point p = mino->getPoint() + currentBlock.getLocation();

		// ���� �� ���� �� ��� �� ������ ���, �� ���������� ����
		if (matrix[MATRIX(p.row, p.col)] != nullptr)
		{
			return false;
		}
	}

	// ����� ������ ����� ���������
	nextBlock = Tetromino(tetrominoTypes[randomNumberGenerator() % 7]);
	nextBlock.moveRight(14);

	return true;
}

// �������, ������� ���������� ���������� ������������ �����
// Input: row - ����� �������
// Output: lines = ���������� ������������ �����
short Game::checkLines(short row)
{
	// ���������� �������� ���������� ������������ �����
	short lines = 0;

	// �������� ������ ��������, � ������� ����� ��������� �����
	std::vector<short> lineRows;

	// ����, �����������, ��� ����� ��������� � ����� ������ �������
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

	// ���� ���������� ������������ ����� ������ ����, �� ��������� ���������� �����
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

		// ���� ��������� ����� �� �����
		while (!lineRows.empty())
		{
			// �������� ������ �������� ��� �����
			short row = lineRows.back();
			lineRows.pop_back();

			// ������� ������ ����� ������� �����
			for (int i = 4; i >= 0; --i)
			{
				delete matrix[MATRIX(row, i)];
				matrix[MATRIX(row, i)] = nullptr;
				delete matrix[MATRIX(row, 9 - i)];
				matrix[MATRIX(row, 9 - i)] = nullptr;
			}

			// � �������� � �� ������������ ��������� ������ �� ����� ������������� �� ������ ���������
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

	// ��������� ���������� ������ ���������
	minoNum -= 10 * lines;

	return lines;
}


// �������, ������������� �������
// Input: -
// Output: -
void Game::levelUp()
{
	// ����������� ���� ��������� ������
	buffer5.loadFromFile("sfx/level_up.ogg");
	levelUpSound.setBuffer(buffer5);
	levelUpSound.play();

	// ���������, �� ���� �� ������� ������ �������������
	if (++level <= 15)
	{
		// �������� ����� ������ �� ������
		std::string levelNumber;
		if (level / 10)
			levelNumber.push_back('1');
		levelNumber.push_back('0' + level % 10);

		// ��������� ����� ���
		backgroundPicture.loadFromFile("img/backgrounds/level" + levelNumber + ".jpg");

		// � ��������� ������ ������� ������
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

	// ������� �������� ������ ������
	levelText.setString("LEVEL: \n" + std::to_string(level));
}

// �������, ������������ ���������� ��������� ����� � ������������ ����� ����� ���������
// Input: -
// Output: scoreAndLines
unsigned int* Game::gameOver()
{
	// ������ ������
	bgm.stop();
	bgm.openFromFile("music/gameover.ogg");
	bgm.play();


	// ������� ��������� � ���������
	gameOverMessage = sf::Text("Game over!\nPlease, push ENTER", ffont);
	gameOverMessage.setCharacterSize(72);
	gameOverMessage.setPosition(400.f, 250.f);
	gameOverMessage.setFillColor(sf::Color(255, 255, 0, 255));

	// ��������� �����
	matrixPicture.setFillColor(sf::Color(0, 0, 0, 255));
	nextBlockRect.setFillColor(sf::Color(0, 0, 0, 255));
	nextText.setFillColor(sf::Color(0, 0, 0, 255));
	scoreRect.setFillColor(sf::Color(0, 0, 0, 255));
	scoreText.setFillColor(sf::Color(0, 0, 0, 255));
	levelText.setFillColor(sf::Color(0, 0, 0, 255));
	linesText.setFillColor(sf::Color(0, 0, 0, 255));
	backgroundRect.setFillColor(sf::Color(0, 0, 0, 255));

	// ������ ���� ������� ����� ���������
	for (Block *mino : matrix)
	{
		if (mino != nullptr)
		{
			mino->setColor(sf::Color(128, 128, 128, 255));
		}
	}

	// ������ ����� ���������� ��� �������� ���������� ��������� ����� � ������������ �����
	unsigned int *scoreAndLines = new unsigned int[2] { score, lines };
	return scoreAndLines;
}

// �������, ��������������� ��������� ������
// Input: -
// Output: -
void Game::setBGMVolume(float volume)
{
	bgm.setVolume(volume * .8f);
}

// �������, ��������������� ��������� ��������
// Input: -
// Output: -
void Game::setSFXVolume(float volume)
{
	moveSound.setVolume(volume);
	lineClearSound.setVolume(volume);
	levelUpSound.setVolume(volume);
}

// �������, ����������� ���������� ��������� �����
// Input: delta - ��������� ����
// Output: -
void Game::updateScore(unsigned int delta)
{
	// ���� ���������� ��������� ����� ������ ������������� ������ �����, ��������� ����
	if (score + delta < UINT32_MAX)
		score += delta;

	// � ��������� ������ ���������� ����� ����� ������������� ������ �����
	else
		score = UINT32_MAX;

	// ������� ���������� ��������� �����
	scoreText.setString("SCORE: \n" + std::to_string(score));
}

// ������� ����������� ������� ��������� ����
// Input: -
// Output: 1, ���� ������ ������, � 0, ���� ���
short Game::downPressed()
{
	// lock ����� �������� ��� � ������� ������
	bool lock = false;

	// ����, ������� ����� �������� ��������� � �������� ����� 
	for (Block *mino : currentBlock.getMinoes())
	{
		Point newPt = mino->getPoint() + currentBlock.getLocation() + Point(1, 0);
		if (newPt.row >= 20 || matrix[MATRIX(newPt.row, newPt.col)] != nullptr)
		{
			lock = true;
			break;
		}
	}

	// ���������, ��� ����� ���������
	if (lock)
	{
		// ���������� ����, ���� ����� ��������� ��� �������
		if (++delayBetweenLockAndNext < 1 + (frameRate[level] < 20) + (frameRate[level] < 5))
			return 0;

		// �������� �������� ����� ������� ����� � ���������� ���������� ���������
		delayBetweenLockAndNext = 0;

		// �������� �����
		short row = currentBlock.getLocation().row;
		for (Block *mino : currentBlock.getMinoes())
		{
			row = std::max(row, static_cast<short>(mino->getPoint().row + currentBlock.getLocation().row));
			Point finalPt = mino->getPoint() + currentBlock.getLocation();
			matrix[MATRIX(finalPt.row, finalPt.col)] = mino;
		}

		// ����������� ���������� ������������ �����
		lines += checkLines(row);
		linesText.setString("LINES: \n" + std::to_string(lines));
		
		// �������� �������
		if (lines / 10 > level)
		{
			levelUp();
		}

		// ����������� ���������� ������ ���������
		minoNum += 4;

		// ���� �� ��������� ��������� ���������, �� ���������� -1
		if (!spawnNextTetromino())
			return -1;

		return 1;
	}
	else
	{
		// �������� ���������, ���� ����� �� ���������
		delayBetweenLockAndNext = 0;
		currentBlock.moveDown(1);
	}
	return 0;
}

// �������, ������� �������� ������������ ������ ��� ������� ENTER ����� ���������
// Input: -
// Output: -
bool Game::fadeAway()
{
	// ������� ����� � �������
	sf::Color bgColor = backgroundRect.getFillColor();
	backgroundRect.setFillColor(sf::Color(bgColor.r, bgColor.g, bgColor.b, bgColor.a - 5));

	// ������ ��������� �� ���������
	gameOverMessage.setFillColor(sf::Color(255, 255, 0, 0));
	// ������� ��������� � ���������� ���������
	hiddenMessage.setFillColor(sf::Color(255, 255, 0, 255));

	// �������� ��������� ������ ��� ������������� �
	//if (bgm.getVolume() > 5)
		//bgm.setVolume(bgm.getVolume() - 5.f);
	//else
		//bgm.stop();

	// ������ ��� ����������
	return backgroundRect.getFillColor().a == 0;
}

// �������, ��������� �� ����� ���� �����
// Input: -
// Output: -
bool Game::fadeIn()
{
	// ������ ������� ����
	sf::Color bgColor = backgroundRect.getFillColor();
	backgroundRect.setFillColor(sf::Color(bgColor.r, bgColor.g, bgColor.b, bgColor.a + 5));

	// ������ ������� �����
	sf::Color matrixColor = matrixPicture.getFillColor();
	matrixPicture.setFillColor(sf::Color(matrixColor.r, matrixColor.g, matrixColor.b, matrixColor.a + 4));

	// ������ ������� ���������� �����
	sf::Color nextBlockColor = nextBlockRect.getFillColor();
	nextBlockRect.setFillColor(sf::Color(nextBlockColor.r, nextBlockColor.g, nextBlockColor.b, nextBlockColor.a + 3));

	// ������ ������� ������, ����������� ��������� ����
	sf::Color nextTextColor = nextText.getFillColor();
	nextText.setFillColor(sf::Color(nextTextColor.r, nextTextColor.g, nextTextColor.b, nextTextColor.a + 5));

	// ���������� ��� ��������� � ������� �����
	for (sf::Sprite &frameMino : frame)
	{
		sf::Color minoColor = frameMino.getColor();
		frameMino.setColor(sf::Color(minoColor.r, minoColor.g, minoColor.b, minoColor.a + 5));
	}
	
	// ������ ��� �������
	return backgroundRect.getFillColor().a == 255;
}


// ������� ��� ��������� ���� ����
// Input: 
//		   target - ����������, ������� ������ ���������� ��� ���������
// 
//		   states - ��� ��� ���� ���������� �� �������� ��������� �������� �������, 
//		   �� �� ����� ���� ����������, ������� � ���� ���������� �������� 
//		   ��� ��������� �����������
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
