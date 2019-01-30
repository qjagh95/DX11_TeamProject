#include "EngineHeader.h"
#include "NavigationManager.h"
#include "Component/Stage2D.h"
#include "Component/Transform.h"
#include "Component/Tile2D.h"

PUN_USING

DEFINITION_SINGLE(CNavigationManager)

CNavigationManager::CNavigationManager()
{
}

CNavigationManager::~CNavigationManager()
{
}

void CNavigationManager::AddStage2D(CStage2D * pStage)
{

	m_StageList.push_back(pStage);
}

void CNavigationManager::ClearStage2D()
{
	m_StageList.clear();
}

void CNavigationManager::ClearStage2D(CScene * pScene)
{
	list<CStage2D*>::iterator	iter;
	list<CStage2D*>::iterator	iterEnd = m_StageList.end();

	for (iter = m_StageList.begin(); iter != iterEnd;)
	{
		if ((*iter)->GetScene() == pScene)
		{
			iter = m_StageList.erase(iter);
			iterEnd = m_StageList.end();
			continue;
		}

		++iter;
	}
}

bool CNavigationManager::Find2D(const Vector3 & vStart, const Vector3 & vEnd)
{
	// �������� �������� ��ġ�� �̿��ؼ� � ���������� ���ԵǾ� �ִ����� �Ǵ��Ѵ�.
	list<CStage2D*>::iterator	iter;
	list<CStage2D*>::iterator	iterEnd = m_StageList.end();

	CStage2D*	pStage = nullptr;
	
	for (iter = m_StageList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter) == nullptr)
		{
			continue;
		}

		CTransform*	pTr = (*iter)->GetTransform();

		Vector3	vPos = pTr->GetWorldPos();
		Vector3	vScale = pTr->GetWorldScale();

		if (vPos.x <= vEnd.x && vEnd.x <= vPos.x + vScale.x &&
			vPos.y <= vEnd.y && vEnd.y <= vPos.y + vScale.y)
		{
			pStage = *iter;
			SAFE_RELEASE(pTr);

			break;
		}

		
		SAFE_RELEASE(pTr);
	}

	if (!pStage)
		return false;

	// ���� ã�� �����Ѵ�.
	// ���� ���������κ��� ����Ÿ���� ���´�.
	CTile2D*	pStart = pStage->GetTile(vStart);
	CTile2D*	pEnd = pStage->GetTile(vEnd);

	if (!pEnd)
		return false;

	else if (pEnd->GetTile2DOption() == T2D_NOMOVE)
		return false;

	// Ÿ�� ����� �ʱ�ȭ���ش�.
	pStage->ClearFindSet();

	m_OpenList.clear();

	// ������ϰ� ��������� ������ �����Ѵ�.
	m_OpenList.reserve(pStage->GetTileCount());

	// ���� ��带 ������Ͽ� �־��ش�.
	m_OpenList.push_back(pStart);

	// ���� Ÿ���� ���� ��ġ�� ���Ѵ�.
	CTransform*	pEndTr = pEnd->GetTransform();

	Vector3	vEndCenter = pEndTr->GetWorldPos() + pEndTr->GetWorldScale() * 0.5f;

	m_fTileWidth = pEndTr->GetWorldScale().x;
	m_fTileHeight = pEndTr->GetWorldScale().y;

	SAFE_RELEASE(pEndTr);

	m_PathList.clear();

	while (!m_OpenList.empty())
	{
		// ���� ù��° ��带 ���� ������Ͽ� �־��ش�.
		CTile2D*	pNode = m_OpenList.back();
		m_OpenList.pop_back();

		pNode->SetTileFindList(TFL_CLOSE);

		// ���������� ������Ͽ� �߰��Ѵ�.
		if (AddOpenList(pStage, pNode, vEndCenter, pEnd))
		{
			// ���� ������ ����� �ε����� �̿��ؼ� ���İ����� ��ġ ������ ����Ʈ�� �����Ѵ�.
			while (!m_FindStack.empty())
			{
				int	iIndex = m_FindStack.top();
				m_FindStack.pop();

				CTile2D*	pTile = pStage->GetTile(iIndex);

				CTransform*	pTr = pTile->GetTransform();

				Vector3	vPos = pTr->GetWorldPos() + pTr->GetWorldScale() * 0.5f;
				vPos.z = 0.f;

				pTile->EnableFind();

				SAFE_RELEASE(pTr);

				m_PathList.push_back(vPos);
			}

			return true;
		}
	}

	return false;
}

