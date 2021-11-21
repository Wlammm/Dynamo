#include "Timer.h"

namespace CommonUtilities
{
	Timer::Timer()
	{
		myUpdateStart = myTimer.now();
		myDeltaTime = 0.0f;
		myTimeScale = 1.0f;
		myTotalTime = 0.0f;
	}

	void Timer::Update()
	{
		myDeltaTime = (myTimer.now() - myUpdateStart).count() / 1000000000.0f;
		myUpdateStart = myTimer.now();
		myTotalTime += myDeltaTime;

		UpdateStopTimers();
	}

	float Timer::GetDeltaTime() const
	{
		return myDeltaTime * myTimeScale;
	}

	float Timer::GetUnscaledDeltaTime() const
	{
		return myDeltaTime;
	}

	float Timer::GetTotalTime() const
	{
		return myTotalTime;
	}

	void Timer::SetTimeScale(const float aScaleValue)
	{
		myTimeScale = aScaleValue;
	}

	float Timer::GetTimeScale() const
	{
		return myTimeScale;
	}

	void Timer::ResetDeltaTime()
	{
		myDeltaTime = 0;
		myUpdateStart = myTimer.now();
	}

	void Timer::SetTimer(const float aTime, const std::function<void(void)>& aCallbackFunction, const bool isRepeatable)
	{
		StopTimer newTimer;
		newTimer.myTimer = aTime;
		newTimer.myStartTime = aTime;
		newTimer.myIsRepeatable = isRepeatable;
		newTimer.myCallback = aCallbackFunction;
		myStopTimers.push_back(newTimer);
	}

	void Timer::SetUpdateTimer(const float aTime, const std::function<void(void)>& aCallbackFunction, const bool isRepeatable)
	{
		StopTimer newTimer;
		newTimer.myTimer = aTime;
		newTimer.myStartTime = aTime;
		newTimer.myIsRepeatable = isRepeatable;
		newTimer.myCallback = aCallbackFunction;
		myUpdateTimers.push_back(newTimer);
	}

	void Timer::ClearAllTimers()
	{
		myStopTimers.clear();
	}

	void Timer::UpdateStopTimers()
	{
		{
			const int size = static_cast<int>(myStopTimers.size());
			if (size > 0)
			{
				for (int i = (size - 1); i >= 0; --i)
				{
					myStopTimers[i].myTimer -= myDeltaTime;
					if (myStopTimers[i].myTimer < 0.0f)
					{
						myStopTimers[i].myCallback();
						if (myStopTimers[i].myIsRepeatable == true)
						{
							myStopTimers[i].myTimer = myStopTimers[i].myStartTime;
						}
						else
						{
							myStopTimers.erase(myStopTimers.begin() + i);
						}
					}
				}
			}
		}

		{
			const int size = static_cast<int>(myUpdateTimers.size());
			if (size > 0)
			{
				for (int i = (size - 1); i >= 0; --i)
				{
					myUpdateTimers[i].myCallback();
					myUpdateTimers[i].myTimer -= myDeltaTime;
					if (myUpdateTimers[i].myTimer < 0.0f)
					{
						if (myUpdateTimers[i].myIsRepeatable == true)
						{
							myUpdateTimers[i].myTimer = myUpdateTimers[i].myStartTime;
						}
						else
						{
							myUpdateTimers.erase(myUpdateTimers.begin() + i);
						}
					}
				}
			}
		}
	}
}
