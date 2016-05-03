#ifndef Timer_h__
#define Timer_h__


#define TIMER_QPC

#ifndef TIMER_QPC
#define TIMER_TGT
#endif


	class Timer
	{
	private:
#ifdef TIMER_TGT
		static int refCount = 0;
		DWORD lastTime, curTime, diff;
#else
		__int64 lastTime, curTime, diff, perfFreq;
#endif
		float timeElapsed;
	public:

		Timer()
			:timeElapsed(0)
		{
#ifdef TIMER_TGT
			if(refCount == 0)
				timeBeginPeriod(1);
			refCount++;
#else
			QueryPerformanceFrequency((LARGE_INTEGER *)&perfFreq);
#endif
		}
		~Timer()
		{
#ifdef TIMER_TGT
			refCount--;
			if(refCount == 0)
				timeEndPeriod(1);
#endif
		}
		void Start()
		{
#ifdef TIMER_TGT
			lastTime = timeGetTime();
#else
			QueryPerformanceCounter((LARGE_INTEGER *)&lastTime);
#endif

		}
		void Stop()
		{
#ifdef TIMER_TGT
			curTime = timeGetTime();
			diff = curTime - lastTime;
			timeElapsed = 1.0d / (double)diff; //time in SECONDS
#else
			QueryPerformanceCounter((LARGE_INTEGER *)&curTime);
			if(lastTime == 0)
				timeElapsed = 0;
			else
			{
				timeElapsed = (float)((double)(curTime - lastTime) / (double)perfFreq);
			}

#endif
			if(timeElapsed < 0.0000001f)
			{
				timeElapsed = 0.0000001f;
			}
		}
		float getTimeElapsed()
		{
			return timeElapsed;
		}
	};


#endif // Timer_h__