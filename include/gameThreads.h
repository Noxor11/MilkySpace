#pragma once 

#include "gameSpecification.h"

#define NUMTHREADS 4
#define STACKSIZE (4 * 1024)

extern volatile bool runThreads;
extern volatile bool canShoot;
extern volatile uint64_t sleepTime;
extern volatile int fade;
extern volatile bool health_low;

extern Thread threads[NUMTHREADS];

void threadShoot(void* player);

void threadFadeIn(void* fade);
void threadFadeOut(void* fade);

void threadWait(void* round_ready);

void threadLowHealth(void* has_low_health);

void exitThreads();