#pragma once

namespace Editor
{
	class Selection : public EditorSystem
	{
	public:
		Selection();

		void Update() override;

	private:
		void SelectionCallback();
		uint GetIDAtPos(const Vec2ui aMousePos);

	private:
		ID3D11Texture2D* myTexture = nullptr;
	};
}