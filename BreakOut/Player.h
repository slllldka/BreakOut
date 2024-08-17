#pragma once

#include "framework.h"

class Player {
private:
	double x, y;
	int width, height;
	double velocity;
public:
	Player();

	double getX();
	double getY();
	int getWidth();
	int getHeight();
	double getVelocity();
	void update();
	void paint(HDC);
	void moveLeft();
	void moveRight();
};