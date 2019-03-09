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

	m_OpenList.SetSortFunc(this, &CNavigationMesh::OpenListSort);
}

CNavigationMesh::~CNavigationMesh()
{
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

void CNavigationMesh::AddCell(const Vector3 vPos[3])
{
	PNavigationCell	pCell = new NavigationCell;

	for (int i = 0; i < 3; ++i)
	{
		pCell->vPos[i] = vPos[i];
	}

	pCell->vEdge[0] = vPos[1] - vPos[0];
	pCell->vEdgeCenter[0] = (vPos[1] + vPos[0]) / 2.f;

	pCell->vEdge[1] = vPos[2] - vPos[1];
	pCell->vEdgeCenter[1] = (vPos[2] + vPos[1]) / 2.f;

	pCell->vEdge[2] = vPos[2] - vPos[0];
	pCell->vEdgeCenter[2] = (vPos[2] + vPos[0]) / 2.f;

	pCell->vCenter = (vPos[0] + vPos[1] + vPos[2]) / 3.f;

	pCell->iIndex = (int)m_vecCell.size();

	for (int i = 0; i < 3; ++i)
	{
		if (m_vMin.x > vPos[i].x)
			m_vMin.x = vPos[i].x;
		if (m_vMin.y > vPos[i].y)
			m_vMin.y = vPos[i].y;
		if (m_vMin.z > vPos[i].z)
			m_vMin.z = vPos[i].z;

		if (m_vMax.x < vPos[i].x)
			m_vMax.x = vPos[i].x;
		if (m_vMax.y < vPos[i].y)
			m_vMax.y = vPos[i].y;
		if (m_vMax.z < vPos[i].z)
			m_vMax.z = vPos[i].z;
	}

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

	m_vecCell.push_back(pCell);
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
	ADJINFO tInfo = {};

	for (int i = 0; i < iSize; i += 2)
	{
		/*
		우상단 삼각형 인접정보 구성
		위쪽 사각형, 오른쪽 사각형, 자기자신 사각형의
		왼쪽 하단 삼각형이 인접정보 후보가 된다
		*/
		// 위쪽 사각형의 좌하단 삼각형 인덱스를 구한다
		int	idx = i - iLineRectCount * 2 + 1;
		if (idx >= 0 && idx < m_vecCell.size())
		{
			tInfo.iIndex = idx;
			tInfo.iEdgeIndex = 0;
			m_vecCell[i]->vecAdj.push_back(tInfo);
		}

		// 자기자신 사각형의 왼쪽 하단 삼각형 인덱스를 구한다.
		idx = i + 1;
		if (idx >= 0 && idx < m_vecCell.size())
		{
			tInfo.iIndex = idx;
			tInfo.iEdgeIndex = 1;
			m_vecCell[i]->vecAdj.push_back(tInfo);
		}

		// 오른쪽 사각형의 왼쪽 하단 삼각형 인덱스를 구한다.
		if ((i / 2) % iLineRectCount != iLineRectCount - 1)
		{
			idx = i + 3;
			tInfo.iIndex = idx;
			tInfo.iEdgeIndex = 2;
			m_vecCell[i]->vecAdj.push_back(tInfo);
		}

		/*
		좌하단 삼각형 인접정보 구성
		아래쪽 사각형, 왼쪽 사각형, 자기자신 사각형의
		오른쪽 상단 삼각형이 인접정보 후보가 된다
		*/
		// 자기자신 사각형의 우상단 삼각형 인덱스를 구한다
		tInfo.iIndex = i;
		tInfo.iEdgeIndex = 0;
		m_vecCell[i + 1]->vecAdj.push_back(tInfo);

		// 왼쪽 사각형의 우상단 삼각형 인덱스를 구한다.
		if ((i / 2) % iLineRectCount != 0)
		{
			tInfo.iIndex = i - 2;
			tInfo.iEdgeIndex = 1;
			m_vecCell[i + 1]->vecAdj.push_back(tInfo);
		}

		idx = i + (iLineRectCount * 2);
		if (idx < m_vecCell.size())
		{
			tInfo.iIndex = idx;
			tInfo.iEdgeIndex = 2;
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

	for (int i = 0; i < iSize; ++i)
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

		for (int j = i + 1; j < iSize; ++j)
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
						tInfo.iIndex = j;
						tInfo.iEdgeIndex = k;
						m_vecCell[i]->vecAdj.push_back(tInfo);

						tInfo.iIndex = i;
						tInfo.iEdgeIndex = l;
						m_vecCell[j]->vecAdj.push_back(tInfo);
						bInsert = true;
						break;
					}

					/*
					두 엣지에 대해서 상대 엣지를 구성하는 2개의 점이 
					대상 엣지 위에 둘 다 존재하는지를 판단한다
					둘 다 존재한다면 무조건 붙어있는 것이다.
					(B1 - A1, B2 - A1), (B1 - A2, B2 - A2)
					내적을 통해서 빼준 벡터의 크기가 그대로 나온다면
					직선 상에 존재하는 것이다. 단, 둘 다 크기가 나올 경우는
					무조건 존재하는 것이다.
					둘 중 하나만 나온다면 크기가 0이면 하나의 점만 직선 위에 존재하고
					두 점 위치가 같다는 것이므로 해당 삼ㄱ가형은
					인접 삼각형이 아니다
					*/
					// B1 - A1 처리
					if (CheckOnEdge(i, j, vOrigin[1][iPosIdx[l][0]],
						vOrigin[0][iPosIdx[k][0]], vEdgeDir[0][k], fLength[0][k], k, l))
					{
						bInsert = true;
						break;
					}

					// B2 - A1 처리
					else if (CheckOnEdge(i, j, vOrigin[1][iPosIdx[l][1]],
						vOrigin[0][iPosIdx[k][0]], vEdgeDir[0][k], fLength[0][k], k, l))
					{
						bInsert = true;
						break;
					}

					// B1 - A2 처리
					else if (CheckOnEdge(i, j, vOrigin[1][iPosIdx[l][0]],
						vOrigin[0][iPosIdx[k][1]], vEdgeDir[0][k] * -1.f, fLength[0][k], k, l))
					{
						bInsert = true;
						break;
					}

					// B2 - A2 처리
					else if (CheckOnEdge(i, j, vOrigin[1][iPosIdx[l][1]],
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
		_itoa_s(i, strNumber, 10);
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
	// 소수점 두번째 자리 까지만 비교
	if ((int)(vOrigin2.x * 100) == (int)(vOrigin1.x * 100) &&
		(int)(vOrigin2.y * 100) == (int)(vOrigin1.y * 100) &&
		(int)(vOrigin2.z * 100) == (int)(vOrigin1.z * 100))
		return false;

	Vector3 vResult = vOrigin1 - vOrigin2;

	float	fDist = vResult.Length();

	// 위에서 구해준 거리를 이용해서 원점으로부터 엣지 방향으로 지정된
	// 거리만큼 이동된 위치에 대상 점이 있는지를 판단한다
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

void CNavigationMesh::FindPath(Vector3 & vStart, const Vector3 & vEnd)
{
	PNavigationCell	pStart = FindCell(vStart);
	PNavigationCell	pEnd = FindCell(vEnd);

	if (!pStart || !pEnd)
		return;

	else if (!pEnd->bEnable)
		return;

	m_OpenList.Clear();

	for (size_t i = 0; i < m_vecCell.size(); ++i)
	{
		m_vecCell[i]->Clear();
	}

	while (!m_FindStack.empty())
	{
		m_FindStack.pop();
	}

	// 시작 노드를 열린목록어 넣어준다.
	pStart->eType = NCLT_OPEN;
	pStart->fG = 0.f;
	pStart->fH = vStart.Distance(vEnd);
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

		// 인접한 셀을 얻어온다. 열린목록에 포함시킬지 
		// 판단해야 하기 때문이다.
		AddOpenList(pCell, pEnd, vStart, vEnd);
	}
}

float CNavigationMesh::GetY(const Vector3& vPos)
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

	Vector3 vCellSize = (m_vMax - m_vMin) / m_iLineRectCount;
	Vector3 vConvertPos = vPos - m_vMin;

	// 가로, 세로를 1로 만들어준다
	vConvertPos.x /= vCellSize.x;
	vConvertPos.z /= vCellSize.z;

	// 사각형 인덱스를 구한다
	int idxX = (int)vConvertPos.x;
	int idxZ = m_iLineRectCount - ((int)vConvertPos.z + 1);

	if (idxX <0 || idxX > m_iLineRectCount)
		return 0.f;

	else if (idxZ <0 || idxZ >m_iLineRectCount)
		return 0.f;

	int idx = (idxZ * m_iLineRectCount + idxX) * 2;

	if (!m_vecCell[idx]->bEnable)
		return 0;

	// 구해준 사각형의 좌상단 점을 구한다
	Vector3 vLTPos = m_vecCell[idx]->vPos[0];
	vLTPos.x /= vCellSize.x;
	vLTPos.z /= vCellSize.z;

	float	fX = vConvertPos.x - vLTPos.x;
	float	fZ = vLTPos.z - vConvertPos.z;

	// 우상단 삼각형일 경우
	if (fX >= fZ)
	{
		// Cell을 구성하는 점의 Y값을 얻어온다
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

	return 0.0f;
}

bool CNavigationMesh::CheckCell(const Vector3 & vPos)
{
	int	iCellIndex = GetCellIndex(vPos);

	if (iCellIndex == -1)
		return false;

	float	fY = GetY(iCellIndex, vPos);

	if (!m_vecCell[iCellIndex]->bEnable ||
		(vPos.y - 2.f > fY || fY > vPos.y + 2.f))
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

	if (!m_vecCell[iCellIndex]->bEnable)
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
			m_pSection[idx].vMax = m_vMin + m_vSectionSize * Vector3((float)(j + 1), (float)1, (float)(i + 1));
		}
	}

	// 최대 4개의 영역에 포함될 수 있다.
	// 어디 포함되어 있는지 판단해야 한다.
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

void CNavigationMesh::AddOpenList(PNavigationCell pCell, PNavigationCell pEnd, const Vector3 & vStart, const Vector3 & vEnd)
{
	for (size_t i = 0; i < pCell->vecAdj.size(); ++i)
	{
		PNavigationCell	pAdj = m_vecCell[pCell->vecAdj[i].iIndex];

		if (!pAdj->bEnable)
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
			}

			// 정해진 경로의 인덱스 수만큼 반복하며 해당 삼각형을
			// 관통하는지 판단한다.
			Vector3	vStartPos = vStart;
			for (size_t j = 2; j < vecPathIndex.size() - 1; ++j)
			{
				int	idx = vecPathIndex[j];

				Vector3	vDir = m_vecCell[idx]->vEdgeCenter[vecCenter[j]] - vStartPos;
				float	fDist = vDir.Length() + 0.1f;
				vDir.Normalize();

				Vector3	vEndPos = vStartPos + vDir * fDist;

				for (int k = 0; k < 3; ++k)
				{
				}
			}

			m_bFindEnd = true;

			return;
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

bool CNavigationMesh::RayIntersectTriangle(Vector3 rayOrigin, Vector3 rayDir, 
	Vector3 v0, Vector3 v1, Vector3 v2, 
	float & fDist, Vector3 & vIntersect)
{
	Vector3 Edge1 = v1 - v0;
	Vector3 Edge2 = v2 - v0;

	// 광선과 Edge2의 수직인 벡터를 구한다
	Vector3 vEgde2_Normal = rayDir.Cross(Edge2);

	// 수직인 벡터와 Edge1의 각도를 구해준다
	float fAngle = Edge1.Dot(vEgde2_Normal);

	// 각도가 0이면 즉, 수직이면 관통하지 않는다
	if (fAngle > -0.00001 && fAngle < 0.00001)
		return false;

	float f = 1.f / fAngle;

	// 정점 v0에서 rayorigin으로의 방향을 구한다
	Vector3 vDir_to_rayOrigin = rayOrigin - v0;


	float u = f * vDir_to_rayOrigin.Dot(vEgde2_Normal);

	if (u < 0.f || u > 1.f)
		return false;

	Vector3 q = vDir_to_rayOrigin.Cross(Edge1);

	float v = f * rayDir.Dot(q);

	if (v < 0.f || u + v > 1.f)
		return false;

	fDist = f * Edge2.Dot(q);

	if (fDist > 0.00001)
	{
		vIntersect = rayOrigin + rayDir * fDist;
		return true;
	}

	return false;
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
