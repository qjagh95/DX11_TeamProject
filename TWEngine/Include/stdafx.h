#pragma once

#include <Windows.h>
#include <list>
#include <vector>
#include <unordered_map>
#include <string>
#include <crtdbg.h>
#include <conio.h>
#include <assert.h>
#include <functional>
#include <algorithm>
#include <process.h>
#include <d2d1.h>
#include <dwrite.h>

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

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
