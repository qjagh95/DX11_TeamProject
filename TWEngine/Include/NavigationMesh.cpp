#include "NavigationMesh.h"
#include "PathManager.h"

PUN_USING

CNavigationMesh::CNavigationMesh()
{
	m_bGrid = false;
	m_iLineRectCount = 0;
	m_iSectionX = 1;
	m_iSectionZ = 1;
	m_pSection = new NavSection[m_iSectionX * m_iSectionZ];

	m_iCloseListSize = 0;
	m_pCloseList = nullptr;

	m_OpenList.SetSortFunc(this, &CNavigationMesh::OpenListSort);
}

CNavigationMesh::~CNavigationMesh()
{
	SAFE_DELETE_ARRAY(m_pCloseList);
	SAFE_DELETE_ARRAY(m_pSection);
	Safe_Delete_VecList(m_vecCell);
}

Vector3 CNavigationMesh::GetMin() const
{
	return m_vMin;
}

Vector3 CNavigationMesh::GetMax() const
{
	return m_vMax;
}

Vector3 CNavigationMesh::GetOffset() const
{
	return m_vOffset;
}

list<Vector3> CNavigationMesh::GetPathList() const
{
	return m_PathList;
}

void CNavigationMesh::SetOffset(const Vector3 & vOffset)
{
	m_vOffset = vOffset;
}

void CNavigationMesh::SetOffsetScale(const Vector3 & vOffsetScale)
{
	m_vOffsetScale = vOffsetScale;
}

bool CNavigationMesh::Init()
{
	m_vMin.x = 10000000.f;
	m_vMin.y = 10000000.f;
	m_vMin.z = 10000000.f;

	m_vMax.x = -10000000.f;
	m_vMax.y = -10000000.f;
	m_vMax.z = -10000000.f;

	return true;
}

void CNavigationMesh::AddCell(std::vector<Vertex3DColor>* vecColor, int _iPos0, int vPos1, int vPos2)
{
	PNavigationCell	pCell = new NavigationCell;
	pCell->m_iVertexIndex[0] = _iPos0;
	pCell->m_iVertexIndex[1] = vPos1;
	pCell->m_iVertexIndex[2] = vPos2;

	pCell->vPos[0] = (*vecColor)[_iPos0].vPos;
	pCell->vPos[1] = (*vecColor)[vPos1].vPos;
	pCell->vPos[2] = (*vecColor)[vPos2].vPos;

	pCell->tCopyVertex[0] = &(*vecColor)[_iPos0];
	pCell->tCopyVertex[1] = &(*vecColor)[vPos1];
	pCell->tCopyVertex[2] = &(*vecColor)[vPos2];

	pCell->vEdge[0] = (*vecColor)[vPos1].vPos - (*vecColor)[_iPos0].vPos;
	pCell->vEdgeCenter[0] = ((*vecColor)[vPos1].vPos + (*vecColor)[_iPos0].vPos) / 2.f;

	pCell->vEdge[1] = (*vecColor)[vPos2].vPos - (*vecColor)[vPos1].vPos;
	pCell->vEdgeCenter[1] = ((*vecColor)[vPos2].vPos + (*vecColor)[vPos1].vPos) / 2.f;

	pCell->vEdge[2] = (*vecColor)[vPos2].vPos - (*vecColor)[_iPos0].vPos;
	pCell->vEdgeCenter[2] = ((*vecColor)[vPos2].vPos + (*vecColor)[_iPos0].vPos) / 2.f;

	pCell->vCenter = ((*vecColor)[_iPos0].vPos + (*vecColor)[vPos1].vPos + (*vecColor)[vPos2].vPos) / 3.f;

	pCell->iIndex = (int)m_vecCell.size();

	if (m_vMin.x > (*vecColor)[_iPos0].vPos.x)
		m_vMin.x = (*vecColor)[_iPos0].vPos.x;
	if (m_vMin.y > (*vecColor)[_iPos0].vPos.y)
		m_vMin.y = (*vecColor)[_iPos0].vPos.y;
	if (m_vMin.z > (*vecColor)[_iPos0].vPos.z)
		m_vMin.z = (*vecColor)[_iPos0].vPos.z;

	if (m_vMax.x < (*vecColor)[_iPos0].vPos.x)
		m_vMax.x = (*vecColor)[_iPos0].vPos.x;
	if (m_vMax.y < (*vecColor)[_iPos0].vPos.y)
		m_vMax.y = (*vecColor)[_iPos0].vPos.y;
	if (m_vMax.z < (*vecColor)[_iPos0].vPos.z)
		m_vMax.z = (*vecColor)[_iPos0].vPos.z;

	if (m_vMin.x > (*vecColor)[vPos1].vPos.x)
		m_vMin.x = (*vecColor)[vPos1].vPos.x;
	if (m_vMin.y > (*vecColor)[vPos1].vPos.y)
		m_vMin.y = (*vecColor)[vPos1].vPos.y;
	if (m_vMin.z > (*vecColor)[vPos1].vPos.z)
		m_vMin.z = (*vecColor)[vPos1].vPos.z;

	if (m_vMax.x < (*vecColor)[vPos1].vPos.x)
		m_vMax.x = (*vecColor)[vPos1].vPos.x;
	if (m_vMax.y < (*vecColor)[vPos1].vPos.y)
		m_vMax.y = (*vecColor)[vPos1].vPos.y;
	if (m_vMax.z < (*vecColor)[vPos1].vPos.z)
		m_vMax.z = (*vecColor)[vPos1].vPos.z;

	if (m_vMin.x > (*vecColor)[vPos2].vPos.x)
		m_vMin.x = (*vecColor)[vPos2].vPos.x;
	if (m_vMin.y > (*vecColor)[vPos2].vPos.y)
		m_vMin.y = (*vecColor)[vPos2].vPos.y;
	if (m_vMin.z > (*vecColor)[vPos2].vPos.z)
		m_vMin.z = (*vecColor)[vPos2].vPos.z;

	if (m_vMax.x < (*vecColor)[vPos2].vPos.x)
		m_vMax.x = (*vecColor)[vPos2].vPos.x;
	if (m_vMax.y < (*vecColor)[vPos2].vPos.y)
		m_vMax.y = (*vecColor)[vPos2].vPos.y;
	if (m_vMax.z < (*vecColor)[vPos2].vPos.z)
		m_vMax.z = (*vecColor)[vPos2].vPos.z;

	Vector3	vDir1 = pCell->vEdge[0];
	Vector3	vDir2 = pCell->vEdge[2];

	vDir1.Normalize();
	vDir2.Normalize();

	Vector3	vFaceNormal = vDir1.Cross(vDir2);
	vFaceNormal.Normalize();

	Vector3	vView = Vector3::Axis[AXIS_Y];

	float	fAngle = vView.Angle(vFaceNormal);

	if (fAngle >= 65.f)
		pCell->bEnable = false;

	m_vMax.y = 2.0f;

	m_vecCell.push_back(pCell);
}

