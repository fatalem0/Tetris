#include "rating.h"

// функция для создания шаблона окна рейтинга
Rating::Rating()
{	
	// установим размер и фон окна рейтинга 
	backgroundRect.setSize(sf::Vector2f(1600.f, 900.f));
	bgImage.loadFromFile("img/backgrounds/highscore.jpg");
	backgroundRect.setTexture(&bgImage);

	// установим размер, цвет и расположение контейнера с результатами
	scoresRect.setSize(sf::Vector2f(1000.f, 600.f));
	scoresRect.setFillColor(sf::Color(0, 0, 0, 200));
	scoresRect.setPosition(300.f, 150.f);

	// объявим массивы векторов, куда будут записываться игроки и где будут храниться имена, количество очков и 
	// количество успешно уничтоженных линий тетрамино. По умолчанию имя игроков пусто, и количество очков и
	// и линий нулевое
	names = std::vector<std::string>(5, "");
	scores = std::vector<unsigned int>(5, 0);
	lines = std::vector<unsigned int>(5, 0);
	
	// открываем файл с рейтингом
	readFs.open("highscore.hs");
	for (int i = 0; readFs; ++i)
	{
		readFs >> names[i];
		readFs >> scores[i];
		readFs >> lines[i];
	}
	readFs.close();

	// загружаем шрифт
	ffont.loadFromFile("arcade.ttf");

	// объявляем переменную с текстом над результатами и распополагаем текст над ними
	ratingHeader = sf::Text("HIGH SCORES\t SCORE   LINES", ffont, 40);
	ratingHeader.setPosition(320.f, 170.f);
	ratingHeader.setFillColor(sf::Color::Yellow);
	ratingHeader.setOutlineColor(sf::Color::Black);

	// в этом цикле мы будем проходиться по файлу с результатами и записывать значения в массив векторов
	for (int i = 0; i < 5; ++i)
	{
		// добавляем в массивы векторов считанные из файла с результатами значения
		rankNumberTexts.push_back(sf::Text(std::to_string(i + 1) + ". ", ffont));
		nameTexts.push_back(sf::Text(names[i], ffont));
		scoreTexts.push_back(sf::Text(std::to_string(scores[i]), ffont));
		linesTexts.push_back(sf::Text(std::to_string(lines[i]), ffont));

		// устанавливаем цвета для новых записей
		rankNumberTexts[i].setFillColor(sf::Color::White);
		nameTexts[i].setFillColor(sf::Color::White);
		scoreTexts[i].setFillColor(sf::Color::White);
		linesTexts[i].setFillColor(sf::Color::White);

		// распологаем в окне новые записи
		rankNumberTexts[i].setPosition(320.f, 240.f + 100.f * i);
		nameTexts[i].setPosition(360.f, 240.f + 100.f * i);
		scoreTexts[i].setPosition(800.f, 240.f + 100.f * i);
		linesTexts[i].setPosition(1060.f, 240.f + 100.f * i);
	}
}

// функция для определения позиции в рейтинге
// Input: 
//		   score - счёт игрока
// Output: int
short Rating::determineRank(unsigned int score)
{	 
	if (!score)
	{
		return 6;
	}
	short i = 0, j = 4;
	while (i <= j)
	{
		int m = (i + j) / 2;
		if (scores[m] > score)
		{
			i = m + 1;
		}
		else if (score > scores[m])
		{
			j = m - 1;
		}
		else
		{
			return m + 1;
		}
	}
	return i + 1;
}

// функция для обновления результата в рейтинге
// Input:
//		   name - имя игрока
//		   score - счёт игрока
//		   lineNum - количество уничтоженных линий
//		   rank - позиция игрока в рейтинге
// Output:
//         -
void Rating::updateRating(std::string &name, unsigned int score, unsigned int lineNum, short rank)
{	
	// вставляем в вектор новое значение имени и удаляем последний элемент из вектора
	names.insert(names.begin() + rank, name);
	nameTexts.insert(nameTexts.begin() + rank, sf::Text(names[rank], ffont));
	names.pop_back();
	nameTexts.pop_back();

	// вставляем в вектор новое значение очков и удаляем последний элемент из вектора
	scores.insert(scores.begin() + rank, score);
	scoreTexts.insert(scoreTexts.begin() + rank, sf::Text(std::to_string(scores[rank]), ffont));
	scores.pop_back();
	scoreTexts.pop_back();

	// вставляем в вектор новое значение уничтоженных линий и удаляем последний элемент из вектора
	lines.insert(lines.begin() + rank, lineNum);
	linesTexts.insert(linesTexts.begin() + rank, sf::Text(std::to_string(lines[rank]), ffont));
	lines.pop_back();
	linesTexts.pop_back();

	// располагаем и закрашиваем элементы
	nameTexts[rank].setPosition(320.f, 240.f + 100.f * rank);
	nameTexts[rank].setFillColor(sf::Color::White);
	scoreTexts[rank].setPosition(800.f, 240.f + 100.f * rank);
	scoreTexts[rank].setFillColor(sf::Color::White);
	linesTexts[rank].setPosition(1060.f, 240.f + 100.f * rank);
	linesTexts[rank].setFillColor(sf::Color::White);

	// сдвигаем других игроков в рейтинге относительно нового 
	for (int i = rank; i < 5; ++i)
	{
		nameTexts[i].setPosition(360.f, 240.f + 100.f * i);
		nameTexts[i].setFillColor(sf::Color::White);
		scoreTexts[i].setPosition(800.f, 240.f + 100.f * i);
		scoreTexts[i].setFillColor(sf::Color::White);
		linesTexts[i].setPosition(1060.f, 240.f + 100.f * i);
		linesTexts[i].setFillColor(sf::Color::White);
	}

	// записываем результат в файл
	writeFs.open("highscore.hs");
	for (int i = 0; i < 5; ++i)
	{
		writeFs << names[i] << ' ';
		writeFs << scores[i] << ' ';
		writeFs << this->lines[i] << ' ';
		writeFs << std::endl;
	}
	writeFs.close();
}

// функция для отрисовки окна с рейтингом.
// Input: 
//		   target - переменная, которая хранит контейнеры для отрисовки
// 
//		   states - так как наши контейнеры не являются объектами высокого уровная, 
//		   то их нужно явно определить, поэтому в этой переменной хранятся 
//		   все настройки контейнеров
//  
// Output: - 
void Rating::draw(sf::RenderTarget& target, sf::RenderStates states) const
{	
	// отрисовываем контейнеры
	target.draw(backgroundRect);
	target.draw(scoresRect);
	target.draw(ratingHeader);

	// отрисовываем значения в таблице рейтинга
	for (int i = 0; i < 5; ++i)
	{
		target.draw(rankNumberTexts[i]);
		target.draw(nameTexts[i]);
		target.draw(scoreTexts[i]);
		target.draw(linesTexts[i]);
	}
}