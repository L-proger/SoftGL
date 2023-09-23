#ifndef FpsCounter_h__
#define FpsCounter_h__

#include "stopwatch.h"

template<size_t AveragingFramesCount = 20>
class FpsCounter
{
public:
	typedef LFramework::Stopwatch::clock_t::duration duration_t;

	FpsCounter():position(0),avgFps(0) {
		_lastClock = _sw.Check();
		timeElapsed = timeElapsed.zero();
		for (size_t i = 0; i < AveragingFramesCount; ++i) {
			_buffer[i] = duration_t::zero();
		}
	}
	~FpsCounter()
	{

	}
	void ComputeFPS(){
		auto nowTime = _sw.Check();
		timeElapsed = nowTime - _lastClock;
		_lastClock = nowTime;

		_buffer[position] = timeElapsed;
		position = (position + 1) % AveragingFramesCount;

		duration_t sum = duration_t::zero();
		for (size_t i = 0; i < AveragingFramesCount; ++i) {
			sum += _buffer[i];
		}

		auto us = std::chrono::duration_cast<std::chrono::microseconds>(sum).count() / AveragingFramesCount;
		if (us == 0) {
			avgFps = 0.0f;
		} else {
			avgFps =1.0f / ( (float)us / 1000000.0f);
		}
	}
	float PreciseFPS() {
		float time = GetFrameTimeSeconds();
		if (time <= 0.0f) {
			return 0.0f;
		}
		return 1.0f / time;
	}
	float AverageFPS()
	{
		return avgFps;
	}
	float GetFrameTimeSeconds() const {
		return (float)std::chrono::duration_cast<std::chrono::microseconds>(timeElapsed).count() / 1000000.0f;
	}
	duration_t GetTimeElapsed() const {
		return timeElapsed;
	}
private:
	LFramework::Stopwatch _sw;
	duration_t _lastClock;
	duration_t timeElapsed;
	float avgFps;
	duration_t _buffer[AveragingFramesCount];
	int position;
};
#endif // FpsCounter_h__
