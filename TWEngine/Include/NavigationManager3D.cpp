#include "NavigationManager3D.h"
#include "NavigationMesh.h"
#include "Component/LandScape.h"
#include "Component/Transform.h"

PUN_USING

DEFINITION_SINGLE(CNavigationManager3D)

CNavigationManager3D::CNavigationManager3D()
{
}

CNavigationManager3D::~CNavigationManager3D()
{
	unordered_map<class CScene*, vector<class CNavigationMesh*>>::iterator	iter;
	unordered_map<class CScene*, vector<class CNavigationMesh*>>::iterator	iterEnd = m_mapNavMesh.end();

	for (iter = m_mapNavMesh.begin(); iter != iterEnd; ++iter)
	{
		Safe_Delete_VecList(iter->second);
	}
}

bool CNavigationManager3D::Init()
{
	return true;
}

CNavigationMesh * CNavigationManager3D::CreateNavMesh(CScene * pScene)
{
	vector<CNavigationMesh*>*	pList = FindNavMeshList(pScene);

	if (!pList)
	{
		vector<CNavigationMesh*> vec;
		m_mapNavMesh.insert(make_pair(pScene, vec));
		
		auto StartIter = m_mapNavMesh.begin();
		auto EndIter = m_mapNavMesh.end();

		for (; StartIter != EndIter; StartIter++)
			pList = &StartIter->second;
	}

	CNavigationMesh*	pMesh = new CNavigationMesh;
	pMesh->Init();
	
	pList->push_back(pMesh);
	return pMesh;
}

CNavigationMesh * CNavigationManager3D::CreateNavMesh(CScene * pScene, const char * pFileName, const string & strPathName)
{
	vector<CNavigationMesh*>*	pList = FindNavMeshList(pScene);

	if (!pList)
	{
		vector<CNavigationMesh*> vec;
		m_mapNavMesh.insert(make_pair(pScene, vec));

		auto StartIter = m_mapNavMesh.begin();
		auto EndIter = m_mapNavMesh.end();

		for (; StartIter != EndIter; StartIter++)
			pList = &StartIter->second;
	}

	CNavigationMesh*	pMesh = new CNavigationMesh;
	pMesh->Init();

	pList->push_back(pMesh);
	return pMesh;
}

CNavigationMesh * CNavigationManager3D::FindNavMesh(class CScene* pScene, const string & strName)
{
	unordered_map<class CScene*, vector<class CNavigationMesh*>>::iterator	iter =
		m_mapNavMesh.find(pScene);

	if (iter == m_mapNavMesh.end())
		return nullptr;

	for (size_t i = 0; i < iter->second.size(); ++i)
	{
		CNavigationMesh*	pNavMesh = iter->second[i];

		if (pNavMesh->m_strName == strName)
			return pNavMesh;
	}

	return nullptr;
}

CNavigationMesh * CNavigationManager3D::FindNavMesh(class CScene* pScene, const Vector3 & vPos)
{
	unordered_map<class CScene*, vector<class CNavigationMesh*>>::iterator	iter =
		m_mapNavMesh.find(pScene);

	if (iter == m_mapNavMesh.end())
		return nullptr;

	for (size_t i = 0; i < iter->second.size(); ++i)
	{
		CNavigationMesh*	pNavMesh = iter->second[i];

		Vector3 vMin = pNavMesh->GetMin();
		Vector3 vMax = pNavMesh->GetMax();
		//Vector3 ConvertPos = vPos + pNavMesh->GetOffset();
		Vector3 ConvertPos = vPos + Vector3(-100.f, 0.0f, 0.0f);

		if (vMin.x <= vPos.x && vPos.x <= vMax.x &&
			vMin.y <= vPos.y && vPos.y <= vMax.y &&
			vMin.z <= vPos.z && vPos.z <= vMax.z)
		{
			return pNavMesh;
		}
	}

	return nullptr;
}

vector<CNavigationMesh*>* CNavigationManager3D::FindNavMeshList(CScene * pScene)
{
	unordered_map<class CScene*, vector<CNavigationMesh*>>::iterator	iter =
		m_mapNavMesh.find(pScene);

	if (iter == m_mapNavMesh.end())
		return nullptr;

	return &iter->second;
}
