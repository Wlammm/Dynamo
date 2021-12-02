#pragma once

constexpr size_t MAX_COMPONENTS_OF_TYPE = 1000;
constexpr size_t MAX_GAMEOBJECTS = 50000;

using GameObjectID = size_t;
using ComponentType = std::uint8_t;

using TypeID = std::string;
using ComponentName = std::string;