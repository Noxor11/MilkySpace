#pragma once

#include <citro2d.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <citro2d.h>
#include <3ds.h>
#include <string>
#include <array>
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
	
	C2D_Sprite spr;

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


	void setImage(C2D_Image img);
	C2D_Image getImage() const;
	const C2D_Image* getImagePtr() const;

	C2D_Sprite getSprite() const;
	const C2D_Sprite* getSpritePtr() const;



	Object(const Object* const object);
	Object(const C2D_SpriteSheet* const spriteSheet, int index);
	Object(const C2D_SpriteSheet* const spriteSheet, int index, float xPosition, float yPosition);
	Object(C2D_Image img);
	Object() = delete;


};
