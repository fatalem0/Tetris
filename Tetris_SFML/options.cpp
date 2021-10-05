#include "options.h"
#include <algorithm>

// функция, создающая шаблон экрана настроек
// Input: 
//		  music - музыка из файла
//	      sound - звуки
// Output: -
Options::Options(sf::Music *music, sf::Sound *sound)
{
	// объявим переменные для ползунков музыки и звуков
	bgm = music;
	sfx = sound;

	// загрузим фон из папки, вставим его в контейнер и установим размер
	bgTexture.loadFromFile("img/backgrounds/options.jpg");
	background.setTexture(&bgTexture);
	background.setSize(sf::Vector2f(1600.f, 900.f));

	rectangle.setFillColor(sf::Color::Black);
	rectangle.setSize(sf::Vector2f(880.0f, 500.0f));
	rectangle.setPosition(360.0f, 200.0f);

	// загрузим шрифт из папки
	ffont.loadFromFile("arcade.ttf");

	// создадим текст для ползунка изменения громкости музыки
	musicVolText = sf::Text("Music Volume:", ffont);
	musicVolText.setOutlineThickness(1.0f);
	musicVolText.setOutlineColor(sf::Color::Black);
	musicVolText.setPosition(400.f, 220.f);
	musicVolText.setFillColor(sf::Color::Cyan);

	// создадим ползунок изменения громкости музыки
	musicSlider.setFillColor(sf::Color::Green);
	musicSlider.setSize(sf::Vector2f(bgm->getVolume() * 8.f, 40.f));
	musicSlider.setPosition(400.f, 270.f);

	// создадим текст для ползунка изменения громкости эффектов
	sfxVolText = sf::Text("SFX Volume:", ffont);
	sfxVolText.setOutlineThickness(1.0f);
	sfxVolText.setOutlineColor(sf::Color::Black);
	sfxVolText.setPosition(400.f, 370.f);
	sfxVolText.setFillColor(sf::Color::White);

	// создадим ползунок изменения громкости эффектов
	sfxSlider.setFillColor(sf::Color::Green);
	sfxSlider.setSize(sf::Vector2f(sfx->getVolume() * 8.f, 40.f));
	sfxSlider.setPosition(400.f, 420.f);
	
	// создадим кнопку вернуться назад
	backText = sf::Text("BACK", ffont);
	backText.setOutlineThickness(2.0f);
	backText.setOutlineColor(sf::Color::Black);
	backText.setPosition(750.f, 650.f);
	backText.setFillColor(sf::Color::White);

	// установим нулевое значение положения курсора
	cursor = 0;
}

// функция, проверяющая нажатие кнопки вверх
// Input: -
// Output: -
void Options::upPressed()
{
	// если кнопка вверх нажата в нулевой позиции, то курсор никуда не сдвигается
	if (--cursor < 0)
		cursor = 0;
	// иначе текст, на котором находится курсор, и соответствующий ползунок подсвечиваются
	else
	{
		switch (cursor)
		{
			case 0:
				musicVolText.setFillColor(sf::Color::Cyan);
				sfxVolText.setFillColor(sf::Color::White);
				break;
			case 1:
				sfxVolText.setFillColor(sf::Color::Cyan);
				backText.setFillColor(sf::Color::White);
		}
	}
}

// функция, проверяющая нажатие кнопки вниз
// Input: -
// Output: -
short Options::downPressed()
{
	// если кнопка вниз нажата в последней позиции, то курсор никуда не сдвигается
	if (++cursor > 2)
		cursor = 2;

	// иначе текст, на котором находится курсор, и соответствующий ползунок подсвечиваются
	else
	{
		switch (cursor)
		{
			case 1:
				musicVolText.setFillColor(sf::Color::White);
				sfxVolText.setFillColor(sf::Color::Cyan);
				break;
			case 2:
				sfxVolText.setFillColor(sf::Color::White);
				backText.setFillColor(sf::Color::Cyan);
		}
	}
	return 0;
}

