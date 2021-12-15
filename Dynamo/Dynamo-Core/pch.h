#pragma once

#include <Windows.h>
#include <d3d11.h>

// Dear ImGui.
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_stdlib.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "ImGuizmo.h"
#include "imgui_node_editor.h"

namespace ImGui
{
	namespace NodeEditor = ax::NodeEditor;
}

// Standard Library
#include <vector>
#include <array>
#include <unordered_map>
#include <assert.h>
#include <string>
#include <unordered_set>
#include <utility>

#include "External/Json/json.hpp"

// CommonUtilities
#include <Vector.hpp>
#include <Matrix.hpp>
#include <ContainerIncludes.hpp>
#include <Quaternion.hpp>
#include <io.hpp>
#include <DArray.hpp>
#include "Types.h"

// ComponentSystem
#include <ComponentRegistry.h>
#include <ComponentAdmin.h>
#include <Component.h>
#include <ComponentDeclaration.hpp>
#include <GameObject.h>

#include "Components/Transform.h"

// Utils
#include "Utils/ConsoleUtils.h"
#include "Utils/Input.h"
#include "Utils/Time.h"
#include "Utils/Debug.h"
#include "Utils/ScopeTimer.h"

// Factories
#include "Factories/JsonFactory.h"
#include "Factories/ModelFactory.h"
#include "Factories/ResourceFactory.h"
#include "Factories/MaterialFactory.h"
#include "Factories/ShaderFactory.h"
#include "Factories/TextureFactory.h"

#include "Core/MainSingleton.h"

#include "Rendering/RenderManager.h"
#include "Scene/Scene.h"