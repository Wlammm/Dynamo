#pragma once

#include "pch.h"

#include "Core/EditorMainSingleton.h"
#include "Managers/EditorManager.h"
#include "PostMaster/EditorPostMaster.h"
#include "Core/EditorWindow.h"
#include "Core/EditorSystem.h"

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

using Main = Editor::MainSingleton;
using Time = Dynamo::Time;
using Input = Dynamo::Input;
using Console = Dynamo::Console;