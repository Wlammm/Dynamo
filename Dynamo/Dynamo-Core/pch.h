#pragma once

#include <Windows.h>
#include <d3d11.h>

// Standard Library
#include <vector>
#include <array>
#include <unordered_map>
#include <assert.h>
#include <string>
#include <memory>

#include "External/Json/json.hpp"

// CommonUtilities
#include <Vector.hpp>
#include <Matrix.hpp>
#include <ContainerIncludes.hpp>
#include <Quaternion.hpp>
#include <io.hpp>
#include <DArray.hpp>

#include <ComponentAdmin.h>
#include <Component.h>
#include <GameObject.h>

#include "Types.h"

// Factories.
#include "Factories/JsonFactory.h"
#include "Factories/ModelFactory.h"
#include "Factories/ResourceFactory.h"
#include "Factories/MaterialFactory.h"
#include "Factories/ShaderFactory.h"
#include "Factories/TextureFactory.h"

#include "Core/MainSingleton.h"