// функция, проверяющая нажатие стрелочки влево
// Input: -
// Output: -
void Options::leftPressed()
{
	// если курсор находится в нулевой позиции, то мы можем менять громкость у ползунка с музыкой
	if (!cursor)
	{
		// проверяем, больше ли нуля существующая громкость
		if (bgm->getVolume() > 0.f)
		{
			sf::Vector2f size = musicSlider.getSize(); // сохраняем данное значение положения ползунка
			bgm->setVolume(std::max(bgm->getVolume() - 5.0f, 0.0f)); // устанавливаем новое значение в пределах границ
			musicSlider.setSize(sf::Vector2f(bgm->getVolume() * 8, size.y)); // сохраняем новое значение
		}
	}
	// иначе курсор находится в позиции 1, и громкость можно менять только у ползунка с эффектами
	else if (cursor == 1)
	{	 
		// проверяем, больше ли нуля существующая громкость
		if (sfx->getVolume() > 0.f)
		{
			sf::Vector2f size = sfxSlider.getSize(); // сохраняем данное значение положения ползунка
			sfx->setVolume(std::max(sfx->getVolume() - 5.0f, 0.0f)); // устанавливаем новое значение в пределах границ
			sfxSlider.setSize(sf::Vector2f(sfx->getVolume() * 8, size.y)); // сохраняем новое значение
			sfx->play(); // проигрываем музыку
		}
	}
}

// функция, проверяющая нажатие стрелочки вправо
// Input: -
// Output: -
void Options::rightPressed()
{
	// если курсор находится в нулевой позиции, то мы можем менять громкость у ползунка с музыкой
	if (!cursor)
	{
		// проверяем, меньше ли ста существующая громкость
		if (bgm->getVolume() < 100.f)
		{
			sf::Vector2f size = musicSlider.getSize(); // сохраняем данное значение положения ползунка
			bgm->setVolume(std::min(bgm->getVolume() + 5.0f, 100.0f)); // устанавливаем новое значение в пределах границ
			musicSlider.setSize(sf::Vector2f(bgm->getVolume() * 8, size.y)); // сохраняем новое значение
		}
	}
	// иначе курсор находится в позиции 1, и громкость можно менять только у ползунка с эффектами
	else if (cursor == 1)
	{
		// проверяем, меньше ли ста существующая громкость
		if (sfx->getVolume() < 100.f)
		{
			sf::Vector2f size = sfxSlider.getSize(); // сохраняем данное значение положения ползунка
			sfx->setVolume(std::min(sfx->getVolume() + 5.0f, 100.0f)); // устанавливаем новое значение в пределах границ
			sfxSlider.setSize(sf::Vector2f(sfx->getVolume() * 8, size.y)); // сохраняем новое значение
			sfx->play(); // проигрываем музыку
		}
	}
}

// функция, возвращающая в главное меню при нажатии на кнопку, находяющуюся в положении 2
// Input: -
// Output: cursor == 2
short Options::enterPressed()
{
	return cursor == 2;
}

// функция, проверяющая нажатие кнопки Z, которая аналогична нажатия стрелочки влево
// Input: -
// Output: -
//void Options::zPressed()
// {
//	leftPressed();
//}

// функция, проверяющая нажатие кнопки X, которая аналогична нажатия стрелочки вправо
// Input: -
// Output: -
//void Options::xPressed()
//{
	//rightPressed();
//}


// функция для отрисовки окна опций
// Input: 
//		   target - переменная, которая хранит контейнеры для отрисовки
// 
//		   states - так как наши контейнеры не являются объектами высокого уровная, 
//		   то их нужно явно определить, поэтому в этой переменной хранятся 
//		   все настройки контейнеров
//  
// Output: - 
void Options::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(background);
	target.draw(rectangle);
	target.draw(musicVolText);
	target.draw(musicSlider);
	target.draw(sfxVolText);
	target.draw(sfxSlider);
	target.draw(backText);
}