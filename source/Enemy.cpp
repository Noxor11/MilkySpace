#include "enemy.h"

#define DEFAULT_ENEMY_LIFE 5

#define RANDOM_X random() % TOP_SCREEN_WIDTH  + width()
#define RANDOM_Y (random() % (TOP_SCREEN_HEIGHT - (int) height())) + height()


Enemy::Enemy(C2D_SpriteSheet* spriteSheet, int index) 
    :   Ship::Ship(spriteSheet, index),
        trace({xPos: (float)(RANDOM_X), yPos: (float)(RANDOM_Y)}){
            this->setLife(DEFAULT_ENEMY_LIFE);

}


Enemy::Enemy(C2D_SpriteSheet* spriteSheet, int index, float xPosition, float yPosition) 
    :   Ship::Ship(spriteSheet, index, xPosition, yPosition),
        trace({xPos: (float)(RANDOM_X), yPos: (float)(RANDOM_Y)}){
            this->setLife(DEFAULT_ENEMY_LIFE);

}

Enemy::Enemy(C2D_Image img) 
    :   Ship::Ship(img),
        trace({xPos: (float)(RANDOM_X), yPos: (float)(RANDOM_Y)}){
            this->setLife(DEFAULT_ENEMY_LIFE);
}

bool Enemy::moveRandomly(){
    bool reachedTrace = followTrace(trace.xPos, trace.yPos);
    if(reachedTrace){
        trace.xPos = RANDOM_X;
        trace.yPos = RANDOM_Y;
    }

    return reachedTrace;
}

std::shared_ptr<Projectile> Enemy::shoot(){

    int gun = ENEMY_BULLET;
    float damage = 1;
    float velocity = -5;

    switch(this->getGun()){

        case BULLET_GUN:
            gun = ENEMY_BULLET;
            damage = 1;
            velocity = -8;
            break;

        case HEAVY_BULLET_GUN:
            gun = HEAVY_BULLET;
            damage = 3;
            velocity = -3;
            break;
        
        case BOMB_GUN:
            gun = BOMB;
            damage = 2;
            velocity  = -4;
            break;


    }
    std::shared_ptr<Projectile> p = std::make_shared<Projectile>(&spriteSheet, gun, damage, velocity);
    p->setDisplayOnTop(true);
    p->setDisplayOnBottom(false);

    return p;
}
