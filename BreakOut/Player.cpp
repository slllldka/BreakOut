#include "Player.h"

#include <iostream>
#include <utility>

extern std::pair<int, int> windowSize;
extern double windowScale;

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
	RECT rect = { x - width / 2, y - height / 2, x + width / 2, y + height / 2 };
	rect.left *= windowScale;
	rect.top *= windowScale;
	rect.right *= windowScale;
	rect.bottom *= windowScale;
	Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
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