list<Vector3> CNavigationManager::GetPathList() const
{
	return m_PathList;
}

bool CNavigationManager::AddOpenList(class CStage2D* pStage, CTile2D* pNode,
	const Vector3& vEnd, class CTile2D* pEndTile)
{
	bool	bSort = false;

	for (int i = 0; i < AND_END; ++i)
	{
		int	iIndex = pNode->GetAdjNodeIndex((ASTAR_NODE_DIR)i);

		if (iIndex == -1)
			continue;

		float	fG = 0.f;
		float	fH = 0.f;
		Vector3	vCenter;

		CTile2D*	pTile = pStage->GetTile(iIndex);

		// �� Ÿ���� ������ Ÿ���� ��� �������� �ʴ´�
		if (pTile->GetTile2DOption() == T2D_NOMOVE)
			continue;

		// �������� ã���� ���
		if (pEndTile == pTile)
		{
			// �������� �־��ش�.
			m_FindStack.push(pEndTile->GetIndex());

			int	iParent = pNode->GetIndex();

			while (iParent != -1)
			{
				m_FindStack.push(iParent);
				pTile = pStage->GetTile(iParent);
				iParent = pTile->GetParent();
			}

			return true;
		}

		switch (pTile->GetTileFindList())
		{
		case TFL_CLOSE:
			continue;
		case TFL_OPEN:
			fG = GetG((ASTAR_NODE_DIR)i) + pNode->GetG();

			// ���� Ÿ���� G���� ���Ѵ�.
			if (pTile->GetG() > fG)
			{
				bSort = true;
				pTile->SetG(fG);
				pTile->Total();
				pTile->SetParent(pNode->GetIndex());
			}

			break;
		case TFL_NONE:
			bSort = true;
			fG = GetG((ASTAR_NODE_DIR)i) + pNode->GetG();
			CTransform*	pTr = nullptr;
			pTr = pTile->GetTransform();
			vCenter = pTr->GetWorldPos() + pTr->GetWorldScale() * 0.5f;
			fH = vCenter.Distance(vEnd);

			SAFE_RELEASE(pTr);
			pTile->SetG(fG);
			pTile->SetH(fH);
			pTile->Total();
			pTile->SetParent(pNode->GetIndex());
			pTile->SetTileFindList(TFL_OPEN);

			m_OpenList.push_back(pTile);
			break;
		}
	}

	if (bSort)
		sort(m_OpenList.begin(), m_OpenList.end(), CNavigationManager::SortOpenList);

	return false;
}

float CNavigationManager::GetG(ASTAR_NODE_DIR eDir)
{
	switch (eDir)
	{
	case AND_LT:
	case AND_RT:
	case AND_RB:
	case AND_LB:
		return sqrtf((m_fTileWidth * 0.5f) * (m_fTileWidth * 0.5f) + (m_fTileHeight * 0.5f) * (m_fTileHeight * 0.5f));
	case AND_T:
	case AND_B:
		return m_fTileHeight;
	case AND_R:
	case AND_L:
		return m_fTileWidth;
	}

	return -1.f;
}

bool CNavigationManager::Init()
{
	return true;
}

bool CNavigationManager::SortOpenList(CTile2D * pSrc, CTile2D * pDest)
{
	return pSrc->GetTotal() > pDest->GetTotal();
}