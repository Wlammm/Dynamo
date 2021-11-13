#include "pch.h"
#include "Timer.h"

CommonUtilities::Timer::Timer()
{
    myUpdateStart = myClock.now();
}

void CommonUtilities::Timer::Update()
{
    myDeltaTime = (myClock.now() - myUpdateStart).count() / 1000000000.0f;
    myUpdateStart = myClock.now();
    myTotalTime += myDeltaTime;
}

float CommonUtilities::Timer::GetDeltaTime() const
{
    return myDeltaTime;
}

double CommonUtilities::Timer::GetTotalTime() const
{
    return myTotalTime;
}
