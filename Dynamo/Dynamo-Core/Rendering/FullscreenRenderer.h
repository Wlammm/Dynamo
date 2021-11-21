#pragma once

namespace Dynamo
{
	class FullscreenRenderer
	{
	public:
		FullscreenRenderer();

		void Render(Shader* aPixelShader);
		void RenderCopy();

	private:
		Shader* myVertexShader = nullptr;
		Shader* myCopyShader = nullptr;
	};
}