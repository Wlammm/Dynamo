#pragma once

namespace Dynamo
{
	class MeshRenderer;

	class Skybox : public Component
	{
		COMP(Skybox);
	public:
		Skybox();
		~Skybox();

		void OnCreate() override;

		void Update() override;
		void EditorUpdate() override;

	private:
		MeshRenderer* mySphere = nullptr;
		Transform* myTransform = nullptr;
	};
}