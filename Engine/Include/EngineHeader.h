#pragma once

#include <list>
#include <ppl.h>
#include <stack>
#include <array>
#include <mutex>
#include <time.h>
#include <vector>
#include <string>
#include <thread>
#include <memory>
#include <conio.h>
#include <fstream>
#include <sstream>
#include <memory.h>
#include <crtdbg.h>
#include <assert.h>
#include <iostream>
#include <atlstr.h>
#include <algorithm>
#include <process.h>
#include <Windows.h>
#include <unordered_map>

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
using namespace concurrency;

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <Audio.h>

#include <MathHeader.h>

#include "Macro.h"
#include "Flag.h"
#include "Types.h"

#include "Input.h"
#include "GUIManager.h"
#include "PathManager.h"
#include "FontManager.h"
#include "SoundManager.h"
#include "TimerManager.h"
#include "SoundManager.h"
#include "ObjectManager.h"
#include "CollisionManager.h"
#include "Scene/SceneManager.h"
#include "NavigationManager3D.h"
#include "Rendering/RenderManager.h"
#include "Rendering/ShaderManager.h"
#include "Resource/ResourcesManager.h"

#include "Scene/Scene.h"
#include "Scene/Layer.h"

#include "GameObject.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Collider.h"
#include "Component/Transform.h"
#include "Component/Animation.h"

#include "Resource/Mesh.h"

#include "BinaryWriter.h"
#include "BinaryReader.h"
