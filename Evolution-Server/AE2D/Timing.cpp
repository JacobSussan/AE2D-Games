#include "Timing.h"

#include <SDL/SDL.h>


//-------------------------------------------------------------------
AE::FpsLimiter::FpsLimiter() 
{

}

//-------------------------------------------------------------------
void AE::FpsLimiter::init(float maxFPS) 
{
	setMaxFPS(maxFPS);
}

//-------------------------------------------------------------------
void AE::FpsLimiter::setMaxFPS(float maxFPS) 
{
	m_maxFPS = maxFPS;
}
//-------------------------------------------------------------------
void AE::FpsLimiter::begin() 
{
	_startTicks = SDL_GetTicks();
}

//-------------------------------------------------------------------
float AE::FpsLimiter::end() //return FPS
{
	calculatFPS();

	float frameTicks = SDL_GetTicks() - _startTicks;
	//Limit the FPS to the max FPS
	if (1000.0f / m_maxFPS > frameTicks) {
		SDL_Delay((Uint32)(1000.0f / m_maxFPS - frameTicks));
	}
	return m_fps;
}

//-------------------------------------------------------------------
void AE::FpsLimiter::calculatFPS() 
{
	static const int NUM_SAMPLES = 10; 	        //The number of frames to average
	static float frameTimes[NUM_SAMPLES]; 	    //Stores all the frametimes for each frame that we will average
	static int currentFrame = 0;                //The current frame we are on
	static float prevTicks = SDL_GetTicks();	//the ticks of the previous frame

	float currentTicks = SDL_GetTicks();        //Ticks for the current frame

	//Calculate the number of ticks (ms) for this frame
	m_frameTime = currentTicks - prevTicks;
	frameTimes[currentFrame % NUM_SAMPLES] = m_frameTime;

	prevTicks = currentTicks; 	//current ticks is now previous ticks
	int count; 	                //The number of frames to average

	currentFrame++;
	if (currentFrame < NUM_SAMPLES) {
		count = currentFrame;
	} else {
		count = NUM_SAMPLES;
	}

	//Average all the frame times
	float frameTimeAverage = 0;
	for (int i = 0; i < count; i++) {
		frameTimeAverage += frameTimes[i];
	}
	frameTimeAverage /= count;

	//Calculate FPS
	if (frameTimeAverage > 0) {
		m_fps = 1000.0f / frameTimeAverage;
	} else {
		m_fps = 60.0f;
	}
}

