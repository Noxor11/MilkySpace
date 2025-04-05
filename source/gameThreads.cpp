#include "gameThreads.h"


volatile bool runThreads = true;
volatile bool canShoot = true;
volatile u64 sleepTime = 3e8;
volatile int fade = 255;
volatile bool health_low = false;


Thread threads[NUMTHREADS];



void threadShoot(void* duration)
{
	while (runThreads)
	{
		if (!canShoot) {
			for (float i = *((u64*)duration) / 10; i < *((u64*)duration) && !canShoot; i += *((u64*)duration) / 10) {
				svcSleepThread(*((u64*)duration) / 10);
			}
				canShoot = true;
		}
		else
			svcSleepThread(1e8);
	    // Mix_PlayChannel(-1, (Mix_Chunk*) arg, 0);
	}
}

void threadFadeIn(void* fade) {
	
	const int velocity = 10;

	while (*(int*)fade > 0) {
		if (*(int*)fade - velocity < 0)
			*(int*)fade = 0;
		else
			*(int*)fade -= velocity;
	
		svcSleepThread(1e8);
	}
}

void threadFadeOut(void* fade) {

	while (*(int*)fade < 255) {
		
		if (*(int*)fade - 5 > 255)
			*(int*)fade = 255;
		else
			*(int*)fade += 5;
	
		svcSleepThread(1e8);
	}
}

void threadWait(void* round_ready) {
	svcSleepThread(2e9);
	*(bool*)round_ready = true;
}

void threadLowHealth(void* has_low_health) {
	while (runThreads) {
		if (*(bool*)has_low_health) {
			Mix_PlayChannel(-1, low_health, 0);
		}
		svcSleepThread(1e9);

	}
}
