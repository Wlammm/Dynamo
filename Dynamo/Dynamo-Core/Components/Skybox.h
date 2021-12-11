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

		void Update() override;

	private:
		MeshRenderer* mySphere = nullptr;
		Transform* myTransform = nullptr;
	};
}