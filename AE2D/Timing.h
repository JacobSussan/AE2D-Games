#pragma once

namespace AE {

//-------------------------------------------------------------------
class FpsLimiter 
{
public:
	FpsLimiter();

	void init(float maxFPS);
	void setMaxFPS(float maxFPS);
	void begin();
	float end(); //return FPS
private:
	void calculatFPS();
	unsigned int _startTicks;

	float m_maxFPS;
	float m_fps;
	float m_frameTime;
};
}

