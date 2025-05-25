#pragma once

#include "gameSpecification.h"

#include <assert.h>
#include <stdlib.h>
#include <unistd.h>


#define MAX_SPRITES			768

const int BACKGROUND_TOP 	=	0;
const int BACKGROUND_BOT 	=	1;
const int MOVING_BG_TOP_1	=	2;
const int MOVING_BG_TOP_2	=	3;
const int MOVING_BG_BOT_1	=	4;
const int MOVING_BG_BOT_2	=	5;
const int ENEMY_1			=	6;
const int ENEMY_2			=	7;
const int ENEMY_3			=	8;
const int PLAYER			=	9;
const int BULLET			=	10;
const int HEAVY_BULLET		=	11;
const int BOMB				=	12;
const int ENEMY_BULLET		=	13;
const int BOTTOM_BAR		=	14;
const int SELECTOR			=	15;

class Object {
	
	Sprite spr;

public:
	float px() const;
	float py() const;
	float real_px() const;
	float real_py() const;
	float width() const;
	float height() const;
	bool hasSamePosAs(const Object* const object) const;
	bool hasSamePosAs(const Object& object) const;

	void setX(float newPosition);
	void setY(float newPosition);
	void setPos(float xPosition, float yPosition);
	void setPos(Object* object);
	void moveXBy(float increment);
	void moveYBy(float increment);
	void moveBy(float xIncrement, float yIncrement);

	bool followTrace(float x, float y);

	void setImage(Image img);
	Image getImage() const;
	const Image* getImagePtr() const;

	Sprite getSprite() const;
	const Sprite* getSpritePtr() const;

	Object(const Object* const object);
	Object(const SpriteSheet* const spriteSheet, int index);
	Object(const SpriteSheet* const spriteSheet, int index, float xPosition, float yPosition);
	Object(Image img);
	Object() = delete;

};