void CNavigationMesh::LoadCell(std::vector<Vertex3DColor>* vecColor)
{
	for (size_t i = 0; i < m_vecCell.size(); ++i)
	{
		m_vecCell[i]->tCopyVertex[0] = &(*vecColor)[m_vecCell[i]->m_iVertexIndex[0]];
		m_vecCell[i]->tCopyVertex[1] = &(*vecColor)[m_vecCell[i]->m_iVertexIndex[1]];
		m_vecCell[i]->tCopyVertex[2] = &(*vecColor)[m_vecCell[i]->m_iVertexIndex[2]];
	}
}

void CNavigationMesh::SetCellCopyVertex(Vertex3DColor & vPos0, Vertex3DColor & vPos1,
	Vertex3DColor & vPos2, int _idx)
{
	if (_idx >= m_vecCell.size())
		return;

	m_vecCell[_idx]->tCopyVertex[0] = &vPos0;
	m_vecCell[_idx]->tCopyVertex[1] = &vPos1;
	m_vecCell[_idx]->tCopyVertex[2] = &vPos2;

}

void CNavigationMesh::AddAdj(int iCellIdx, int iAdjIdx)
{
	if (iCellIdx < 0 || iCellIdx >= m_vecCell.size())
		return;

	ADJINFO	tInfo = {};
	tInfo.iIndex = iAdjIdx;
	tInfo.iEdgeIndex = 0;
	m_vecCell[iCellIdx]->vecAdj.push_back(tInfo);
}

void CNavigationMesh::CreateGridMapAdj(int iLineRectCount)
{
	m_bGrid = true;
	m_iLineRectCount = iLineRectCount;

	size_t iSize = m_vecCell.size();
	ADJINFO	tInfo = {};

	for (size_t i = 0; i < iSize; i += 2)
	{
		// 우상단 삼각형 인접정보 구성.
		// 위쪽 사각형, 오른쪽 사각형, 자기자신 사각형의 왼쪽 하단 삼각형이
		// 인접정보 후보가 된다.
		// 위쪽 사각형의 좌하단 삼각형 인덱스를 구한다.
		int	idx = (int)(i - iLineRectCount * 2 + 1);
		if (idx >= 0 && idx < m_vecCell.size())
		{
			tInfo.iIndex = idx;
			tInfo.iEdgeIndex = 0;
			m_vecCell[i]->vecAdj.push_back(tInfo);
		}

		// 자기자신 사각형의 왼쪽 하단 삼각형 인덱스를 구한다.
		idx = (int)i + 1;
		if (idx >= 0 && idx < m_vecCell.size())
		{
			tInfo.iIndex = idx;
			tInfo.iEdgeIndex = 2;
			m_vecCell[i]->vecAdj.push_back(tInfo);
		}

		// 오른쪽 사각형의 왼쪽 하단 삼각형 인덱스를 구한다.
		if ((i / 2) % iLineRectCount != iLineRectCount - 1)
		{
			idx = (int)i + 3;
			tInfo.iIndex = idx;
			tInfo.iEdgeIndex = 1;
			m_vecCell[i]->vecAdj.push_back(tInfo);
		}

		// 좌하단 삼각형 인접정보 구성.
		// 아래쪽 사각형, 왼쪽 사각형, 자기자신 사각형의 오른쪽 상단 삼각형이
		// 인접정보 후보가 된다.
		// 자기자신 사각형의 우상단 삼각형 인덱스를 구한다.
		tInfo.iIndex = (int)i;
		tInfo.iEdgeIndex = 0;
		m_vecCell[i + 1]->vecAdj.push_back(tInfo);

		// 왼쪽 사각형의 우상단 삼각형 인덱스를 구한다.
		if ((i / 2) % iLineRectCount != 0)
		{
			tInfo.iIndex = (int)i - 3;
			tInfo.iEdgeIndex = 2;
			m_vecCell[i + 1]->vecAdj.push_back(tInfo);
		}

		idx = (int)i + (iLineRectCount * 2);
		if (idx < m_vecCell.size())
		{
			tInfo.iIndex = idx;
			tInfo.iEdgeIndex = 1;
			m_vecCell[i + 1]->vecAdj.push_back(tInfo);
		}
	}

	CreateSection();
}

