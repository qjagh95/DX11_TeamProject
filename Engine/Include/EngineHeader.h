#pragma once

#include <Windows.h>
#include <list>
#include <vector>
#include <unordered_map>
#include <string>
#include <crtdbg.h>
#include <stack>
#include <algorithm>
#include <conio.h>
#include <assert.h>
#include <array>
#include <thread>
#include <mutex>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <atlstr.h>
#include <process.h>
#include <memory>
#include <memory.h>
#include <time.h>

#include <d3d11.h>
#include <d3dcompiler.h> // shader를 컴파일하는 기능을 제공한다
#include <DirectXCollision.h>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

#include <d2d1.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
#pragma warning(disable:4251)

#include <dinput.h>

#pragma comment(lib , "dinput8")
#pragma comment(lib, "dxguid")

using namespace std;

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <Audio.h>

#include <MathHeader.h>

#include "Macro.h"
#include "Flag.h"
#include "Types.h"

#include "PathManager.h"
#include "Resource/ResourcesManager.h"
#include "Scene/SceneManager.h"
#include "Rendering/RenderManager.h"
#include "Rendering/ShaderManager.h"
#include "NavigationManager.h"
#include "TimerManager.h"
#include "StageManager.h"
#include "CollisionManager.h"
#include "SoundManager.h"
#include "ObjectManager.h"
#include "NavigationManager.h"
#include "FontManager.h"
#include "GUIManager.h"
#include "SoundManager.h"
#include "NavigationManager3D.h"
#include "Input.h"

#include "Scene/Scene.h"
#include "Scene/Layer.h"

#include "GameObject.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "Component/Collider.h"
#include "Resource/Mesh.h"

#include "BineryWriter.h"
#include "BineryReader.h"
