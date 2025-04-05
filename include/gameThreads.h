#pragma once 

#include "sound.h"
#include "global.h"
#include "projectile.h"
#include "ship.h"


#include <stdio.h>
#include <memory>
#include <vector>
#include <3ds.h>

#define NUMTHREADS 4
#define STACKSIZE (4 * 1024)

extern volatile bool runThreads;
extern volatile bool canShoot;
extern volatile u64 sleepTime;
extern volatile int fade;
extern volatile bool health_low;

extern Thread threads[NUMTHREADS];

void threadShoot(void* player);

void threadFadeIn(void* fade);
void threadFadeOut(void* fade);

void threadWait(void* round_ready);

void threadLowHealth(void* has_low_health);
