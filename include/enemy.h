#pragma once

#include "ship.h"
#include "global.h"

#include <random>

class Enemy : public Ship {
    
private:

    struct Trace{
        float xPos;
        float yPos;
    };

    Trace trace;

public:

    Enemy(C2D_SpriteSheet* spriteSheet, int index);
	Enemy(C2D_SpriteSheet* spriteSheet, int index, float xPosition, float yPosition);
	Enemy(C2D_Image img);

    std::shared_ptr<Projectile> shoot() override;

    bool moveRandomly();
};
