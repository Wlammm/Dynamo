#pragma once

namespace Editor
{
	class EditorSystem
	{
	public:
		virtual void Update() = 0;

		virtual void Init();
	};
}