void CNavigationMesh::CreateAdj()
{
	size_t	iSize = m_vecCell.size();

	FILE*	pFile = NULL;

	fopen_s(&pFile, "NaviAdj.txt", "wt");

	Vector3	vOrigin[2][3];
	Vector3	vEdgeDir[2][3];
	float	fLength[2][3];

	for (size_t i = 0; i < iSize; ++i)
	{
		Vector3	vSrc[3];
		Vector3	vSrcOrigin[3];
		float	fSrcLength[3] = {};
		for (int k = 0; k < 3; ++k)
		{
			vEdgeDir[0][k] = m_vecCell[i]->vEdge[k];

			fLength[0][k] = vEdgeDir[0][k].Length();

			vEdgeDir[0][k].Normalize();
		}

		vOrigin[0][0] = m_vecCell[i]->vPos[0];
		vOrigin[0][1] = m_vecCell[i]->vPos[1];
		vOrigin[0][2] = m_vecCell[i]->vPos[2];

		for (size_t j = i + 1; j < iSize; ++j)
		{
			// 삼각형의 3개의 엣지들을 서로 내적하여 평행한지 비교한다.
			Vector3	vDest[3];
			Vector3	vDestOrigin[3];
			int	iPosIdx[3][2] = { 1, 0, 2, 1, 2, 0 };
			bool	bInsert = false;

			vOrigin[1][0] = m_vecCell[j]->vPos[0];
			vOrigin[1][1] = m_vecCell[j]->vPos[1];
			vOrigin[1][2] = m_vecCell[j]->vPos[2];

			for (int k = 0; k < 3; ++k)
			{
				vEdgeDir[1][k] = m_vecCell[j]->vEdge[k];

				fLength[1][k] = vEdgeDir[1][k].Length();

				vEdgeDir[1][k].Normalize();
			}

			for (int k = 0; k < 3; ++k)
			{
				if (bInsert)
					break;

				for (int l = 0; l < 3; ++l)
				{
					// A삼각형과 B삼각형의 엣지를 구성하는 두 정점이 같을 경우
					// 이 삼각형은 무조건 붙어있는 삼각형이다.
					if ((vOrigin[0][iPosIdx[k][0]] == vOrigin[1][iPosIdx[l][0]] ||
						vOrigin[0][iPosIdx[k][0]] == vOrigin[1][iPosIdx[l][1]]) &&
						(vOrigin[0][iPosIdx[k][1]] == vOrigin[1][iPosIdx[l][0]] ||
							vOrigin[0][iPosIdx[k][1]] == vOrigin[1][iPosIdx[l][1]]))
					{
						ADJINFO	tInfo = {};
						tInfo.iIndex = (int)j;
						tInfo.iEdgeIndex = k;
						m_vecCell[i]->vecAdj.push_back(tInfo);

						tInfo.iIndex = (int)i;
						tInfo.iEdgeIndex = l;
						m_vecCell[j]->vecAdj.push_back(tInfo);
						bInsert = true;
						break;
					}

					// 두 엣지에 대해서 상대 엣지를 구성하는 2개의 점이 대상 엣지
					// 위에 둘다 존재하는지를 판단한다. 둘다 존재한다면
					// 무조건 붙어있는 것이다.
					// (B1 - A1, B2 - A1), (B1 - A2, B2 - A2)
					// 내적을 통해서 빼준 벡터의 크기가 그대로 나온다면
					// 직선상에 존재하는 것이다. 단, 둘다 크기가 나올 경우는
					// 무조건 존재하는 것이고 둘중 하나만 나온다면 크기가 0이라면
					// 하나의 점만 직선위에 존재하고 두 점 위치가 같다는 것이므로
					// 해당 삼각형은 인접 삼각형이 아니다.
					// B1 - A1 처리
					if (CheckOnEdge((int)i, (int)j, vOrigin[1][iPosIdx[l][0]],
						vOrigin[0][iPosIdx[k][0]], vEdgeDir[0][k], fLength[0][k], k, l))
					{
						bInsert = true;
						break;
					}

					// B2 - A1 처리
					else if (CheckOnEdge((int)i, (int)j, vOrigin[1][iPosIdx[l][1]],
						vOrigin[0][iPosIdx[k][0]], vEdgeDir[0][k], fLength[0][k], k, l))
					{
						bInsert = true;
						break;
					}

					// B1 - A2 처리
					else if (CheckOnEdge((int)i, (int)j, vOrigin[1][iPosIdx[l][0]],
						vOrigin[0][iPosIdx[k][1]], vEdgeDir[0][k] * -1.f, fLength[0][k], k, l))
					{
						bInsert = true;
						break;
					}

					// B2 - A2 처리
					else if (CheckOnEdge((int)i, (int)j, vOrigin[1][iPosIdx[l][1]],
						vOrigin[0][iPosIdx[k][1]], vEdgeDir[0][k] * -1.f, fLength[0][k], k, l))
					{
						bInsert = true;
						break;
					}
				}
			}
		}

		string	strText;

		char	strNumber[128] = {};
		_itoa_s((int)i, strNumber, 10);
		strText += strNumber;
		strText += " Adj : ";

		memset(strNumber, 0, 128);

		_cprintf("%d Cell Adj : ", i);

		for (size_t j = 0; j < m_vecCell[i]->vecAdj.size(); ++j)
		{
			_itoa_s(m_vecCell[i]->vecAdj[j].iIndex, strNumber, 10);
			strText += strNumber;
			strText += ", ";
			_cprintf("%d, ", m_vecCell[i]->vecAdj[j].iIndex);
		}

		_cprintf("\n");
		strText += "\n";

		fputs(strText.c_str(), pFile);
	}

	fclose(pFile);

	CreateSection();
}

