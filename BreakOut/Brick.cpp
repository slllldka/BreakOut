#include "Brick.h"


Brick::Brick(double x, double y, int brushIdx) {
	this->x = x;
	this->y = y;
	this->brushIdx = brushIdx;
}
HBRUSH Brick::brush[7] = {CreateSolidBrush(RGB(255, 0, 0)), CreateSolidBrush(RGB(255, 128, 0)), CreateSolidBrush(RGB(255, 255, 0)),
						  CreateSolidBrush(RGB(0, 255, 0)), CreateSolidBrush(RGB(0, 0, 255)), CreateSolidBrush(RGB(0, 0, 128)), CreateSolidBrush(RGB(128, 0, 255)) };

void Brick::paint(HDC hdc) {
	RECT rect = { x - width / 2 + 2, y - height / 2 + 2, x + width / 2 - 2, y + height / 2 - 2 };
	Rectangle(hdc, x - width / 2 + 1, y - height / 2 + 1, x + width / 2 - 1, y + height / 2 - 1);
	FillRect(hdc, &rect, brush[brushIdx]);
}