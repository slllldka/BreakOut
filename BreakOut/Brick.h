#pragma once

#include "framework.h"

class Brick {
public:
	static const int width = 50, height = 20;
	static HBRUSH brush[7];
	double x, y;
	int brushIdx;
public:
	Brick(double, double, int);
	void paint(HDC);
};