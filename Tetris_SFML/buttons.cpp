#include "buttons.h"
#include <iostream>

// объ€вим переменные, которые показывают, удерживаетс€ ли нажата€ кнопка, по умолчанию они не удерживаютс€
// и наход€тс€ в нуле
volatile unsigned int Buttons::upHold = 0,
					  Buttons::downHold = 0, 
					  Buttons::leftHold = 0,
					  Buttons::rightHold = 0;

// функци€, провер€юща€ нажатие кнопки вверх
// Input: -
// Output: upHold
unsigned int Buttons::upPressed()
{
	// если удержание кнопки станет равно максимальному целому значению, то оно примет значение 1
	if (++upHold == UINT32_MAX)
		upHold = 1;
	return upHold;
}

// функци€, провер€юща€ нажатие кнопки вниз
// Input: -
// Output: downHold
unsigned int Buttons::downPressed()
{
	// если удержание кнопки станет равно максимальному целому значению, то оно примет значение 1
	if (++downHold == UINT32_MAX)
		downHold = 1;
	return downHold;
}

// функци€, провер€юща€ нажатие кнопки влево
// Input: -
// Output: leftHold
unsigned int Buttons::leftPressed()
{
	// если удержание кнопки станет равно максимальному целому значению, то оно примет значение 1
	if (++leftHold == UINT32_MAX)
		leftHold = 1;
	return leftHold;
}

// функци€, провер€юща€ нажатие кнопки вправо
// Input: -
// Output: rightHold
unsigned int Buttons::rightPressed()
{
	// если удержание кнопки станет равно максимальному целому значению, то оно примет значение 1
	if (++rightHold == UINT32_MAX)
		rightHold = 1;
	return rightHold;
}

// функци€, провер€юща€ отпуск кнопки вверх
// Input: -
// Output: 
void Buttons::upReleased()
{
	upHold = 0;
}

// функци€, провер€юща€ отпуск кнопки вниз
// Input: -
// Output: 
void Buttons::downReleased()
{
	downHold = 0;
}

// функци€, провер€юща€ отпуск кнопки влево
// Input: -
// Output: 
void Buttons::leftReleased()
{
	leftHold = 0;
}

// функци€, провер€юща€ отпуск кнопки вправо
// Input: -
// Output: 
void Buttons::rightReleased()
{
	rightHold = 0;
}


bool Buttons::activateInput(unsigned int val, unsigned int rate, bool delayBetweenFirstPress)
{
	if (delayBetweenFirstPress)
		return ((val < rate) || (val > rate*3)) && !(val % rate - 1);
	return !(val % rate - 1);
}