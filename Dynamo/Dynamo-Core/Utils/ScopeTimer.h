#pragma once

/*
*	Usage example : SCOPE_TIMER("SceneUpdate", true);
*/

#define SCOPE_TIMER(aName, aShouldPrint) auto scopeTimerVar = ScopeTimer(aName, aShouldPrint)
namespace Dynamo
{
	class ScopeTimer
	{
	public:
		ScopeTimer(const std::string& aName, bool aShouldPrint = false);
		~ScopeTimer();

	private:
		std::chrono::high_resolution_clock myTimer;
		std::chrono::time_point<std::chrono::high_resolution_clock> myStartTime;
		std::string myName;
		bool myShouldPrint = false;
	};
}