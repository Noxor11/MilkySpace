#pragma once
#include "gameSpecification.h"
#include "projectile.h"
#include "gameObject.h"

#include <memory>

#define DEFAULT_PLAYER_LIFE 10

struct ammunition {
    int bomb;
    int special;
};

enum gun {
    BULLET_GUN, HEAVY_BULLET_GUN, BOMB_GUN
};

class Ship : public Object {

private:

    float life;
    ammunition ammo;
    gun current_gun;
    
public:

	Ship(SpriteSheet* spriteSheet, int index);
	Ship(SpriteSheet* spriteSheet, int index, float xPosition, float yPosition);
	Ship(Image img);

    virtual std::shared_ptr<Projectile> shoot();

    gun getGun();
    void changeGun();

    float getLife();
    void setLife(float ammount);
    void changeLifeBy(float ammount);

    void resetState();

};
