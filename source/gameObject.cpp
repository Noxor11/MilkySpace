#include "gameObject.h"
#include "global.h"


float Object::px() const{
	return spr.params.pos.x + spr.params.pos.w / 2;
}

float Object::py() const{
	return spr.params.pos.y + spr.params.pos.h / 2;
}

float Object::real_px() const{ return spr.params.pos.x; }
float Object::real_py() const{ return spr.params.pos.y; }

float Object::width()  const{ return spr.params.pos.w; }
float Object::height() const{ return spr.params.pos.h; }

bool Object::hasSamePosAs(const Object* const object) const {
	return real_px() == object->real_px() &&
		   real_py() == object->real_py();
}

bool Object::hasSamePosAs(const Object& object) const{
	return real_px() == object.real_px() &&
		   real_py() == object.real_py();
}


void Object::setX(float newPosition) {
	spr.params.pos.x = newPosition + spr.params.pos.w / 2;
}

void Object::setY(float newPosition) {
	spr.params.pos.y = newPosition + spr.params.pos.h / 2;
}

void Object::setPos(float xPosition, float yPosition) {
    
	spr.params.pos.x = xPosition + spr.params.pos.w / 2; 
	spr.params.pos.y = yPosition + spr.params.pos.h / 2;
}

void Object::setPos(Object* object) {
	spr.params.pos.x = object->spr.params.pos.x;
	spr.params.pos.y = object->spr.params.pos.y;

}

C2D_Sprite Object::getSprite() const {
	return spr;
}

const C2D_Sprite* Object::getSpritePtr() const {
	return &spr;
}

C2D_Image Object::getImage() const {
	return spr.image;
}

const C2D_Image* Object::getImagePtr() const {
	return &spr.image;
}

void Object::setImage(C2D_Image img) {
	spr.image = img; 
}



void Object::moveXBy(float increment) {
	spr.params.pos.x += increment;
}

void Object::moveYBy(float increment) {
	spr.params.pos.y -= increment;
}

void Object::moveBy(float xIncrement, float yIncrement) {
	moveXBy(xIncrement);
	moveYBy(yIncrement);
}

/**
 * @brief Moves object to a position, following a trace.
 * 
 * @return true if it got to the destination.
 * @return false if it hasn't reached the destination.
 */
bool Object::followTrace(float x, float y){
	bool reached_x = false, reached_y = false;

	float speed = 8;

	if(px() > x){

		if(px() - speed < x){
			moveXBy(x - px());
		}
		else
			moveXBy(-speed);
	}
	else if(px() < x){

		if(px() + speed > x){
			moveXBy(x - px());
		}
		else
			moveXBy(speed);	
			
	}
	else
		reached_x = true;

	if(py() < y){

		if(py() - speed > y)
			moveYBy(y - py());
		
		else
			moveYBy(-speed);
	}
	else if(py() > y){

		if(py() - speed < y)
			moveYBy(py() - y);
		else
			moveYBy(speed);	
			
	}

	else
		reached_y = true;

	return reached_x && reached_y;
}

Object::Object(const Object* object) : 
	Object::Object(*object){}

Object::Object(const C2D_SpriteSheet* const spriteSheet, int index) : 
	Object::Object(spriteSheet, index, 0, 0){}

Object::Object(const C2D_SpriteSheet* const spriteSheet, int index, float xPosition, float yPosition) {
	C2D_SpriteFromSheet(&spr, *spriteSheet, index);
	C2D_SpriteSetCenter(&spr, 0.5f, 0.5f);
	setPos(xPosition, yPosition);}

Object::Object(C2D_Image img) {
	C2D_SpriteFromImage(&spr, img);
	C2D_SpriteSetCenter(&spr, 0.5f, 0.5f);
    setPos(0,0);
}

