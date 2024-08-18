#include "Brick.h"

extern double windowScale;

Brick::Brick(double x, double y, int brushIdx) {
	this->x = x;
	this->y = y;
	this->brushIdx = brushIdx;
}
HBRUSH Brick::brush[7] = {CreateSolidBrush(RGB(255, 0, 0)), CreateSolidBrush(RGB(255, 128, 0)), CreateSolidBrush(RGB(255, 255, 0)),
						  CreateSolidBrush(RGB(0, 255, 0)), CreateSolidBrush(RGB(0, 0, 255)), CreateSolidBrush(RGB(0, 0, 128)), CreateSolidBrush(RGB(128, 0, 255)) };

void Brick::paint(HDC hdc) {
	RECT outline = { x - width / 2 + 1, y - height / 2 + 1, x + width / 2 - 1, y + height / 2 - 1 };
	RECT inside = { x - width / 2 + 2, y - height / 2 + 2, x + width / 2 - 2, y + height / 2 - 2 };
	outline.left *= windowScale;
	outline.top *= windowScale;
	outline.right *= windowScale;
	outline.bottom *= windowScale;
	inside.left *= windowScale;
	inside.top *= windowScale;
	inside.right *= windowScale;
	inside.bottom *= windowScale;
	Rectangle(hdc, outline.left, outline.top, outline.right, outline.bottom);
	FillRect(hdc, &inside, brush[brushIdx]);
}