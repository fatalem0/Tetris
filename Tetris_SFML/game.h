#pragma once
#include "tetromino.h"
#include "block.h"
#include "Interface.h"
#include "pause.h"
#include <SFML/Audio.hpp>
#include <random>

// определим игровую сетку
#define MATRIX(row, col) ((row+2)*10 + (col))

// класс, инициализирующий методы и свойства окна игры
class Game : public Interface
{
	sf::RectangleShape backgroundRect;
	sf::Texture backgroundPicture;

	sf::RectangleShape matrixPicture;
	std::vector<sf::Sprite> frame;

	sf::Font ffont;
	sf::RectangleShape nextBlockRect;
	sf::Text nextText;

	Tetromino currentBlock;
	Tetromino nextBlock;

	std::vector<Block*> matrix;

	const unsigned short frameRate[21] = {
		48, 43, 38, 33, 28, 23, 18, 13, 8, 6,
		5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2
	};
	const char tetrominoTypes[7] = { 'L', 'J', 'T', 'O', 'Z', 'S', 'I' };
	std::random_device randomNumberGenerator;

	sf::SoundBuffer buffer1, buffer3, buffer5;

	sf::Music bgm;
	sf::Sound moveSound, lineClearSound, levelUpSound;

	sf::RectangleShape scoreRect;
	sf::Text scoreText, levelText, linesText;

	short level;
	unsigned int score;
	unsigned int lines;

	short delayBetweenLockAndNext;

	short minoNum;

	sf::Text hiddenMessage;
	sf::Text gameOverMessage;

public:
	Game(short level);
	~Game();

	bool spawnNextTetromino();
	short checkLines(short row);
	void levelUp();
	unsigned int* gameOver();
	void setBGMVolume(float volume);
	void setSFXVolume(float volume);
	void updateScore(unsigned int delta);

	short downPressed() override;
	void leftPressed() override;
	void rightPressed() override;
	void zPressed() override;
	void xPressed() override;

	bool fadeAway() override;
	bool fadeIn() override;

	unsigned short getCurrentSpeed()
	{
		return frameRate[level];
	}
	short getMinoNum()
	{
		return minoNum;
	}

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};