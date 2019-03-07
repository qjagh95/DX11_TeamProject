#pragma once

#include "EngineHeader.h"
#include <stack>
#include <algorithm>
using namespace std;

PUN_BEGIN

class PUN_DLL CNavigaionManager3D
{
private:
	unordered_map<class CScene*, vector<class CNavigationMesh*>> m_mapNavMesh;

public:
	bool Init();
	class CNavigationMesh* CreateNavMesh(class CScene* pScene);
	class CNavigationMesh* FindNavMesh(const string& strName);
	class CNavigationMesh* FindNavMesh(const Vector3& vPos);

private:
	vector<class CNavigationMesh*>* FindNavMeshList(class CScene* pScene);

	DECLARE_SINGLE(CNavigaionManager3D)
};

PUN_END