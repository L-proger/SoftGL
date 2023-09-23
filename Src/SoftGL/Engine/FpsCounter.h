#pragma once

#include <chrono>


template<size_t AveragingFramesCount = 20>
class FpsCounter
{
public:

	using Clock = std::chrono::steady_clock;
	using Duration = std::chrono::steady_clock::duration;
	using TimePoint = std::chrono::steady_clock::time_point;

	FpsCounter():position(0),avgFps(0) {
		_lastClock = Clock::now();
		timeElapsed = timeElapsed.zero();
		for (size_t i = 0; i < AveragingFramesCount; ++i) {
			_buffer[i] = {};
		}
	}
	~FpsCounter()
	{

	}
	void ComputeFPS(){
		auto nowTime = Clock::now();
		timeElapsed = nowTime - _lastClock;
		_lastClock = nowTime;

		_buffer[position] = timeElapsed;
		position = (position + 1) % AveragingFramesCount;

		Duration sum = {};
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
	Duration GetTimeElapsed() const {
		return timeElapsed;
	}
private:
	TimePoint _lastClock;
	Duration timeElapsed;
	float avgFps;
	Duration _buffer[AveragingFramesCount];
	int position;
};