bool CNavigationMesh::CheckOnEdge(int iSrc, int iDest,
	const Vector3 & vOrigin1, const Vector3 & vOrigin2,
	const Vector3 & vEdge, float fEdgeLength, int iEdge1, int iEdge2)
{
	if ((int)(vOrigin2.x * 100) == (int)(vOrigin1.x * 100) && (int)(vOrigin2.y * 100) == (int)(vOrigin1.y * 100) &&
		(int)(vOrigin2.z * 100) == (int)(vOrigin1.z * 100))
		return false;

	Vector3	vResult = vOrigin1 - vOrigin2;

	float	fDist = vResult.Length();

	// 위에서 구해준 거리를 이용해서 원점으로부터 엣지 방향으로 지정된 거리만큼 이동된 위치에 대상 점이 있는지를
	// 판단한다.
	vResult = vOrigin2 + vEdge * fDist;

	if ((int)(vResult.x * 100) == (int)(vOrigin1.x * 100) && (int)(vResult.y * 100) == (int)(vOrigin1.y * 100) &&
		(int)(vResult.z * 100) == (int)(vOrigin1.z * 100))
	{
		vResult -= vOrigin2;
		if (vResult.Length() < fEdgeLength)
		{
			ADJINFO	tInfo = {};
			tInfo.iIndex = iDest;
			tInfo.iEdgeIndex = iEdge1;
			m_vecCell[iSrc]->vecAdj.push_back(tInfo);

			tInfo.iIndex = iSrc;
			tInfo.iEdgeIndex = iEdge2;
			m_vecCell[iDest]->vecAdj.push_back(tInfo);
			return true;
		}
	}

	return false;
}

bool CNavigationMesh::CheckPathDir(const Vector3 & vSrc1,
	const Vector3 & vSrc2, const Vector3 & vDest1,
	const Vector3 & vDest2, Vector3 & vIntersect)
{
	double t;
	double s;
	double under = (vDest2.z - vDest1.z)*(vSrc2.x - vSrc1.x) - (vDest2.x - vDest1.x)*(vSrc2.z - vSrc1.z);
	if (under == 0)
		return false;

	double _t = (vDest2.x - vDest1.x)*(vSrc1.z - vDest1.z) - (vDest2.z - vDest1.z)*(vSrc1.x - vDest1.x);
	double _s = (vSrc2.x - vSrc1.x)*(vSrc1.z - vDest1.z) - (vSrc2.z - vSrc1.z)*(vSrc1.x - vDest1.x);

	t = _t / under;
	s = _s / under;

	if (t<0.0 || t>1.0 || s<0.0 || s>1.0)
		return false;

	if (_t == 0 && _s == 0)
		return false;

	vIntersect.x = (float)(vSrc1.x + t * (double)(vSrc2.x - vSrc1.x));
	vIntersect.z = (float)(vSrc1.z + t * (double)(vSrc2.z - vSrc1.z));

	return true;
}

void CNavigationMesh::FindPath(const Vector3 & vStart,
	const Vector3 & vEnd)
{
	PNavigationCell	pStart = FindCell(vStart);
	PNavigationCell	pEnd = FindCell(vEnd);

	if (!pStart || !pEnd)
		return;

	else if (!pEnd->bEnable || pEnd->bMove == false)
		return;

	if (pStart == pEnd)
	{
		m_PathList.clear();
		m_PathList.push_back(vEnd);
		return;
	}

	if (!m_pCloseList)
		m_pCloseList = new PNavigationCell[m_vecCell.size()];

	for (int i = 0; i < m_iCloseListSize; ++i)
	{
		m_pCloseList[i]->eType = NCLT_NONE;
		m_pCloseList[i]->iParentIdx = -1;
		m_pCloseList[i]->fG = -1.f;
		m_pCloseList[i]->fH = -1.f;
		m_pCloseList[i]->fTotal = -1.f;
	}

	m_iCloseListSize = 0;

	while (!m_OpenList.empty())
	{
		PNavigationCell	pOpenCell = nullptr;
		m_OpenList.pop(pOpenCell);

		pOpenCell->eType = NCLT_NONE;
		pOpenCell->iParentIdx = -1;
		pOpenCell->fG = -1.f;
		pOpenCell->fH = -1.f;
		pOpenCell->fTotal = -1.f;
	}

	while (!m_FindStack.empty())
	{
		m_FindStack.pop();
	}

	m_PathList.clear();

	// 시작 노드를 열린목록에 넣어준다.
	pStart->eType = NCLT_OPEN;
	pStart->fG = 0.f;
	Vector3 Temp = vStart;
	pStart->fH = Temp.Distance(vEnd);
	pStart->fTotal = pStart->fH;

	m_OpenList.Insert(pStart);

	m_bFindEnd = false;

	PNavigationCell	pCell = nullptr;

	while (!m_OpenList.empty() && !m_bFindEnd)
	{
		// 열린목록에서 셀을 얻어온다.
		m_OpenList.pop(pCell);

		// 얻어온 셀은 닫힌목록으로 만들어준다.
		pCell->eType = NCLT_CLOSE;

		m_pCloseList[m_iCloseListSize] = pCell;
		++m_iCloseListSize;

		// 인접한 셀을 얻어온다. 열린목록에 포함시킬지 판단해야 하기
		// 때문이다.
		AddOpenList(pCell, pEnd, vStart, vEnd);
	}
}

