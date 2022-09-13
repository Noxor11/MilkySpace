#include "projectile.h"


Projectile::Projectile(C2D_SpriteSheet* spriteSheet, int index) : Object::Object(spriteSheet, index){
    displayOnBottom = false,
    displayOnTop = false;     
    damage = 1;
}

Projectile::Projectile(C2D_SpriteSheet* spriteSheet, int index, float damage) : Object::Object(spriteSheet, index){
    displayOnBottom = false,
    displayOnTop = false;     
    this->damage = damage;
}

Projectile::Projectile(C2D_SpriteSheet* spriteSheet, int index, float damage, float velocity) : Object::Object(spriteSheet, index){
    this->damage = damage;
    this->velocity = velocity;
    displayOnBottom = false,
    displayOnTop = false;     
    damage = 1;
}

Projectile::Projectile(C2D_Image img) : Object::Object(img){
    displayOnBottom = false,
    displayOnTop = false;     
    damage = 1;
}




bool Projectile::isDisplayOnBottom(){ return displayOnBottom; }
bool Projectile::isDisplayOnTop(){ return displayOnTop; }

float Projectile::getDamage(){ return damage; }

void Projectile::setDamage(float damage){
    this->damage = damage;
}

float Projectile::getVelocity(){ return velocity; }

void Projectile::setVelocity(float velocity){
    this->velocity = velocity;
}



void Projectile::setDisplayOnBottom(bool d){
    displayOnBottom = d;
}

void Projectile::setDisplayOnTop(bool d){
    displayOnTop = d;
}