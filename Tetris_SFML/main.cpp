#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "buttons.h"
#include "menu.h"
#include "loading.h"
#include "options.h"
#include "rating.h"
#include "game.h"


// объявим класс, в котором будем фиксировать текущее положение программы
enum class ProgramState
{
	MENU, LOAD, STAND_BY, GAME, GAME_OVER, PAUSE, QUIT_GAME, ENTER_NAME, OPTIONS, HIGHSCORE
};

int main()
{	
	// сгенерируем окно для приложения
	sf::RenderWindow window(sf::VideoMode(1600, 900), "ANIME-TETRIS");

	// изначальное положение программы - главное меню
	ProgramState state = ProgramState::MENU;

	// импортируем положение интерфейса
	Interface *currentInterface;

	// ипмортируем начальное положение интерфейса
	Menu title;
	currentInterface = &title;

	// состояние игры - не запущена
	Game *tetrisGameInstance = nullptr;

	// загрузим шрифты из папки
	sf::Font ffont;
	ffont.loadFromFile("arcade.ttf");

	// запустим музыку из папки
	sf::Music music;
	music.openFromFile(title.getMusic());
	music.setLoopPoints(sf::Music::TimeSpan(sf::seconds(52.646f), sf::seconds(49.180f)));
	music.play();
	music.setLoop(true);
	float musicVolume = 100.f;

	// установим громкость эффектов
	sf::Sound sfx;
	sfx.setVolume(100.f);

	// установим таймер
	sf::Clock programClock;

	// переменная нулевого уровня
	short startingLevel;

	// объявим количество очков в ноль
	unsigned int *results = nullptr;

	// начальное положение кнопок - не удерживаются
	bool downButtonHeld = false;
	bool leftButtonHeld = false;
	bool rightButtonHeld = false;
	unsigned int inputRate = 40;

	Rating hiScoreScreen;
	std::string playerName;

	// пока окно открыто, программа будет работать
	while (window.isOpen())
	{
		// если возвращаемся в главное меню, то очищаем предыдущие окна
		window.clear();

		// рисуем текущее окно
		window.draw(*currentInterface);

		// проверка на запуск окна загрузки игры
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

		// проверка на запуск окна ожидания запуска
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

		// проверка на запуск окна игры
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

		// проверка на выход из игры
		else if (state == ProgramState::QUIT_GAME)
		{
			// проверка на проигрыш в игре
			if (currentInterface->fadeAway())
			{
				delete tetrisGameInstance;
				tetrisGameInstance = nullptr;
				currentInterface = nullptr;

				// после проигрыша просим ввести имя игрока, чтобы зарегистрировать его в рейтинге
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

				// после возвращаемся в меню
				state = ProgramState::MENU;
				currentInterface = &title;
				music.setVolume(musicVolume);
				music.play();
				delete results;
				results = nullptr;
			}
		}

		// проверка на нажатие кнопок
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

		// проверяем все действия, которые были запущены в последнее время
		sf::Event event;
		while (window.pollEvent(event))
		{
			// действие нажатия кнопки
			if (event.type == sf::Event::KeyPressed)
			{
				// оператор выбора для нажатой клавиши
				switch (event.key.code)
				{
					case sf::Keyboard::Escape:
						// если кнопка Escape нажата в игре, то переводим игрока в окно паузы
						if (state == ProgramState::GAME)
						{
							state = ProgramState::PAUSE;
							currentInterface = new Pause();
						}

						// если кнопка Escape нажата в окне паузы, то переводим игрока в игру
						else if (state == ProgramState::PAUSE)
						{
							state = ProgramState::GAME;
							delete currentInterface;
							currentInterface = tetrisGameInstance;
						}

						// если кнопка Escape нажата в окне рейтинга, то переводим игрока в главное меню
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
							// оператор выбора при нажатии кнопки Enter в главном меню
							switch (currentInterface->enterPressed())
							{
								// если нажата первая надпись, то открывается экран загрузки игры
								case 0:
									state = ProgramState::LOAD;
									startingLevel = title.getLevelCursor();
									currentInterface = new Loading();
									musicVolume = music.getVolume();
									programClock.restart();
									break;

								// если нажата вторая надпись, то открывается окно настроек
								case 1:
									state = ProgramState::OPTIONS;
									currentInterface = new Options(&music, &sfx);
									break;

								// если нажата третья надпись, то открывается окно рейтинга
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

						// если текущее положение окна - проигрыш, то происходит выход из игры 
						else if (state == ProgramState::GAME_OVER)
						{
							state = ProgramState::QUIT_GAME;
						}

						// если текущее положение окна - окно опций, и нажата кнопка Enter, то игрока
						// переводит в главное меню
						else if (state == ProgramState::OPTIONS && currentInterface->enterPressed())
						{
							delete currentInterface;
							currentInterface = &title;
							state = ProgramState::MENU;
						}
				}
			}

			// если все события закрыты, то окно закрывается
			else if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		// возвращаемся на рабочий стол
		window.display();
	}

	return 0;
}