float CNavigationMesh::GetY(const Vector3 & vPos)
{
	if (!m_bGrid)
	{
		int	iSectionIndex = GetSectionIndex(vPos);

		if (0 <= iSectionIndex && iSectionIndex < m_iSectionX * m_iSectionZ)
		{
			Vector3	vOrigin = vPos;
			vOrigin.y = m_vMax.y;
			Vector3	vDir = Vector3(0.f, -1.f, 0.f);

			for (int i = 0; i < m_pSection[iSectionIndex].iSize; ++i)
			{
				Vector3	vIntersect;
				float	fDist = 0.f;
				PNavigationCell	pCell = m_pSection[iSectionIndex].pCellList[i];

				if (RayIntersectTriangle(vOrigin, vDir, pCell->vPos[0],
					pCell->vPos[1],
					pCell->vPos[2],
					fDist, vIntersect))
					return vIntersect.y;
			}
		}

		return 0.f;
	}

	Vector3	vCellSize = (m_vMax - m_vMin) / m_iLineRectCount;
	Vector3	vConvertPos = vPos - m_vMin;

	// 가로, 세로를 1로 만들어준다.
	vConvertPos.x /= vCellSize.x;
	vConvertPos.z /= vCellSize.z;

	// 사각형 인덱스를 구한다.
	int	idxX = (int)vConvertPos.x;
	int	idxZ = m_iLineRectCount - ((int)vConvertPos.z + 1);

	if (idxX < 0 || idxX > m_iLineRectCount)
		return 0.f;

	else if (idxZ < 0 || idxZ > m_iLineRectCount)
		return 0.f;

	int	idx = (idxZ * m_iLineRectCount + idxX) * 2;

	if (!m_vecCell[idx]->bEnable || m_vecCell[idx]->bMove == false)
		return 0;

	// 구해준 사각형의 좌상단 점을 구한다.
	Vector3	vLTPos = m_vecCell[idx]->vPos[0];
	vLTPos.x /= vCellSize.x;
	vLTPos.z /= vCellSize.z;

	float	fX = vConvertPos.x - vLTPos.x;
	float	fZ = vLTPos.z - vConvertPos.z;

	// 우상단 삼각형일 경우
	if (fX >= fZ)
	{
		// Cell을 구성하는 점의 Y값을 얻어온다.
		float	fY[3] = {};
		for (int i = 0; i < 3; ++i)
		{
			fY[i] = m_vecCell[idx]->vPos[i].y;
		}

		return fY[0] + (fY[1] - fY[0]) * fX + (fY[2] - fY[1]) * fZ;
	}

	// 좌하단 삼각형일 경우
	float	fY[3] = {};
	for (int i = 0; i < 3; ++i)
	{
		fY[i] = m_vecCell[idx + 1]->vPos[i].y;
	}

	return fY[0] + (fY[1] - fY[2]) * fX + (fY[2] - fY[0]) * fZ;
}

bool CNavigationMesh::CheckCell(const Vector3 & vPos)
{
	int	iCellIndex = GetCellIndex(vPos);

	if (iCellIndex == -1)
		return false;

	float	fY = GetY(iCellIndex, vPos);

	if (!m_vecCell[iCellIndex]->bEnable || (vPos.y - 2.f > fY || fY > vPos.y + 2.f)
		|| m_vecCell[iCellIndex]->bMove == false)
		return false;

	return true;
}

float CNavigationMesh::GetY(int iCellIndex, const Vector3 & vPos)
{
	if (!m_bGrid)
	{
		Vector3	vOrigin = vPos;
		vOrigin.y = m_vMax.y;
		Vector3	vDir = Vector3(0.f, -1.f, 0.f);
		Vector3	vIntersect;
		float	fDist = 0.f;

		if (RayIntersectTriangle(vOrigin, vDir, m_vecCell[iCellIndex]->vPos[0],
			m_vecCell[iCellIndex]->vPos[1],
			m_vecCell[iCellIndex]->vPos[2],
			fDist, vIntersect))
			return vIntersect.y;

		return 0.f;
	}

	Vector3	vCellSize = (m_vMax - m_vMin) / m_iLineRectCount;
	Vector3	vConvertPos = vPos - m_vMin;

	// 가로, 세로를 1로 만들어준다.
	vConvertPos.x /= vCellSize.x;
	vConvertPos.z /= vCellSize.z;

	if (!m_vecCell[iCellIndex]->bEnable || m_vecCell[iCellIndex]->bMove == false)
		return 0.f;

	// 구해준 사각형의 좌상단 점을 구한다.
	Vector3	vLTPos = m_vecCell[iCellIndex]->vPos[0];
	vLTPos.x /= vCellSize.x;
	vLTPos.z /= vCellSize.z;

	float	fX = vConvertPos.x - vLTPos.x;
	float	fZ = vLTPos.z - vConvertPos.z;

	// Cell을 구성하는 점의 Y값을 얻어온다.
	float	fY[3] = {};
	for (int i = 0; i < 3; ++i)
	{
		fY[i] = m_vecCell[iCellIndex]->vPos[i].y;
	}

	// 우상단 삼각형일 경우
	if (fX >= fZ)
	{
		return fY[0] + (fY[1] - fY[0]) * fX + (fY[2] - fY[1]) * fZ;
	}

	return fY[0] + (fY[1] - fY[2]) * fX + (fY[2] - fY[0]) * fZ;
}

