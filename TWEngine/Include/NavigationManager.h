#pragma once

#include "Engine.h"
#include <stack>
#include <algorithm>

using namespace std;

PUN_BEGIN

class PUN_DLL CNavigationManager
{
private:
	list<class CStage2D*>	m_StageList;
	vector<class CTile2D*>	m_OpenList;
	stack<int>				m_FindStack;
	list<Vector3>			m_PathList;
	CStage2D*				m_pStage;
	float	m_fTileWidth;
	float	m_fTileHeight;

public:
	void AddStage2D(class CStage2D* pStage);
	void ClearStage2D();
	void ClearStage2D(class CScene* pScene);
	bool Find2D(const Vector3& vStart, const Vector3& vEnd);
	list<Vector3>	GetPathList()	const;

private:
	bool AddOpenList(class CStage2D* pStage, class CTile2D* pNode,
		const Vector3& vEnd, class CTile2D* pEndTile);
	float	GetG(ASTAR_NODE_DIR eDir);

public:
	bool Init();

private:
	static bool SortOpenList(class CTile2D* pSrc, class CTile2D* pDest);

	DECLARE_SINGLE(CNavigationManager)
};

PUN_END