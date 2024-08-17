#include "Player.h"

#include <iostream>
#include <utility>

extern std::pair<int, int> windowSize;

extern bool leftPressed;
extern bool rightPressed;

extern long leftPressedTime;
extern long rightPressedTime;
extern long leftReleasedTime;
extern long rightReleasedTime;

extern long prevTime;
extern long curTime;

Player::Player() {
	x = 150;
	y = 350;
	width = 50;
	height = 20;
	velocity = 0.3;
}

double Player::getX() {
	return x;
}
double Player::getY() {
	return y;
}
int Player::getWidth() {
	return width;
}
int Player::getHeight() {
	return height;
}
double Player::getVelocity() {
	return velocity;
}

void Player::update() {
	if (leftPressed && rightPressed) {
		leftPressedTime > rightPressedTime ? moveLeft() : moveRight();
	}
	else if (leftPressed && !rightPressed) {
		moveLeft();
	}
	else if (rightPressed && !leftPressed) {
		moveRight();
	}
}

void Player::paint(HDC hdc) {
	Rectangle(hdc, x - width / 2, y - height / 2, x + width / 2, y + height / 2);
	/*Rectangle(hdc, 0, 0, 100, 100);
	Rectangle(hdc, 0, 100, 100, 200);
	Rectangle(hdc, 0, 200, 100, 300);
	Rectangle(hdc, 0, 300, 100, 400);
	Rectangle(hdc, 0, 400, 100, 500);
	Rectangle(hdc, 0, 500, 100, 600);

	Rectangle(hdc, 0, 0, 100, 100);
	Rectangle(hdc, 100, 0, 200, 100);
	Rectangle(hdc, 200, 0, 300, 100);
	Rectangle(hdc, 300, 0, 400, 100);
	Rectangle(hdc, 400, 0, 500, 100);
	Rectangle(hdc, 500, 0, 600, 100);
	Rectangle(hdc, 600, 0, 700, 100);
	Rectangle(hdc, 700, 0, 800, 100);*/
}

void Player::moveLeft() {
	if (prevTime < leftPressedTime) {
		x -= (curTime - leftPressedTime) * velocity;
	}
	else {
		x -= (curTime - prevTime) * velocity;
	}

	if (x - width / 2 < 0) {
		x = width / 2;
	}
}

void Player::moveRight() {
	if (prevTime < rightPressedTime) {
		x += (curTime - rightPressedTime) * velocity;
	}
	else {
		x += (curTime - prevTime) * velocity;
	}
	if (x + width / 2 > windowSize.first) {
		x = windowSize.first - width / 2;
	}
}