void CNavigationMesh::CreateSection()
{
	m_iSectionX = 5;
	m_iSectionZ = 5;

	Vector3	vLength = m_vMax - m_vMin;
	m_vSectionSize = vLength / Vector3((float)m_iSectionX, 1, (float)m_iSectionZ);

	SAFE_DELETE_ARRAY(m_pSection);

	m_pSection = new NavSection[m_iSectionX * m_iSectionZ];

	for (int i = 0; i < m_iSectionZ; ++i)
	{
		for (int j = 0; j < m_iSectionX; ++j)
		{
			int	idx = i * m_iSectionX + j;
			m_pSection[idx].vSize = m_vSectionSize;
			m_pSection[idx].vMin = m_vMin + m_vSectionSize * Vector3((float)j, 0, (float)i);
			m_pSection[idx].vMax = m_vMin + m_vSectionSize * Vector3(((float)j + 1), 1, ((float)i + 1));
		}
	}

	// 최대 4개의 영역에 포함될 수 있다.
	// 어디 포함되어 있는지 판단해야 한다. 제발.
	for (size_t i = 0; i < m_vecCell.size(); ++i)
	{
		// 삼각형을 구성하는 3개의 점을 이용해서 인덱스를 구한다.
		int	iMinX = 100000, iMinZ = 100000;
		int	iMaxX = -100000, iMaxZ = -100000;

		for (int j = 0; j < 3; ++j)
		{
			Vector3	vPos = m_vecCell[i]->vPos[j] - m_vMin;
			vPos /= m_vSectionSize;

			int x, z;
			x = (int)vPos.x;
			z = (int)vPos.z;

			if (iMinX > x)
				iMinX = x;

			if (iMaxX < x)
				iMaxX = x;

			if (iMinZ > z)
				iMinZ = z;

			if (iMaxZ < z)
				iMaxZ = z;
		}

		iMinX = iMinX < 0 ? 0 : iMinX;
		iMinZ = iMinZ < 0 ? 0 : iMinZ;
		iMaxX = iMaxX >= m_iSectionX ? m_iSectionX - 1 : iMaxX;
		iMaxZ = iMaxZ >= m_iSectionZ ? m_iSectionZ - 1 : iMaxZ;

		if (iMinX < m_iSectionX && iMaxX >= 0 &&
			iMinZ < m_iSectionZ && iMaxZ >= 0)
		{
			for (int j = iMinZ; j <= iMaxZ; ++j)
			{
				for (int k = iMinX; k <= iMaxX; ++k)
				{
					int	idx = j * m_iSectionX + k;

					m_pSection[idx].Add(m_vecCell[i]);
				}
			}
		}
	}
}

void CNavigationMesh::AddOpenList(PNavigationCell pCell,
	PNavigationCell pEnd, const Vector3& vStart,
	const Vector3& vEnd)
{
	for (size_t i = 0; i < pCell->vecAdj.size(); ++i)
	{
		PNavigationCell	pAdj = m_vecCell[pCell->vecAdj[i].iIndex];

		if (!pAdj->bEnable || pAdj->bMove == false)
			continue;

		else if (pAdj->eType == NCLT_CLOSE)
			continue;

		// 도착셀을 찾았을 경우
		if (pAdj == pEnd)
		{
			m_FindStack.push(pAdj->iIndex);

			// 부모의 인덱스를 얻어온다.
			int	iParent = pCell->iIndex;

			while (iParent != -1)
			{
				m_FindStack.push(iParent);
				iParent = m_vecCell[iParent]->iParentIdx;
			}

			// 스택으로 만들어진 경로를 원래대로 되돌린다.
			vector<int>	vecPathIndex;

			while (!m_FindStack.empty())
			{
				int	idx = m_FindStack.top();
				m_FindStack.pop();
				vecPathIndex.push_back(idx);
			}

			vector<int>	vecCenter;
			for (size_t j = 0; j < vecPathIndex.size() - 1; ++j)
			{
				int	iEdgeIndex = -1;

				pCell = m_vecCell[vecPathIndex[j]];

				for (size_t k = 0; k < pCell->vecAdj.size(); ++k)
				{
					if (pCell->vecAdj[k].iIndex == vecPathIndex[j + 1])
					{
						iEdgeIndex = pCell->vecAdj[k].iEdgeIndex;
						break;
					}
				}

				// 위에서 어느 엣지를 기준으로 인접정보가 구성되었는지
				// 찾았다면 여기서 센터정보를 구해준다.
				vecCenter.push_back(iEdgeIndex);

				m_PathList.push_back(pCell->vEdgeCenter[iEdgeIndex]);
			}

			m_PathList.push_back(vEnd);
			m_bFindEnd = true;
			return;
		}

		float	fG = pAdj->vCenter.Distance(pCell->vCenter);
		float	fH = pAdj->vCenter.Distance(vEnd);

		if (pAdj->eType == NCLT_NONE)
		{
			pAdj->fG = fG + pCell->fG;
			pAdj->fH = fH;
			pAdj->fTotal = pAdj->fG + pAdj->fH;
			pAdj->iParentIdx = pCell->iIndex;
			pAdj->eType = NCLT_OPEN;
			m_OpenList.Insert(pAdj);
		}

		else if (pAdj->fG > fG + pCell->fG)
		{
			pAdj->fG = fG + pCell->fG;
			pAdj->fH = fH;
			pAdj->fTotal = pAdj->fG + pAdj->fH;
			pAdj->iParentIdx = pCell->iIndex;
			m_OpenList.Sort();
		}
	}
}

PNavigationCell CNavigationMesh::FindCell(const Vector3 & vPos)
{
	int	idx = GetCellIndex(vPos);

	if (idx < 0 || idx >= m_vecCell.size())
		return nullptr;

	return m_vecCell[idx];
}

int CNavigationMesh::GetCellIndex(const Vector3 & vPos)
{
	if (!m_bGrid)
	{
		int	iSectionIndex = GetSectionIndex(vPos);

		if (0 <= iSectionIndex && iSectionIndex < m_iSectionX * m_iSectionZ)
		{
			Vector3	vOrigin = vPos;
			vOrigin.y = m_vMax.y;
			Vector3	vDir = Vector3(0.f, -1.f, 0.f);

			for (int i = 0; i < m_pSection[iSectionIndex].iSize; ++i)
			{
				Vector3	vIntersect;
				float	fDist = 0.f;
				PNavigationCell	pCell = m_pSection[iSectionIndex].pCellList[i];

				if (RayIntersectTriangle(vOrigin, vDir, pCell->vPos[0],
					pCell->vPos[1],
					pCell->vPos[2],
					fDist, vIntersect))
					return pCell->iIndex;
			}
		}

		return -1;
	}

	Vector3	vCellSize = (m_vMax - m_vMin) / m_iLineRectCount;
	Vector3	vConvertPos = vPos - m_vMin;

	// 가로, 세로를 1로 만들어준다.
	vConvertPos.x /= vCellSize.x;
	vConvertPos.z /= vCellSize.z;

	// 사각형 인덱스를 구한다.
	int	idxX = (int)vConvertPos.x;
	int	idxZ = m_iLineRectCount - ((int)vConvertPos.z + 1);

	if (idxX < 0 || idxX > m_iLineRectCount)
		return -1;

	else if (idxZ < 0 || idxZ > m_iLineRectCount)
		return -1;

	return (idxZ * m_iLineRectCount + idxX) * 2;
}

