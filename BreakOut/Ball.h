#pragma once

#include "framework.h"

class Ball {
private:
	double x, y;
	double radius;
	double velocity;
	double angle;
	const int up = 0, down = 1, left = 2, right = 3;
	int latest;
public:
	Ball();
	double getVelocity();
	double getAngle();
	void setInitial();
	void update();
	void paint(HDC);
	void changeAngleUp();
	void changeAngleDown();
	void changeAngleLeft();
	void changeAngleRight();
};