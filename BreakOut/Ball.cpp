#define _USE_MATH_DEFINES

#include "Ball.h"
#include "Player.h"
#include "Brick.h"

#include <iostream>
#include <list>
#include <cmath>
#include <utility>
#include <ctime>
#include <cstdlib>

extern std::pair<int, int> windowSize;

extern int stage;
extern int death;

extern long prevTime;
extern long curTime;
extern Player player;
extern std::list<Brick> bricks;

Ball::Ball() {
	srand(time(NULL));
	x = (rand() % 3 + 1) * 75;
	y = 300;
	radius = 5;
	velocity = 0;
	angle = 0;
	latest = -2;
}

double Ball::getVelocity() {
	return velocity;
}
double Ball::getAngle() {
	return angle;
}
void Ball::setInitial() {
	srand(time(NULL));
	velocity = 0.5;
	angle = rand() % 2 == 0 ? 315 : 225;
}

void Ball::update() {

	if (y >= windowSize.second - radius) {
		//stage = 0;
		srand(time(NULL));
		x = (rand() % 3 + 1) * 75;
		y = 300;
		velocity = 0;
		angle = 0;
		latest = -2;
		death++;
	}
	else if (y <= radius && latest != down) {
		changeAngleDown();
	}
	else if (x <= radius && latest != right) {
		changeAngleRight();
	}
	else if (x >= windowSize.first - radius && latest != left) {
		changeAngleLeft();
	}
	else if (y >= player.getY() - player.getHeight() / 2 && y <= player.getY() - player.getHeight() / 2 + radius * 2) {
		if (x >= player.getX() - player.getWidth() / 2 - radius && x <= player.getX() + player.getWidth() / 2 + radius) {
			if (latest != -1) {
				changeAngleUp();
				latest = -1;
			}
		}
	}
	else {
		bool collided = false;
		if(bricks.size() != 0){
			for (std::list<Brick>::iterator iter = bricks.begin(); iter != bricks.end();) {
				Brick brick = *iter;

				//1번(왼쪽위) y = x - (brick.x - Brick::width / 2) + (brick.y - Brick::height / 2)
				//2번(왼쪽아래) y = -x + (brick.x - Brick::width / 2) + (brick.y + Brick::height / 2)
				//3번(오른쪽위) y = -x + (brick.x + Brick::width / 2) + (brick.y - Brick::height / 2)
				//4번(오른쪽아래) y = x - (brick.x + Brick::width / 2) + (brick.y + Brick::height / 2)

				//up (1↓, 3↓)
				if ((y <= x - (brick.x - Brick::width / 2) + (brick.y - Brick::height / 2))
					&& (y < -x + (brick.x + Brick::width / 2) + (brick.y - Brick::height / 2))
					&& (y >= brick.y - Brick::height / 2)
					&& (y <= brick.y - Brick::height / 2 + radius * 2)) {
					if (!collided) {
						collided = true;
						changeAngleUp();
					}
					iter = bricks.erase(iter);
				}
				//down (2↑, 4↑)
				else if ((y > -x + (brick.x - Brick::width / 2) + (brick.y + Brick::height / 2))
					&& (y >= x - (brick.x + Brick::width / 2) + (brick.y + Brick::height / 2))
					&& (y >= brick.y + Brick::height / 2 - radius * 2)
					&& (y <= brick.y + Brick::height / 2)) {
					if (!collided) {
						collided = true;
						changeAngleDown();
					}
					iter = bricks.erase(iter);
				}

				//left (1↑, 2↓)
				else if ((y > x - (brick.x - Brick::width / 2) + (brick.y - Brick::height / 2))
					&& (y <= -x + (brick.x - Brick::width / 2) + (brick.y + Brick::height / 2))
					&& (x >= brick.x - Brick::width / 2)
					&& (x <= brick.x - Brick::width / 2 + radius * 2)) {
					if (!collided) {
						collided = true;
						changeAngleLeft();
					}
					iter = bricks.erase(iter);
				}

				//right (3↑, 4↓)
				else if ((y >= -x + (brick.x + Brick::width / 2) + (brick.y - Brick::height / 2))
					&& (y < x - (brick.x + Brick::width / 2) + (brick.y + Brick::height / 2))
					&& (x >= brick.x + Brick::width / 2 - radius * 2)
					&& (x <= brick.x + Brick::width / 2)) {
					if (!collided) {
						collided = true;
						changeAngleRight();
					}
					iter = bricks.erase(iter);
				}
				else {
					iter++;
				}
			}

			if (bricks.size() == 0) {
				stage++;

				srand(time(NULL));
				x = (rand() % 3 + 1) * 75;
				y = 300;
				velocity = 0;
				angle = 0;
				latest = -1;
			}
		}
	}

	x += (curTime - prevTime) * velocity * cos(angle * M_PI / 180);
	y += (curTime - prevTime) * velocity * sin(angle * M_PI / 180);
}
void Ball::paint(HDC hdc) {
	Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);
	//Ellipse(hdc, 300, 500, 400, 600);
}

void Ball::changeAngleUp() {
	latest = up;
	angle = 360 - angle;
}
void Ball::changeAngleDown() {
	latest = down;
	angle = 360 - angle;
}
void Ball::changeAngleLeft() {
	latest = left;
	angle = 180 - angle;
}
void Ball::changeAngleRight() {
	latest = right;
	angle = 180 - angle;
}