bool CNavigationMesh::RayIntersectTriangle(Vector3 rayOrigin,
	Vector3 rayDir, Vector3 v0, Vector3 v1, Vector3 v2,
	float & t, Vector3 & vIntersect)
{
	Vector3 e1, e2, h, s, q;
	float a, f, u, v;

	e1 = v1 - v0;
	e2 = v2 - v0;
	h = rayDir.Cross(e2);
	a = e1.Dot(h);

	if (a > -0.00001 && a < 0.00001)
		return false;

	f = 1.f / a;
	s = rayOrigin - v0;

	u = f * s.Dot(h);

	if (u < 0.f || u > 1.f)
		return false;

	q = s.Cross(e1);

	v = f * rayDir.Dot(q);

	if (v < 0.f || u + v > 1.f)
		return false;

	t = f * e2.Dot(q);

	if (t > 0.00001)
	{
		vIntersect = rayOrigin + rayDir * t;
		return true;
	}

	return false;
}

bool CNavigationMesh::RectIntersectPoint(Vector3 vRectMin, Vector3 vRectMax, Vector3 vTrianglePos)
{
	return vRectMin.x <  vTrianglePos.x && vRectMin.z < vTrianglePos.z && vRectMax.x > vTrianglePos.x && vRectMax.z  > vTrianglePos.z;
}

int CNavigationMesh::GetSectionIndex(Vector3 vPos)
{
	vPos -= m_vMin;
	vPos /= m_vSectionSize;

	int x, z;
	x = (int)vPos.x;
	z = (int)vPos.z;

	return z * m_iSectionX + x;
}

int CNavigationMesh::MousePickGetCellIndex(const int & _iStartSectionIndex,
	const int & _iEndSectionIndex, Vector3 rayOrigin, Vector3 rayDir, Vector3 & vIntersect)
{
	int iStartSectionIndex = _iStartSectionIndex;
	int iEndSectionIndex = _iEndSectionIndex;

	if (iEndSectionIndex > m_iSectionX * m_iSectionZ)
		iEndSectionIndex = m_iSectionX * m_iSectionZ;

	if (iStartSectionIndex > m_iSectionX * m_iSectionZ)
		iStartSectionIndex = m_iSectionX * m_iSectionZ;

	if (iEndSectionIndex < 0)
		iEndSectionIndex = 0;

	if (iStartSectionIndex < 0)
		iStartSectionIndex = 0;

	Vector3	vOrigin = rayOrigin;
	Vector3	vDir = rayDir;

	for (int z = iStartSectionIndex; z < iEndSectionIndex; ++z)
	{
		for (int x = 0; x < m_pSection[z].iSize; ++x)
		{
			float	fDist = 500.0f;

			PNavigationCell	pCell = m_pSection[z].pCellList[x];

			if (RayIntersectTriangle(vOrigin, vDir, pCell->vPos[0], pCell->vPos[1], pCell->vPos[2], fDist, vIntersect))
			{
				//pCell->bMove = isMove;
				PNavigationCell	pCell2 = m_pSection[z].pCellList[x];

				if (pCell2 == NULLPTR)
					return pCell->iIndex;

				//pCell2->tCopyVertex[0]->vColor = Color;
				//pCell2->tCopyVertex[1]->vColor = Color;
				//pCell2->tCopyVertex[2]->vColor = Color;
				CInput::GetInst()->SetSelectCellCenter(pCell->vCenter);
				return pCell->iIndex;
			}
		}
	}

	return -1;
}

void CNavigationMesh::Click(bool _bIsMove, Vector4 _vColor)
{
	int iSelectNavIndex = CInput::GetInst()->GetiSelectNavIndex();
	if (iSelectNavIndex == -1)
	{
		return;
	}

	Vector3 vCenter = m_vecCell[iSelectNavIndex]->vCenter;

	Vector3 vRectMin = vCenter;
	vRectMin.x -= CInput::GetInst()->GetXBrushSize() / 2.f;
	vRectMin.z -= CInput::GetInst()->GetZBrushSize() / 2.f;
	Vector3 vRectMax = vCenter;
	vRectMax.x += CInput::GetInst()->GetXBrushSize() / 2.f;
	vRectMax.z += CInput::GetInst()->GetZBrushSize() / 2.f;

	int iMaxSection = GetSectionIndex(vRectMax);
	int iMinSection = GetSectionIndex(vRectMin);

	if (iMinSection < iMaxSection)
	{
		if (iMaxSection >= m_iSectionX * m_iSectionZ)
		{
			iMaxSection = m_iSectionX * m_iSectionZ - 1;
		}
		for (size_t iSection = iMinSection; iSection <= iMaxSection; ++iSection)
		{
			for (int x = 0; x < m_pSection[iSection].iSize; ++x)
			{
				PNavigationCell	pCell = m_pSection[iSection].pCellList[x];

				bool bCollision = true;

				for (int i = 0; i < 3; ++i)
				{
					if (RectIntersectPoint(vRectMin, vRectMax, pCell->vPos[i]) == false)
					{
						bCollision = false;
						break;
					}
				}

				if (bCollision)
				{
					pCell->bMove = _bIsMove;
					pCell->tCopyVertex[0]->vColor = _vColor;
					pCell->tCopyVertex[1]->vColor = _vColor;
					pCell->tCopyVertex[2]->vColor = _vColor;
				}
			}
		}
	}
	else
	{
		if (iMinSection >= m_iSectionX * m_iSectionZ)
		{
			iMinSection = m_iSectionX * m_iSectionZ - 1;
		}

		for (size_t iSection = iMaxSection; iSection <= iMinSection; ++iSection)
		{
			for (int x = 0; x < m_pSection[iSection].iSize; ++x)
			{
				PNavigationCell	pCell = m_pSection[iSection].pCellList[x];

				bool bCollision = true;

				for (int i = 0; i < 3; ++i)
				{
					if (RectIntersectPoint(vRectMin, vRectMax, pCell->vPos[i]) == false)
					{
						bCollision = false;
						break;
					}
				}

				if (bCollision)
				{
					pCell->bMove = _bIsMove;
					pCell->tCopyVertex[0]->vColor = _vColor;
					pCell->tCopyVertex[1]->vColor = _vColor;
					pCell->tCopyVertex[2]->vColor = _vColor;
				}
			}
		}

	}
}

