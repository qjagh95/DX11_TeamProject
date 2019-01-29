#pragma once

#include "Macro.h"

// DirectX11 ���� �����ϴ� ���� ����� Ȱ���Ѵ�
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

#define PUN_PI	3.141592f

float RadianToDegree(float fRadian);
float DegreeToRadian(float fDegree);