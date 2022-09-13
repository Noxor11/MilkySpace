#pragma once
#include "gameObject.h"


class Projectile : public Object{

private:
    bool displayOnBottom;
    bool displayOnTop;

    float damage;
    float velocity;

public:

	Projectile(C2D_SpriteSheet* spriteSheet, int index);
    Projectile(C2D_SpriteSheet* spriteSheet, int index, float damage);
	Projectile(C2D_SpriteSheet* spriteSheet, int index, float damage, float velocity);
	Projectile(C2D_Image img);

    bool isDisplayOnBottom();
    bool isDisplayOnTop();

    float getDamage();
    void setDamage(float damage);

    float getVelocity();
    void setVelocity(float damage);

    void setDisplayOnBottom(bool d);
    void setDisplayOnTop(bool d);

};