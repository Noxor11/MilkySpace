#pragma once
#include "gameObject.h"


class Projectile : public Object {

private:
    bool displayOnBottom;
    bool displayOnTop;

    float damage;
    float velocity;

public:

	Projectile(SpriteSheet* spriteSheet, int index);
    Projectile(SpriteSheet* spriteSheet, int index, float damage);
	Projectile(SpriteSheet* spriteSheet, int index, float damage, float velocity);
	Projectile(Image img);

    bool shouldDisplayOnBottom();
    bool shouldDisplayOnTop();

    float getDamage();
    void setDamage(float damage);

    float getVelocity();
    void setVelocity(float damage);

    void setDisplayOnBottom(bool d);
    void setDisplayOnTop(bool d);

};
