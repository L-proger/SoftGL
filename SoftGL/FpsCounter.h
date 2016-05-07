#ifndef FpsCounter_h__
#define FpsCounter_h__

#include "stopwatch.h"

#define MAX_AVG_STEPS 20

class FpsCounter
{
public:
	FpsCounter()
		:position(MAX_AVG_STEPS)
	{

	}
	~FpsCounter()
	{

	}
	void ComputeFPS()
	{
		auto ms = _sw.CheckMs();
		_sw.Reset();

		timeElapsed = ms.count() / 1000.0f;
		fps = 1.0f / timeElapsed;
		

		position++;
		
		if(position >= MAX_AVG_STEPS)
			position = 0;

		buffer[position] = fps;

		avgFps = 0.0f;
		for(int i = 0; i < MAX_AVG_STEPS; i++)
		{
			avgFps+=buffer[i];
		}
		avgFps /= (float)MAX_AVG_STEPS; 

	}
	float PreciseFPS()
	{
		return fps;
	}
	float AverageFPS()
	{
		return avgFps;
	}
	float GetTimeElapsed()
	{
		return timeElapsed;
	}
private:
	LFramework::Stopwatch _sw;
	float fps;
	float timeElapsed;
	float avgFps;
	float buffer[MAX_AVG_STEPS];
	int position;
};
#endif // FpsCounter_h__
