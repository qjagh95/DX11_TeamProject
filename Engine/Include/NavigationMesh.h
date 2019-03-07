#pragma once

#include "EngineHeader.h"

PUN_BEGIN

enum NAVIMESH_CELL_LIST_TYPE
{
	NCLT_NONE,
	NCLT_OPEN,
	NCLT_CLOSE
};

typedef struct PUN_DLL AdjInfo
{
	int	iIndex;
	int iEdgeIndex;
}ADJINFO, *PADJINFO;

typedef struct PUN_DLL _tagNavigationCell
{
	NAVIMESH_CELL_LIST_TYPE	eType;
	Vector3		vPos[3];
	Vector3		vEdge[3];
	Vector3		vEdgeCenter[3];
	Vector3		vCenter;
	vector<ADJINFO>	vecAdj;
	int			iIndex;
	int			iParentIdx;
	float		fG;
	float		fH;
	float		fTotal;
	bool		bEnable;

	_tagNavigationCell()
	{
		eType = NCLT_NONE;
		iParentIdx = -1;
		iIndex = -1;
		fG = -1.f;
		fH = -1.f;
		fTotal = -1.f;
		bEnable = true;
	}
}NavigationCell, *PNavigationCell;


typedef struct PUN_DLL _tagNavSection
{
	Vector3		vSize;
	PNavigationCell*	pCellList;
	int			iSize;
	int			iCapacity;
	Vector3		vMin;
	Vector3		vMax;

	void Resize()
	{
		if (iSize == iCapacity)
		{
			iCapacity *= 2;
			PNavigationCell*	pList = new PNavigationCell[iCapacity];

			memset(pList, 0, sizeof(PNavigationCell) * iCapacity);

			memcpy(pList, pCellList, sizeof(PNavigationCell) * iSize);

			SAFE_DELETE_ARRAY(pCellList);
			pCellList = pList;
		}
	}

	void Add(PNavigationCell pCell)
	{
		Resize();
		pCellList[iSize] = pCell;
		++iSize;
	}

	_tagNavSection()
	{
		iSize = 0;
		iCapacity = 2048;
		pCellList = new PNavigationCell[iCapacity];

		memset(pCellList, 0, sizeof(PNavigationCell) * iCapacity);
	}

	~_tagNavSection()
	{
		SAFE_DELETE_ARRAY(pCellList);
	}
}NavSection, *PNavSection;

class PUN_DLL CNavigationMesh
{
private:
	friend class CNavigationManager3D;

private:
	CNavigationMesh();

public:
	~CNavigationMesh();

private:
	string		m_strName;
	vector<PNavigationCell>	m_vecCell;
	Vector3		m_vOffset;
	Vector3		m_vOffsetScale;
	Vector3		m_vMin;
	Vector3		m_vMax;
	int			m_iLineRectCount;
	bool		m_bGrid;
	int			m_iSectionX;
	int			m_iSectionZ;
	Vector3		m_vSectionSize;
	PNavSection	m_pSection;

private:
	vector<PNavigationCell>	m_vecOpen;
	stack<int>				m_FindStack;
	list<Vector3>			m_PathList;
	bool					m_bFindEnd;

public:
	Vector3	GetMin()	const;
	Vector3 GetMax()	const;
	Vector3 GetOffset()	const;

public:
	void SetOffset(const Vector3& vOffset);
	void SetOffsetScale(const Vector3& vOffsetScale);

public:
	bool Init();
	void AddCell(const Vector3 vPos[3]);
	void AddAdj(int iCellIdx, int iAdjIdx);
	void CreateGridMapAdj(int iLineRectCount);
	void CreateAdj();
	bool CheckOnEdge(int iSrc, int iDest, const Vector3& vOrigin1, const Vector3& vOrigin2,
		const Vector3& vEdge, float fEdgeLength, int iEdge1, int iEdge2);

public:
	float GetY(const Vector3 vPos);
	bool CheckCell(const Vector3& vPos);
	float GetY(int iCellIndex, const Vector3& vPos);
	void CreateSection();

private:
	int GetCellIndex(const Vector3& vPos);
	bool RayIntersectTriangle(Vector3 rayOrigin, Vector3 rayDir,
		Vector3 v0, Vector3 v1, Vector3 v2,
		float& fDist, Vector3& vIntersect);
	int GetSectionIndex(Vector3 vPos);

public:
	void Save(const char* pFileName, const string& strPathKey = DATA_PATH);
	void SaveFromFullPath(const char* pFullPath);
	void Load(const char* pFileName, const string& strPathKey = DATA_PATH);
	void LoadFromFullPath(const char* pFullPath);
};

PUN_END