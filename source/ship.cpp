#include "ship.h"


Ship::Ship(C2D_SpriteSheet* spriteSheet, int index) : Object::Object(spriteSheet, index){
    life = DEFAULT_PLAYER_LIFE;
    ammo = (ammunition){3, -1};
    current_gun = BULLET_GUN;
}


Ship::Ship(C2D_SpriteSheet* spriteSheet, int index, float xPosition, float yPosition) : Object::Object(spriteSheet, index, xPosition, yPosition){
    life = DEFAULT_PLAYER_LIFE;
    ammo = (ammunition){3, -1};
    current_gun = BULLET_GUN;
}

Ship::Ship(C2D_Image img) : Object::Object(img){
    life = DEFAULT_PLAYER_LIFE;
    ammo = (ammunition){3, -1};
    current_gun = BULLET_GUN;
}


std::shared_ptr<Projectile> Ship::shoot(){

    int gun = BULLET;
    float damage = 1;
    float velocity = 5;

    switch(current_gun){

        case BULLET_GUN:
        	Mix_PlayChannel(-1, laser, 0);

            gun = BULLET;
            damage = 1;
            velocity = 10;
            break;

        case HEAVY_BULLET_GUN:
            Mix_PlayChannel(-1, heavy_bullet, 0);

            gun = HEAVY_BULLET;
            damage = 3;
            velocity = 15;
            break;
        
        case BOMB_GUN:
            Mix_PlayChannel(-1, heavy_bullet, 0);

            gun = BOMB;
            damage = 5;
            velocity  = 20;
            break;


    }

    return std::make_shared<Projectile>(&spriteSheet, gun, damage, velocity);
}

gun Ship::getGun(){ return current_gun;}


void Ship::changeGun(){

    current_gun = (gun)((current_gun+1) %3);
    
    
    switch(current_gun){
        case BULLET_GUN:
            sleepTime = 3e8;
            break;

        case HEAVY_BULLET_GUN:
            sleepTime = 1e9;
            break;
        
        case BOMB_GUN:
            sleepTime = 5e9;
            break; 

    }


}

float Ship::getLife(){ return life;}

void Ship::setLife(float ammount){
    this->life = ammount;
}

void Ship::changeLifeBy(float ammount){
    this->life += ammount;
}


void Ship::resetState(){
    this->life = DEFAULT_PLAYER_LIFE;
    this->ammo = (ammunition){3, -1};
    this->current_gun = BULLET_GUN;
}
