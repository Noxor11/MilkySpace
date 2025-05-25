#pragma once

#include "gameSpecification.h"
#include "ship.h"


class Enemy : public Ship {
    
private:

    struct Trace{
        float xPos;
        float yPos;
    };

    Trace trace;

public:

    Enemy(SpriteSheet* spriteSheet, int index);
	Enemy(SpriteSheet* spriteSheet, int index, float xPosition, float yPosition);
	Enemy(Image img);

    std::shared_ptr<Projectile> shoot() override;

    bool moveRandomly();
};
