#include "gameThreads.h"
#include "gameSpecification.h"
#include "sound.h"

volatile bool runThreads = true;
volatile bool canShoot = true;
volatile uint64_t sleepTime = 3e8;
volatile int fade = 255;
volatile bool health_low = false;


Thread threads[NUMTHREADS];



void threadShoot(void* duration)
{
	while (runThreads)
	{
		if (!canShoot) {
			for (float i = *((uint64_t*)duration) / 10; i < *((uint64_t*)duration) && !canShoot; i += *((uint64_t*)duration) / 10) {
				SleepThread(*((uint64_t*)duration) / 10);
			}
				canShoot = true;
		}
		else
			SleepThread(1e8);
	}
}

void threadFadeIn(void* fade) {
	
	const int velocity = 10;

	while (*(int*)fade > 0) {
		if (*(int*)fade - velocity < 0)
			*(int*)fade = 0;
		else
			*(int*)fade -= velocity;
	
		SleepThread(1e8);
	}
}

void threadFadeOut(void* fade) {

	while (*(int*)fade < 255) {
		
		if (*(int*)fade - 5 > 255)
			*(int*)fade = 255;
		else
			*(int*)fade += 5;
	
		SleepThread(1e8);
	}
}

void threadWait(void* round_ready) {
	SleepThread(2e9);
	*(bool*)round_ready = true;
}

void threadLowHealth(void* has_low_health) {
	while (runThreads) {
		if (*(bool*)has_low_health) {
			PlayChunkOnChannel(-1, low_health, 0);
		}
		SleepThread(1e9);
	}
}

void exitThreads() {
	// tell threads to exit & wait for them to exit
	runThreads = false;
	for (int i = 0; i < NUMTHREADS; i ++)
	{
		JoinThread(threads[i], 1e8);
		FreeThread(threads[i]);
	}
}