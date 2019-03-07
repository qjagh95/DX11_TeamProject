#pragma once

#include "EngineHeader.h"
#include <stack>
#include <algorithm>

using namespace std;

PUN_BEGIN

class PUN_DLL CNavigationManager3D
{
private:
	unordered_map<class CScene*, vector<class CNavigationMesh*>>	m_mapNavMesh;

public:
	bool Init();
	class CNavigationMesh* CreateNavMesh(class CScene* pScene);
	class CNavigationMesh* CreateNavMesh(class CScene* pScene,
		const char* pFileName, const string& strPathName = DATA_PATH);
	class CNavigationMesh* FindNavMesh(class CScene* pScene, const string& strName);
	class CNavigationMesh* FindNavMesh(class CScene* pScene, const Vector3& vPos);

private:
	vector<class CNavigationMesh*>* FindNavMeshList(class CScene* pScene);

	DECLARE_SINGLE(CNavigationManager3D)
};

PUN_END