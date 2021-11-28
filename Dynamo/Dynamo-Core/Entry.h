#pragma once

#include <functional>

namespace Dynamo
{
	class Entry
	{
	public:
		static void Init(std::function<void(void)> aOutsideUpdate);
		static void Run();
		
	private:
		inline static std::function<void(void)> myOutsideUpdate = nullptr;
	};
}