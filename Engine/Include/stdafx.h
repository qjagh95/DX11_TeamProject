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
#include <d2d1.h>
#include <dwrite.h>

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
#pragma warning(disable:4251)

using namespace std;

#include <d3d11.h>
#include <d3dcompiler.h> // shader를 컴파일하는 기능을 제공한다
#include <DirectXCollision.h>

#include "Types.h"

#include <mutex>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <atlstr.h>
#include <process.h>
#include <array>
#include <atlstr.h>


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

#include "Scene/Scene.h"
#include "Scene/Layer.h"

#include "GameObject.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "Component/Collider.h"
#include "Rendering/Shader.h"
#include "Resource/Mesh.h"