PNavigationCell CNavigationMesh::GetIndexFromCell(size_t _idx)
{
	return PNavigationCell();
}

void CNavigationMesh::Save(const char * pFileName, const string & strPathKey)
{
	const char* pPath = GET_SINGLE(CPathManager)->FindPathFromMultibyte(strPathKey);

	string	strPath;

	if (pPath)
		strPath = pPath;

	strPath += pFileName;

	SaveFromFullPath(strPath.c_str());
}

void CNavigationMesh::SaveFromFullPath(const char * pFullPath)
{
	FILE*	pFile = NULL;

	fopen_s(&pFile, pFullPath, "wb");

	if (!pFile)
		return;

	fwrite(&m_vMin, sizeof(Vector3), 1, pFile);
	fwrite(&m_vMax, sizeof(Vector3), 1, pFile);
	fwrite(&m_iLineRectCount, sizeof(int), 1, pFile);
	fwrite(&m_bGrid, sizeof(bool), 1, pFile);

	// 셀 수 저장
	size_t	iSize = m_vecCell.size();

	fwrite(&iSize, sizeof(size_t), 1, pFile);

	// 각각의 셀 정보 저장
	for (size_t i = 0; i < iSize; ++i)
	{
		fwrite(m_vecCell[i]->vPos, sizeof(Vector3), 3, pFile);
		fwrite(m_vecCell[i]->vEdge, sizeof(Vector3), 3, pFile);
		fwrite(m_vecCell[i]->vEdgeCenter, sizeof(Vector3), 3, pFile);
		fwrite(&m_vecCell[i]->vCenter, sizeof(Vector3), 1, pFile);
		fwrite(&m_vecCell[i]->iIndex, sizeof(int), 1, pFile);
		fwrite(&m_vecCell[i]->bEnable, sizeof(bool), 1, pFile);
		fwrite(&m_vecCell[i]->bMove, sizeof(bool), 1, pFile);
		fwrite(&m_vecCell[i]->m_iVertexIndex, sizeof(int), 3, pFile);
		size_t	iAdjSize = m_vecCell[i]->vecAdj.size();

		fwrite(&iAdjSize, sizeof(size_t), 1, pFile);

		fwrite(&m_vecCell[i]->vecAdj[0], sizeof(ADJINFO), iAdjSize, pFile);
	}

	fclose(pFile);
}

void CNavigationMesh::Load(const char * pFileName, const string & strPathKey)
{
	const char* pPath = GET_SINGLE(CPathManager)->FindPathFromMultibyte(strPathKey);

	string	strPath;

	if (pPath)
		strPath = pPath;

	strPath += pFileName;

	LoadFromFullPath(strPath.c_str());
}

void CNavigationMesh::LoadFromFullPath(const char * pFullPath)
{
	FILE*	pFile = NULL;

	fopen_s(&pFile, pFullPath, "rb");

	if (!pFile)
		return;

	fread(&m_vMin, sizeof(Vector3), 1, pFile);
	fread(&m_vMax, sizeof(Vector3), 1, pFile);
	fread(&m_iLineRectCount, sizeof(int), 1, pFile);
	fread(&m_bGrid, sizeof(bool), 1, pFile);

	// 셀 수 저장
	size_t	iSize = 0;

	fread(&iSize, sizeof(size_t), 1, pFile);

	Safe_Delete_VecList(m_vecCell);

	// 각각의 셀 정보 저장
	for (size_t i = 0; i < iSize; ++i)
	{
		PNavigationCell	pCell = new NavigationCell;

		pCell->eType = NCLT_NONE;
		pCell->iParentIdx = -1;
		pCell->fG = 0;
		pCell->fH = 0;
		pCell->fTotal = 0;

		fread(pCell->vPos, sizeof(Vector3), 3, pFile);
		fread(pCell->vEdge, sizeof(Vector3), 3, pFile);
		fread(pCell->vEdgeCenter, sizeof(Vector3), 3, pFile);
		fread(&pCell->vCenter, sizeof(Vector3), 1, pFile);
		fread(&pCell->iIndex, sizeof(int), 1, pFile);
		fread(&pCell->bEnable, sizeof(bool), 1, pFile);
		fread(&pCell->bMove, sizeof(bool), 1, pFile);
		fread(&pCell->m_iVertexIndex, sizeof(int), 3, pFile);

		size_t	iAdjSize = 0;

		fread(&iAdjSize, sizeof(size_t), 1, pFile);

		pCell->vecAdj.clear();
		pCell->vecAdj.resize(iAdjSize);

		fread(&pCell->vecAdj[0], sizeof(ADJINFO), iAdjSize, pFile);

		m_vecCell.push_back(pCell);
	}

	fclose(pFile);

	CreateSection();
}

bool CNavigationMesh::OpenListSort(const PNavigationCell & pSrc, const PNavigationCell & pDest)
{
	return pSrc->fTotal < pDest->fTotal;
}
