#pragma once
#include "gameObject.h"
#include "projectile.h"
#include "scene.h"
#include "gameThreads.h"
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

	Ship(C2D_SpriteSheet* spriteSheet, int index);
	Ship(C2D_SpriteSheet* spriteSheet, int index, float xPosition, float yPosition);
	Ship(C2D_Image img);

    virtual std::shared_ptr<Projectile> shoot();

    gun getGun();
    void changeGun();

    float getLife();
    void setLife(float ammount);
    void changeLifeBy(float ammount);

    void resetState();

};
