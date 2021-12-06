#pragma once

namespace Dynamo
{
	class Scene;

	class SceneSerializer
	{
	public:
		static void SaveScene(Scene* aScene, const std::string& aPath);
		static void LoadScene(Scene* aScene, const std::string& aPath);

	private:
		static void DoJsonCheck(nlohmann::json& aJson);

	};
}