#pragma once

#include <Windows.h>
#include <list>
#include <vector>
#include <unordered_map>
#include <string>
#include <crtdbg.h>
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

#include <d3d11.h>
#include <d3dcompiler.h> // shader를 컴파일하는 기능을 제공한다
#include <DirectXCollision.h>

#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "ImGui")

#include <d2d1.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
#pragma warning(disable:4251)

#include <MathHeader.h>
#include <Vector3.h>
#include <Vector4.h>
#include <Vector2.h>
#include <Matrix.h>

using namespace std;

#include "Types.h"
#include "Engine.h"

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

//#include <Audio.h>

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
#include "SoundManagerT.h"

#include "Scene/Scene.h"
#include "Scene/Layer.h"

#include "GameObject.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "Component/Collider.h"
#include "Resource/Mesh.h"
