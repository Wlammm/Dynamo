#pragma once

namespace Dynamo
{
	class Debug
	{
	public:
		static void ImGui(const std::string& aName, const std::function<void(void)>& aFunction);
		static void Update();

	private:
		static std::vector<std::pair<std::string, std::function<void(void)>>> myImGuiDraws;
		static bool isOpen;
	};
}