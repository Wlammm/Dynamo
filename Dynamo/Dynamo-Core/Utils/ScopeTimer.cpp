#include "pch.h"
#include "ScopeTimer.h"

namespace Dynamo
{
	ScopeTimer::ScopeTimer(const std::string& aName, bool aShouldPrint)
	{
		myName = aName;
		myShouldPrint = aShouldPrint;
		myStartTime = myTimer.now();
	}

	ScopeTimer::~ScopeTimer()
	{
		float time = (myTimer.now() - myStartTime).count() / 1000000.0f;

		if (myShouldPrint)
		{
			Console::Log("Timer - %s: %f ms", myName.c_str(), time);
		}
	}
}