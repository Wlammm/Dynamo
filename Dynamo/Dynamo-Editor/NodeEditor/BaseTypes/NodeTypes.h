#pragma once

namespace Editor
{
	enum class PinType
	{
		In,
		Out
	};

	enum class PinData
	{
		Bool,
		Int,
		Float,
		String,

		None = 254, 
		Unknown = 255,
	};

	struct PinDataColor
	{
		static ImColor GetColor(const PinData aType)
		{
			switch (aType)
			{
			case PinData::Bool:			return ImColor(220, 48, 48);
			case PinData::Int:			return ImColor(68, 201, 156);
			case PinData::Float:		return ImColor(147, 226, 74);
			case PinData::String:		return ImColor(124, 21, 153);

			case PinData::None:			return ImColor(255, 255, 255);
			case PinData::Unknown:		return ImColor(255, 0, 0);

			default:					return ImColor(100, 100, 100);
			}
		}
	};

	struct Payload
	{

	};
}

