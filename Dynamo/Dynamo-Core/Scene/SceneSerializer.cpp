#include "pch.h"
#include "SceneSerializer.h"
#include <fstream>

constexpr char version[] = "1.0";

namespace Dynamo
{
	void SceneSerializer::SaveScene(Scene* aScene, const std::string& aPath)
	{
		nlohmann::json finalJson;
		finalJson["Version"] = version;

		for (auto ob : aScene->GetAllGameObjects())
		{
			nlohmann::json objectJson;
			for (auto comp : ob->GetAllComponents())
			{
				nlohmann::json component;
				component["compType"] = comp->GetTypeID();
				component["data"] = comp->Save();

				objectJson["components"].push_back(component);
			}

			objectJson["name"] = ob->GetName();
			objectJson["tag"] = ob->GetTag();
			objectJson["isActive"] = ob->IsActive();

			finalJson["gameObjects"].push_back(objectJson);
		}

		std::ofstream stream(aPath);
		stream << finalJson.dump(4);
		stream.close();
	}

	void SceneSerializer::LoadScene(Scene* aScene, const std::string& aPath)
	{
		nlohmann::json json;
		std::ifstream stream(aPath);
		stream >> json;
		stream.close();

		DoJsonCheck(json);

		for (int i = 0; i < json["gameObjects"].size(); ++i)
		{
			nlohmann::json ob = json["gameObjects"][i];
			GameObject* object = aScene->CreateGameObject();
			object->SetName(ob["name"].get<std::string>());
			object->SetActive(ob["isActive"].get<bool>());
			object->SetTag(ob["tag"].get<Tag>());

			for (int j = 0; j < ob["components"].size(); ++j)
			{
				TypeID type = ob["components"][j]["compType"].get<std::string>();
				nlohmann::json data = ob["components"][j]["data"];

				Component* comp = aScene->GetComponentAdmin().AddComponentWithTypeID(type, object->GetGameObjectID());
				comp->Load(data);
			}
		}
	}

	void SceneSerializer::DoJsonCheck(nlohmann::json& aJson)
	{
		if (!aJson.contains("Version"))
		{
			Console::ErrorLog("Failed to parse scene. Invalid type.");
			return;
		}

		if (aJson["Version"] != version)
		{
			Console::ErrorLog("Outdated scene version.");
			return;
		}
	}
}