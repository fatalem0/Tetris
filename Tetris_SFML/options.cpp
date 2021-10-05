#include "options.h"
#include <algorithm>

// �������, ��������� ������ ������ ��������
// Input: 
//		  music - ������ �� �����
//	      sound - �����
// Output: -
Options::Options(sf::Music *music, sf::Sound *sound)
{
	// ������� ���������� ��� ��������� ������ � ������
	bgm = music;
	sfx = sound;

	// �������� ��� �� �����, ������� ��� � ��������� � ��������� ������
	bgTexture.loadFromFile("img/backgrounds/options.jpg");
	background.setTexture(&bgTexture);
	background.setSize(sf::Vector2f(1600.f, 900.f));

	rectangle.setFillColor(sf::Color::Black);
	rectangle.setSize(sf::Vector2f(880.0f, 500.0f));
	rectangle.setPosition(360.0f, 200.0f);

	// �������� ����� �� �����
	ffont.loadFromFile("arcade.ttf");

	// �������� ����� ��� �������� ��������� ��������� ������
	musicVolText = sf::Text("Music Volume:", ffont);
	musicVolText.setOutlineThickness(1.0f);
	musicVolText.setOutlineColor(sf::Color::Black);
	musicVolText.setPosition(400.f, 220.f);
	musicVolText.setFillColor(sf::Color::Cyan);

	// �������� �������� ��������� ��������� ������
	musicSlider.setFillColor(sf::Color::Green);
	musicSlider.setSize(sf::Vector2f(bgm->getVolume() * 8.f, 40.f));
	musicSlider.setPosition(400.f, 270.f);

	// �������� ����� ��� �������� ��������� ��������� ��������
	sfxVolText = sf::Text("SFX Volume:", ffont);
	sfxVolText.setOutlineThickness(1.0f);
	sfxVolText.setOutlineColor(sf::Color::Black);
	sfxVolText.setPosition(400.f, 370.f);
	sfxVolText.setFillColor(sf::Color::White);

	// �������� �������� ��������� ��������� ��������
	sfxSlider.setFillColor(sf::Color::Green);
	sfxSlider.setSize(sf::Vector2f(sfx->getVolume() * 8.f, 40.f));
	sfxSlider.setPosition(400.f, 420.f);
	
	// �������� ������ ��������� �����
	backText = sf::Text("BACK", ffont);
	backText.setOutlineThickness(2.0f);
	backText.setOutlineColor(sf::Color::Black);
	backText.setPosition(750.f, 650.f);
	backText.setFillColor(sf::Color::White);

	// ��������� ������� �������� ��������� �������
	cursor = 0;
}

// �������, ����������� ������� ������ �����
// Input: -
// Output: -
void Options::upPressed()
{
	// ���� ������ ����� ������ � ������� �������, �� ������ ������ �� ����������
	if (--cursor < 0)
		cursor = 0;
	// ����� �����, �� ������� ��������� ������, � ��������������� �������� ��������������
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

// �������, ����������� ������� ������ ����
// Input: -
// Output: -
short Options::downPressed()
{
	// ���� ������ ���� ������ � ��������� �������, �� ������ ������ �� ����������
	if (++cursor > 2)
		cursor = 2;

	// ����� �����, �� ������� ��������� ������, � ��������������� �������� ��������������
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

// �������, ����������� ������� ��������� �����
// Input: -
// Output: -
void Options::leftPressed()
{
	// ���� ������ ��������� � ������� �������, �� �� ����� ������ ��������� � �������� � �������
	if (!cursor)
	{
		// ���������, ������ �� ���� ������������ ���������
		if (bgm->getVolume() > 0.f)
		{
			sf::Vector2f size = musicSlider.getSize(); // ��������� ������ �������� ��������� ��������
			bgm->setVolume(std::max(bgm->getVolume() - 5.0f, 0.0f)); // ������������� ����� �������� � �������� ������
			musicSlider.setSize(sf::Vector2f(bgm->getVolume() * 8, size.y)); // ��������� ����� ��������
		}
	}
	// ����� ������ ��������� � ������� 1, � ��������� ����� ������ ������ � �������� � ���������
	else if (cursor == 1)
	{	 
		// ���������, ������ �� ���� ������������ ���������
		if (sfx->getVolume() > 0.f)
		{
			sf::Vector2f size = sfxSlider.getSize(); // ��������� ������ �������� ��������� ��������
			sfx->setVolume(std::max(sfx->getVolume() - 5.0f, 0.0f)); // ������������� ����� �������� � �������� ������
			sfxSlider.setSize(sf::Vector2f(sfx->getVolume() * 8, size.y)); // ��������� ����� ��������
			sfx->play(); // ����������� ������
		}
	}
}

// �������, ����������� ������� ��������� ������
// Input: -
// Output: -
void Options::rightPressed()
{
	// ���� ������ ��������� � ������� �������, �� �� ����� ������ ��������� � �������� � �������
	if (!cursor)
	{
		// ���������, ������ �� ��� ������������ ���������
		if (bgm->getVolume() < 100.f)
		{
			sf::Vector2f size = musicSlider.getSize(); // ��������� ������ �������� ��������� ��������
			bgm->setVolume(std::min(bgm->getVolume() + 5.0f, 100.0f)); // ������������� ����� �������� � �������� ������
			musicSlider.setSize(sf::Vector2f(bgm->getVolume() * 8, size.y)); // ��������� ����� ��������
		}
	}
	// ����� ������ ��������� � ������� 1, � ��������� ����� ������ ������ � �������� � ���������
	else if (cursor == 1)
	{
		// ���������, ������ �� ��� ������������ ���������
		if (sfx->getVolume() < 100.f)
		{
			sf::Vector2f size = sfxSlider.getSize(); // ��������� ������ �������� ��������� ��������
			sfx->setVolume(std::min(sfx->getVolume() + 5.0f, 100.0f)); // ������������� ����� �������� � �������� ������
			sfxSlider.setSize(sf::Vector2f(sfx->getVolume() * 8, size.y)); // ��������� ����� ��������
			sfx->play(); // ����������� ������
		}
	}
}

// �������, ������������ � ������� ���� ��� ������� �� ������, ������������ � ��������� 2
// Input: -
// Output: cursor == 2
short Options::enterPressed()
{
	return cursor == 2;
}

// �������, ����������� ������� ������ Z, ������� ���������� ������� ��������� �����
// Input: -
// Output: -
//void Options::zPressed()
// {
//	leftPressed();
//}

// �������, ����������� ������� ������ X, ������� ���������� ������� ��������� ������
// Input: -
// Output: -
//void Options::xPressed()
//{
	//rightPressed();
//}


// ������� ��� ��������� ���� �����
// Input: 
//		   target - ����������, ������� ������ ���������� ��� ���������
// 
//		   states - ��� ��� ���� ���������� �� �������� ��������� �������� �������, 
//		   �� �� ����� ���� ����������, ������� � ���� ���������� �������� 
//		   ��� ��������� �����������
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