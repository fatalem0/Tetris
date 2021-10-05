#include "buttons.h"
#include <iostream>

// ������� ����������, ������� ����������, ������������ �� ������� ������, �� ��������� ��� �� ������������
// � ��������� � ����
volatile unsigned int Buttons::upHold = 0,
					  Buttons::downHold = 0, 
					  Buttons::leftHold = 0,
					  Buttons::rightHold = 0;

// �������, ����������� ������� ������ �����
// Input: -
// Output: upHold
unsigned int Buttons::upPressed()
{
	// ���� ��������� ������ ������ ����� ������������� ������ ��������, �� ��� ������ �������� 1
	if (++upHold == UINT32_MAX)
		upHold = 1;
	return upHold;
}

// �������, ����������� ������� ������ ����
// Input: -
// Output: downHold
unsigned int Buttons::downPressed()
{
	// ���� ��������� ������ ������ ����� ������������� ������ ��������, �� ��� ������ �������� 1
	if (++downHold == UINT32_MAX)
		downHold = 1;
	return downHold;
}

// �������, ����������� ������� ������ �����
// Input: -
// Output: leftHold
unsigned int Buttons::leftPressed()
{
	// ���� ��������� ������ ������ ����� ������������� ������ ��������, �� ��� ������ �������� 1
	if (++leftHold == UINT32_MAX)
		leftHold = 1;
	return leftHold;
}

// �������, ����������� ������� ������ ������
// Input: -
// Output: rightHold
unsigned int Buttons::rightPressed()
{
	// ���� ��������� ������ ������ ����� ������������� ������ ��������, �� ��� ������ �������� 1
	if (++rightHold == UINT32_MAX)
		rightHold = 1;
	return rightHold;
}

// �������, ����������� ������ ������ �����
// Input: -
// Output: 
void Buttons::upReleased()
{
	upHold = 0;
}

// �������, ����������� ������ ������ ����
// Input: -
// Output: 
void Buttons::downReleased()
{
	downHold = 0;
}

// �������, ����������� ������ ������ �����
// Input: -
// Output: 
void Buttons::leftReleased()
{
	leftHold = 0;
}

// �������, ����������� ������ ������ ������
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