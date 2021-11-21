#pragma once
#include "Timer.h"

namespace Dynamo
{
	class Time
	{
	public:
		static void Create();
		static void Destroy();
		static void Update();

		// Starts a timer that will execute a callback event after aTimer has run out
		static void StartCallbackEvent(const float aTimer, const std::function<void(void)> aCallbackFunction);

		// Starts a timer that will execute a callback event every frame until aTimer has run out
		static void StartUpdateCallbackEvent(const float aTimer, const std::function<void(void)> aCallbackFunction);

		static void SetTimeScale(const float aTimeScale);
		static float GetTimeScale();

		static float GetDeltaTime();
		static float GetUnscaledDeltaTime();
		static float GetTotalTime();

		/*
		static float DeltaTime;
		static float UnscaledDeltaTime;
		static double TotalTime;
		*/

	private:
		void CreateInternal();

	private:
		static Time* myInstance;

		std::unique_ptr<CU::Timer> myTimer;
	};

	/*inline float Time::DeltaTime = 0.0f;
	inline float Time::UnscaledDeltaTime = 0.0f;
	inline double Time::TotalTime = 0.0f;*/
}
