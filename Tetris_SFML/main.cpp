#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "buttons.h"
#include "menu.h"
#include "loading.h"
#include "options.h"
#include "rating.h"
#include "game.h"


// ������� �����, � ������� ����� ����������� ������� ��������� ���������
enum class ProgramState
{
	MENU, LOAD, STAND_BY, GAME, GAME_OVER, PAUSE, QUIT_GAME, ENTER_NAME, OPTIONS, HIGHSCORE
};

int main()
{	
	// ����������� ���� ��� ����������
	sf::RenderWindow window(sf::VideoMode(1600, 900), "ANIME-TETRIS");

	// ����������� ��������� ��������� - ������� ����
	ProgramState state = ProgramState::MENU;

	// ����������� ��������� ����������
	Interface *currentInterface;

	// ����������� ��������� ��������� ����������
	Menu title;
	currentInterface = &title;

	// ��������� ���� - �� ��������
	Game *tetrisGameInstance = nullptr;

	// �������� ������ �� �����
	sf::Font ffont;
	ffont.loadFromFile("arcade.ttf");

	// �������� ������ �� �����
	sf::Music music;
	music.openFromFile(title.getMusic());
	music.setLoopPoints(sf::Music::TimeSpan(sf::seconds(52.646f), sf::seconds(49.180f)));
	music.play();
	music.setLoop(true);
	float musicVolume = 100.f;

	// ��������� ��������� ��������
	sf::Sound sfx;
	sfx.setVolume(100.f);

	// ��������� ������
	sf::Clock programClock;

	// ���������� �������� ������
	short startingLevel;

	// ������� ���������� ����� � ����
	unsigned int *results = nullptr;

	// ��������� ��������� ������ - �� ������������
	bool downButtonHeld = false;
	bool leftButtonHeld = false;
	bool rightButtonHeld = false;
	unsigned int inputRate = 40;

	Rating hiScoreScreen;
	std::string playerName;

	// ���� ���� �������, ��������� ����� ��������
	while (window.isOpen())
	{
		// ���� ������������ � ������� ����, �� ������� ���������� ����
		window.clear();

		// ������ ������� ����
		window.draw(*currentInterface);

		// �������� �� ������ ���� �������� ����
		if (state == ProgramState::LOAD)
		{
			if (music.getVolume() > 1)
				music.setVolume(music.getVolume() - 1.f);
			else
				music.stop();
			if (programClock.getElapsedTime() > sf::seconds(3.f))
			{
				if (currentInterface->fadeAway())
				{
					programClock.restart();
					state = ProgramState::STAND_BY;
					delete currentInterface;
					currentInterface = new Game(startingLevel);
					tetrisGameInstance = dynamic_cast<Game*>(currentInterface);
				}
			}
		}

		// �������� �� ������ ���� �������� �������
		else if (state == ProgramState::STAND_BY)
		{
			if (tetrisGameInstance != nullptr)
			{
				tetrisGameInstance->setBGMVolume(musicVolume);
				tetrisGameInstance->setSFXVolume(sfx.getVolume());
			}

			if (currentInterface->fadeIn())
			{
				tetrisGameInstance->spawnNextTetromino();
				state = ProgramState::GAME;
				programClock.restart();
			}
		}

		// �������� �� ������ ���� ����
		else if (state == ProgramState::GAME)
		{
			inputRate = 40 - (tetrisGameInstance->getMinoNum() / 38) * 4;

			if (programClock.getElapsedTime() >= sf::seconds(tetrisGameInstance->getCurrentSpeed() / 60.f))
			{
				downButtonHeld = false;
				if (tetrisGameInstance->downPressed() == -1)
				{
					state = ProgramState::GAME_OVER;
					results = tetrisGameInstance->gameOver();
				}
				programClock.restart();
			}
		}

		// �������� �� ����� �� ����
		else if (state == ProgramState::QUIT_GAME)
		{
			// �������� �� �������� � ����
			if (currentInterface->fadeAway())
			{
				delete tetrisGameInstance;
				tetrisGameInstance = nullptr;
				currentInterface = nullptr;

				// ����� ��������� ������ ������ ��� ������, ����� ���������������� ��� � ��������
				short rank = hiScoreScreen.determineRank(results[0]);
				if (rank < 6)
				{
					state = ProgramState::ENTER_NAME;
					std::cout << "\nCongratulations! You're top 5 high players! Enter your name below!" << std::endl;
					std::cout << "PLEASE: No spaces and at most 20 characters!" << std::endl;
					std::cin >> playerName;
					if (playerName.size() > 20)
						playerName = playerName.substr(0, 20);

					hiScoreScreen.updateRating(playerName, results[0], results[1], rank - 1);
				}

				// ����� ������������ � ����
				state = ProgramState::MENU;
				currentInterface = &title;
				music.setVolume(musicVolume);
				music.play();
				delete results;
				results = nullptr;
			}
		}

		// �������� �� ������� ������
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		{
			if (Buttons::activateInput(Buttons::upPressed(), inputRate, true))
			{
				currentInterface->upPressed();
			}
		}
		else
		{
			Buttons::upReleased();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		{
			unsigned int downButtonRate = state == ProgramState::GAME ? inputRate/2 : inputRate;

			if (!downButtonHeld && 
				Buttons::activateInput(Buttons::downPressed(), downButtonRate, state != ProgramState::GAME))
			{
				short status = currentInterface->downPressed();
				if (status == 1)
				{
					Buttons::downReleased();
					downButtonHeld = true;
					programClock.restart();
				}
				else if (status == -1 && state == ProgramState::GAME)
				{
					state = ProgramState::GAME_OVER;
					results = tetrisGameInstance->gameOver();
				}

				if (tetrisGameInstance && state == ProgramState::GAME)
					tetrisGameInstance->updateScore(1);
			}
		}
		else
		{
			Buttons::downReleased();
			downButtonHeld = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		{
			if (!rightButtonHeld && Buttons::activateInput(Buttons::leftPressed(), inputRate, true))
			{
				leftButtonHeld = true;
				Buttons::rightReleased();
				currentInterface->leftPressed();
			}
		}
		else
		{
			Buttons::leftReleased();
			leftButtonHeld = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		{
			if (!leftButtonHeld && Buttons::activateInput(Buttons::rightPressed(), inputRate, true))
			{
				rightButtonHeld = true;
				Buttons::leftReleased();
				currentInterface->rightPressed();
			}
		}
		else
		{
			Buttons::rightReleased();
			rightButtonHeld = false;
		}

		// ��������� ��� ��������, ������� ���� �������� � ��������� �����
		sf::Event event;
		while (window.pollEvent(event))
		{
			// �������� ������� ������
			if (event.type == sf::Event::KeyPressed)
			{
				// �������� ������ ��� ������� �������
				switch (event.key.code)
				{
					case sf::Keyboard::Escape:
						// ���� ������ Escape ������ � ����, �� ��������� ������ � ���� �����
						if (state == ProgramState::GAME)
						{
							state = ProgramState::PAUSE;
							currentInterface = new Pause();
						}

						// ���� ������ Escape ������ � ���� �����, �� ��������� ������ � ����
						else if (state == ProgramState::PAUSE)
						{
							state = ProgramState::GAME;
							delete currentInterface;
							currentInterface = tetrisGameInstance;
						}

						// ���� ������ Escape ������ � ���� ��������, �� ��������� ������ � ������� ����
						else if (state == ProgramState::HIGHSCORE)
						{
							state = ProgramState::MENU;
							currentInterface = &title;

							music.openFromFile(title.getMusic());
							music.setLoopPoints(sf::Music::TimeSpan(sf::seconds(52.646f), sf::seconds(49.180f)));
							music.play();
							music.setLoop(true);
						}
						break;
					case sf::Keyboard::Z:
						currentInterface->zPressed();
						break;
					case sf::Keyboard::X:
						currentInterface->xPressed();
						break;
					case sf::Keyboard::Enter:

						if (state == ProgramState::MENU)
						{
							// �������� ������ ��� ������� ������ Enter � ������� ����
							switch (currentInterface->enterPressed())
							{
								// ���� ������ ������ �������, �� ����������� ����� �������� ����
								case 0:
									state = ProgramState::LOAD;
									startingLevel = title.getLevelCursor();
									currentInterface = new Loading();
									musicVolume = music.getVolume();
									programClock.restart();
									break;

								// ���� ������ ������ �������, �� ����������� ���� ��������
								case 1:
									state = ProgramState::OPTIONS;
									currentInterface = new Options(&music, &sfx);
									break;

								// ���� ������ ������ �������, �� ����������� ���� ��������
								case 2:
									state = ProgramState::HIGHSCORE;
									currentInterface = &hiScoreScreen;
									music.stop();
									music.openFromFile("music/pause.ogg");
									music.setLoop(true);
									music.play();
									break;
							}
						}

						// ���� ������� ��������� ���� - ��������, �� ���������� ����� �� ���� 
						else if (state == ProgramState::GAME_OVER)
						{
							state = ProgramState::QUIT_GAME;
						}

						// ���� ������� ��������� ���� - ���� �����, � ������ ������ Enter, �� ������
						// ��������� � ������� ����
						else if (state == ProgramState::OPTIONS && currentInterface->enterPressed())
						{
							delete currentInterface;
							currentInterface = &title;
							state = ProgramState::MENU;
						}
				}
			}

			// ���� ��� ������� �������, �� ���� �����������
			else if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		// ������������ �� ������� ����
		window.display();
	}

	return 0;
}