#include "EngineHeader.h"
#include "Stage2D.h"
#include "Transform.h"
#include "../GameObject.h"
#include "Tile2D.h"
#include "Renderer.h"
#include "Material.h"
#include "Camera.h"
#include "../Scene/Scene.h"
#include "../Device.h"
#include "../NavigationManager.h"

PUN_USING

CStage2D::CStage2D()
{
	m_eComType = CT_STAGE2D;
	SetTag("Stage2D");

	m_bTileLineOn = true;
	m_eTileType = S2D_NONE;

	GET_SINGLE(CNavigationManager)->AddStage2D(this);
	m_iFlag = -1;	
}

CStage2D::CStage2D(const CStage2D & stage2D) :
	CComponent(stage2D)
{
	*this = stage2D;
}

CStage2D::~CStage2D()
{
	Safe_Release_VecList(m_vecTileTr);
	Safe_Release_VecList(m_vecTile);
	Safe_Release_VecList(m_vecTileObj);
}

STAGE2D_TILE_TYPE CStage2D::GetTileType() const
{
	return m_eTileType;
}

UINT CStage2D::GetTileNumX() const
{
	return m_iNumX;
}

UINT CStage2D::GetTileNumY() const
{
	return m_iNumY;
}

Vector3 CStage2D::GetTileScale() const
{
	return m_vTileScale;
}

CTile2D * CStage2D::GetTile(const Vector3 & vPos) const
{
	int	idx = GetTileIndex(vPos);

	if (idx == -1)
		return nullptr;

	return m_vecTile[idx];
}

CTile2D * CStage2D::GetTile(int iIndex) const
{	
	return m_vecTile[iIndex];
}

size_t CStage2D::GetTileCount() const
{
	return m_vecTile.size();
}

void CStage2D::SetWorldSize(Vector3 vWorldSize)
{
	m_vWorldSize = vWorldSize;
}

void CStage2D::SetTileType(STAGE2D_TILE_TYPE eType)
{
	m_eTileType = eType;
}

void CStage2D::SetTileOption(const Vector3 & vPos, TILE2D_OPTION eOption)
{
	int	iIndex = GetTileIndex(vPos);

	if (iIndex == -1)
		return;

	m_vecTile[iIndex]->SetTileOption(eOption);
}

void CStage2D::OnTile(bool bTile)
{
	m_bTileLineOn = bTile;

	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		m_vecTile[i]->OnTile(bTile);
	}
}

void CStage2D::ClearFindSet()
{
	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		m_vecTile[i]->ClearFindSet();
	}
}

void CStage2D::SetVisible(bool bVisible)
{
	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		m_vecTile[i]->SetEnable(bVisible);
	}
}

void CStage2D::SetVisible()
{
	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		if (m_iFlag == -1)
		{
			m_vecTile[i]->SetEnable(true);
		}
		else if (m_iFlag == 1)
		{
			m_vecTile[i]->SetEnable(false);
		}
	}
	m_iFlag *= -1;
}

void CStage2D::CreateTile(UINT iNumX, UINT iNumY, const Vector3 & vStart,
	const Vector3 & vTileScale, STAGE2D_TILE_TYPE eType,
	const string& strKey, const TCHAR* pFileName,
	const string& strPathKey)
{
	Safe_Release_VecList(m_vecTileTr);
	Safe_Release_VecList(m_vecTile);
	Safe_Release_VecList(m_vecTileObj);

	m_vecTileObj.resize(iNumX * iNumY);
	m_vecTile.resize(iNumX * iNumY);
	m_vecTileTr.resize(iNumX * iNumY);

	Vector3	vScale;
	vScale.x = vTileScale.x * iNumX;
	vScale.y = vTileScale.y * 2 + (iNumY - 2) * (vTileScale.y / 2.f);
	vScale.z = 1.f;

	m_vTileScale = vTileScale;
	m_vStartPos = vStart;
	m_pTransform->SetWorldScale(vScale);
	m_pTransform->SetWorldPos(vStart);

	m_eTileType = eType;
	m_iNumX = iNumX;
	m_iNumY = iNumY;

	switch (eType)
	{
	case S2D_RECT:
		CreateRect(vStart, vTileScale, strKey, pFileName, strPathKey);
		break;
	case S2D_ISOMETRIC:
		CreateIsometric(vStart, vTileScale, strKey, pFileName, strPathKey);
		break;
	}

	for (UINT i = 0; i < m_iNumY; ++i)
	{
		for (UINT j = 0; j < m_iNumX; ++j)
		{
			UINT	iIndex = i * m_iNumX + j;

			// 8방향의 노드를 구한다.
			for (int k = 0; k < AND_END; ++k)
			{
				int	iTileIndex = GetAdjTileIndex(j, i, (ASTAR_NODE_DIR)k, m_eTileType);

				m_vecTile[iIndex]->SetAdjTile(iTileIndex, (ASTAR_NODE_DIR)k);
				m_vecTile[iIndex]->SetIndex(iIndex);
			}
		}
	}
}

int CStage2D::GetAdjTileIndex(int x, int y, ASTAR_NODE_DIR eDir, STAGE2D_TILE_TYPE eType)
{
	switch (eType)
	{
	case S2D_RECT:
		// 방향별 인덱스를 구한다.
		switch (eDir)
		{
		case AND_LT:
			if (x - 1 < 0 || y + 1 >= (int)m_iNumY)
				return -1;

			return (y + 1) * m_iNumX + (x - 1);
		case AND_T:
			if (y + 1 >= (int)m_iNumY)
				return -1;

			return (y + 1) * m_iNumX + x;
		case AND_RT:
			if (x + 1 >= (int)m_iNumX || y + 1 >= (int)m_iNumY)
				return -1;

			return (y + 1) * m_iNumX + (x + 1);
		case AND_R:
			if (x + 1 >= (int)m_iNumX)
				return -1;

			return y * m_iNumX + (x + 1);
		case AND_RB:
			if (x + 1 >= (int)m_iNumX || y - 1 < 0)
				return -1;

			return (y - 1) * m_iNumX + (x + 1);
		case AND_B:
			if (y - 1 < 0)
				return -1;

			return (y - 1) * m_iNumX + x;
		case AND_LB:
			if (x - 1 < 0 || y - 1 < 0)
				return -1;

			return (y - 1) * m_iNumX + (x - 1);
		case AND_L:
			if (x - 1 < 0)
				return -1;

			return y * m_iNumX + (x - 1);
		}
		break;
	case S2D_ISOMETRIC:
		// 방향별 인덱스를 구한다.
		switch (eDir)
		{
		case AND_LT:
			if (y % 2 == 0)
			{
				if (x - 1 < 0 || y + 1 >= (int)m_iNumY)
					return -1;

				return (y + 1) * m_iNumX + (x - 1);
			}

			else
			{
				if (y + 1 >= (int)m_iNumY)
					return -1;

				return (y + 1) * m_iNumX + x;
			}
		case AND_T:
			if (y + 2 >= (int)m_iNumY)
				return -1;

			return (y + 2) * m_iNumX + x;
		case AND_RT:
			if (y % 2 == 0)
			{
				if (y + 1 >= (int)m_iNumY)
					return -1;

				return (y + 1) * m_iNumX + x;
			}

			else
			{
				if (x + 1 >= (int)m_iNumX || y + 1 >= (int)m_iNumY)
					return -1;

				return (y + 1) * m_iNumX + (x + 1);
			}
		case AND_R:
			if (x + 1 >= (int)m_iNumX)
				return -1;

			return y * m_iNumX + (x + 1);
		case AND_RB:
			if (y % 2 == 0)
			{
				if (y - 1 < 0)
					return -1;

				return (y - 1) * m_iNumX + x;
			}

			else
			{
				if (x + 1 >= (int)m_iNumX || y - 1 < 0)
					return -1;

				return (y - 1) * m_iNumX + (x + 1);
			}
		case AND_B:
			if (y - 2 < 0)
				return -1;

			return (y - 2) * m_iNumX + x;
		case AND_LB:
			if (y % 2 == 0)
			{
				if (x - 1 < 0 || y - 1 < 0)
					return -1;

				return (y - 1) * m_iNumX + (x - 1);
			}

			else
			{
				if (y - 1 < 0)
					return -1;

				return (y - 1) * m_iNumX + x;
			}
		case AND_L:
			if (x - 1 < 0)
				return -1;

			return y * m_iNumX + (x - 1);
		}
		break;
	}

	return -1;
}

void CStage2D::CreateRect(const Vector3& vStart, const Vector3& vTileScale,
	const string& strKey, const TCHAR* pFileName,
	const string& strPathKey)
{
	for (UINT i = 0; i < m_iNumY; ++i)
	{
		for (UINT j = 0; j < m_iNumX; ++j)
		{
			UINT	iIndex = i * m_iNumX + j;

			CGameObject*	pTileObj = CGameObject::CreateObject("TileObj");

			pTileObj->SetScene(m_pScene);
			pTileObj->SetLayer(m_pLayer);

			CTile2D*	pTile = pTileObj->AddComponent<CTile2D>("Tile");

			pTile->SetTileType(S2D_RECT);
			pTile->OnTile(m_bTileLineOn);

			CTransform*	pTransform = pTileObj->GetTransform();

			pTransform->SetWorldScale(vTileScale);

			Vector3	vPos = vStart + vTileScale * Vector3((float)j, (float)i, 1.f);

			pTransform->SetWorldPos(vPos);

			if (pFileName)
			{
				CRenderer*	pRenderer = pTileObj->AddComponent<CRenderer>("Renderer");

				pRenderer->SetMesh("TexRect");
				pRenderer->SetRenderState(ALPHA_BLEND);

				SAFE_RELEASE(pRenderer);

				CMaterial*	pMaterial = pTileObj->AddComponent<CMaterial>("Material");

				pMaterial->SetDiffuseTex(0, strKey, pFileName, strPathKey);

				SAFE_RELEASE(pMaterial);
			}

			m_vecTileObj[iIndex] = pTileObj;
			m_vecTileTr[iIndex] = pTransform;
			m_vecTile[iIndex] = pTile;
		}
	}
}

void CStage2D::CreateIsometric(const Vector3& vStart, const Vector3& vTileScale,
	const string& strKey, const TCHAR* pFileName,
	const string& strPathKey)
{
	for (UINT i = 0; i < m_iNumY; ++i)
	{
		for (UINT j = 0; j < m_iNumX; ++j)
		{
			UINT	iIndex = i * m_iNumX + j;

			CGameObject*	pTileObj = CGameObject::CreateObject("TileObj");

			pTileObj->SetScene(m_pScene);
			pTileObj->SetLayer(m_pLayer);

			CTile2D*	pTile = pTileObj->AddComponent<CTile2D>("Tile");

			pTile->SetTileType(S2D_ISOMETRIC);

			CTransform*	pTransform = pTileObj->GetTransform();

			pTransform->SetWorldScale(vTileScale);

			Vector3	vPos;

			float	fOffset = 0.f;

			if (i % 2 == 1)
				fOffset = vTileScale.x / 2.f;

			vPos.x = vStart.x + fOffset + j * vTileScale.x;
			vPos.y = vStart.y + i * vTileScale.y / 2.f;

			pTransform->SetWorldPos(vPos);

			if (pFileName)
			{
				CRenderer*	pRenderer = pTileObj->AddComponent<CRenderer>("Renderer");

				pRenderer->SetMesh("TexRect");
				pRenderer->SetRenderState(ALPHA_BLEND);

				SAFE_RELEASE(pRenderer);

				CMaterial*	pMaterial = pTileObj->AddComponent<CMaterial>("Material");

				pMaterial->SetDiffuseTex(0, strKey, pFileName, strPathKey);

				SAFE_RELEASE(pMaterial);
			}

			m_vecTileObj[iIndex] = pTileObj;
			m_vecTileTr[iIndex] = pTransform;
			m_vecTile[iIndex] = pTile;
		}
	}
}

int CStage2D::GetTileIndex(const Vector3 & vPos) const
{
	switch (m_eTileType)
	{
	case S2D_RECT:
		return GetRectTileIndex(vPos);
	case S2D_ISOMETRIC:
		return GetIsometricTileIndex(vPos);
	}

	return -1;
}

int CStage2D::GetRectTileIndex(const Vector3 & vPos) const
{
	int	x = (int)((vPos.x - m_vStartPos.x) / m_vTileScale.x);
	if (x < 0 || x >= (int)m_iNumX)
		return -1;

	int	y = (int)((vPos.y - m_vStartPos.y) / m_vTileScale.y);
	if (y < 0 || y >= (int)m_iNumY)
		return -1;

	return y * m_iNumX + x;
}

int CStage2D::GetIsometricTileIndex(const Vector3 & vPos) const
{
	int	idxX = GetIsometricTileIndexX(vPos);
	if (idxX == -1)
		return -1;

	int	idxY = GetIsometricTileIndexY(vPos);
	if (idxY == -1)
		return -1;

	return idxY * m_iNumX + idxX;
}

int CStage2D::GetIsometricTileIndexX(const Vector3 & vPos) const
{
	if (m_iNumX == 0 || m_iNumY == 0 || m_vTileScale.x == 0.f ||
		m_vTileScale.y == 0.f)
		return -1;

	int	iY = GetIsometricTileIndexY(vPos);

	if (iY < 0 || iY >= (int)m_iNumY)
		return -1;

	float	x = vPos.x - m_vStartPos.x;
	float	y = vPos.y - m_vStartPos.y;
	int	idx = -1;

	if (iY % 2 == 0)
		idx = (int)x / (int)m_vTileScale.x;

	else
		idx = ((int)x - (int)m_vTileScale.x / 2) / (int)m_vTileScale.x;

	if (idx < 0 || idx >= (int)m_iNumX)
		return -1;

	return idx;
}

int CStage2D::GetIsometricTileIndexY(const Vector3 & vPos) const
{
	if (m_iNumX == 0 || m_iNumY == 0 || m_vTileScale.x == 0.f ||
		m_vTileScale.y == 0.f)
		return -1;

	float	x = vPos.x - m_vStartPos.x;
	float	y = vPos.y - m_vStartPos.y;

	// x, y 위치를 타일의 가로, 세로 크기로 나누어서 1, 1 공간으로
	// 변환시켜준다.
	float	fX = x / m_vTileScale.x;
	float	fY = y / m_vTileScale.y;

	// 사각 타일을 기준으로 이 위치가 어느정도쯤에 있는지 비율을
	// 구해준다.
	fX -= (int)fX;
	fY -= (int)fY;

	int	iXIdx = (int)x / (int)m_vTileScale.x;
	int	iYIdx = (int)y / (int)m_vTileScale.y;

	// 현재 Y의 비율이 0.5 보다 작다면 사각형의 아래쪽 부분에
	// 존재한다는 것이다.
	if (fY < 0.5f)
	{
		// 현재 비율을 이용해서 현재 점이 왼쪽 하단 마름모 밖의
		// 삼각형 안일 경우를 구해준다.
		if (fY < 0.5f - fX)
		{
			if (iYIdx == 0)
				return -1;

			else if (iYIdx == 1)
			{
				if (iXIdx == 0)
					return -1;

				else
					return 1;
			}

			else
			{
				if (iXIdx == 0)
					return -1;

				else
					return (iYIdx - 1) * 3 - (iYIdx - 2);
			}
		}

		// 우하단 일경우
		else if (fY < fX - 0.5f)
		{
			if (iYIdx == 0)
				return -1;

			else if (iYIdx == 1)
			{
				if (iXIdx > (int)m_iNumX - 1)
					return -1;

				return 1;
			}

			else
			{
				if (iXIdx > (int)m_iNumX - 1)
					return -1;

				return (iYIdx - 1) * 3 - (iYIdx - 2);
			}
		}

		else
		{
			if (iYIdx == 0)
				return 0;

			else if (iYIdx == 1)
				return 2;

			else
				return (iYIdx - 1) * 3 - (iYIdx - 2) + 1;
		}
	}

	// 위쪽 라인일 경우
	else if (fY > 0.5f)
	{
		// 좌표지점이 좌 상단 지점일 경우
		if (fY - 0.5f > fX)
		{
			if (iYIdx == 0)
			{
				if (iXIdx == 0)
					return -1;

				else
					return 1;
			}

			else if (iYIdx == 1)
			{
				if (iXIdx == 0)
					return -1;

				else
					return 3;
			}

			else
			{
				if (iXIdx == 0)
					return -1;

				return (iYIdx - 1) * 3 - (iYIdx - 2) + 2;
			}
		}

		// 좌표지점이 우 상단 지점일 경우
		else if (fY - 0.5f > 1.f - fX)
		{
			if (iYIdx == 0)
			{
				if (iXIdx > (int)m_iNumX - 1)
					return -1;

				else
					return 1;
			}

			else if (iYIdx == 1)
			{
				if (iXIdx > (int)m_iNumX - 1)
					return -1;

				else
					return 3;
			}

			else
			{
				if (iXIdx > (int)m_iNumX - 1)
					return -1;

				return (iYIdx - 1) * 3 - (iYIdx - 2) + 2;
			}
		}

		else
		{
			if (iYIdx == 0)
				return 0;

			else if (iYIdx == 1)
				return 2;

			else
				return (iYIdx - 1) * 3 - (iYIdx - 2) + 1;
		}
	}

	else
	{
		if (iYIdx == 0)
			return 0;

		else if (iYIdx == 1)
			return 2;

		else
			return (iYIdx - 1) * 3 - (iYIdx - 2) + 1;
	}

	return -1;
}

void CStage2D::Start()
{
}

void CStage2D::AfterClone()
{
}

bool CStage2D::Init()
{
	return true;
}

int CStage2D::Input(float fTime)
{
	return 0;
}

int CStage2D::Update(float fTime)
{
	CTransform*	pMainCameraTr = m_pScene->GetMainCameraTransform();

	Vector3	vCameraPos = pMainCameraTr->GetWorldPos();

	SAFE_RELEASE(pMainCameraTr);

	vCameraPos -= m_vStartPos;		
	/*
	vCameraPos.x -= _RESOLUTION.iWidth;
	vCameraPos.y -= _RESOLUTION.iHeight;*/		
	
	Vector3	vEndPos = vCameraPos;
	vEndPos.x += _RESOLUTION.iWidth;
	vEndPos.y += _RESOLUTION.iHeight;

	if (vCameraPos.x + _RESOLUTION.iWidth / 2.f < 0)
		vEndPos.x = (float)_RESOLUTION.iWidth;//_RESOLUTION.iWidth;
	else if (vCameraPos.x + (float)_RESOLUTION.iWidth >= vEndPos.x)
		vCameraPos.x = m_vWorldSize.x/*m_vWorldSize.x*/ - (float)_RESOLUTION.iWidth;

	if (vCameraPos.y + (float)_RESOLUTION.iHeight / 2 < 0)
		vEndPos.y = (float)_RESOLUTION.iHeight;//_RESOLUTION.iHeight;//_RESOLUTION.iHeight;1200.f;
	else if (vCameraPos.y + (float)_RESOLUTION.iHeight >= vEndPos.y)
		vCameraPos.y = m_vWorldSize.y - (float)_RESOLUTION.iHeight;
	vCameraPos = m_vStartPos;

	vCameraPos /= m_vTileScale;
	vEndPos /= m_vTileScale;

	m_iStartX = (int)vCameraPos.x;
	if (m_iStartX < 0)
		m_iStartX = 0;

	else if (m_iStartX >= (int)m_iNumX)
		m_iStartX = m_iNumX - 1;

	m_iStartY = (int)vCameraPos.y;
	if (m_iStartY < 0)
		m_iStartY = 0;

	else if (m_iStartY >= (int)m_iNumY)
		m_iStartY = m_iNumY - 1;

	m_iEndX = (int)vEndPos.x + 1;
	if (m_iEndX < 0)
		m_iEndX = 0;

	else if (m_iEndX >= (int)m_iNumX)
		m_iEndX = m_iNumX - 1;

	m_iEndY = (int)vEndPos.y * 2 + 1;
	if (m_iEndY < 0)
		m_iEndY = 0;

	else if (m_iEndY >= (int)m_iNumY)
		m_iEndY = m_iNumY - 1;

	for (int i = m_iStartY; i <= m_iEndY; ++i)
	{
		for (int j = m_iStartX; j <= m_iEndX; ++j)
		{
			m_vecTileObj[i * m_iNumX + j]->Update(fTime);
		}
	}

	return 0;
}

int CStage2D::LateUpdate(float fTime)
{
	for (int i = m_iStartY; i < m_iEndY; ++i)
	{
		for (int j = m_iStartX; j < m_iEndX; ++j)
		{
			m_vecTileObj[i * m_iNumX + j]->LateUpdate(fTime);
		}
	}

	return 0;
}

void CStage2D::Collision(float fTime)
{
	for (int i = m_iStartY; i < m_iEndY; ++i)
	{
		for (int j = m_iStartX; j < m_iEndX; ++j)
		{
			m_vecTileObj[i * m_iNumX + j]->Collision(fTime);
		}
	}
}

void CStage2D::Render(float fTime)
{
	for (int i = m_iStartY; i < m_iEndY; ++i)
	{
		for (int j = m_iStartX; j < m_iEndX; ++j)
		{
			m_vecTileObj[i * m_iNumX + j]->Render(fTime);
		}
	}
}

CStage2D * CStage2D::Clone()
{
	return new CStage2D(*this);
}

void CStage2D::Save(FILE * pFile)
{
	CComponent::Save(pFile);	

	// 타일 수 저장
	fwrite(&m_iNumX, sizeof(UINT), 1, pFile);
	fwrite(&m_iNumY, sizeof(UINT), 1, pFile);

	// 위치와 크기 저장
	fwrite(&m_vTileScale, sizeof(Vector3), 1, pFile);
	fwrite(&m_vStartPos, sizeof(Vector3), 1, pFile);

	// 타일 타입 저장
	fwrite(&m_eTileType, sizeof(STAGE2D_TILE_TYPE), 1, pFile);

	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		// 타일의 크기와 위치를 저장한다.
		Vector3	vScale = m_vecTileTr[i]->GetWorldScale();
		Vector3	vPos = m_vecTileTr[i]->GetWorldPos();

		fwrite(&vScale, sizeof(Vector3), 1, pFile);
		fwrite(&vPos, sizeof(Vector3), 1, pFile);

		m_vecTile[i]->Save(pFile);
	}

}

void CStage2D::Load(FILE * pFile)
{
	CComponent::Load(pFile);

	// 타일 수 저장
	fread(&m_iNumX, sizeof(UINT), 1, pFile);
	fread(&m_iNumY, sizeof(UINT), 1, pFile);

	// 위치와 크기 저장
	fread(&m_vTileScale, sizeof(Vector3), 1, pFile);
	fread(&m_vStartPos, sizeof(Vector3), 1, pFile);
	/*fread(&m_iStartX, sizeof(int), 1, pFile);
	fread(&m_iStartY, sizeof(int), 1, pFile);
	fread(&m_iEndX, sizeof(int), 1, pFile);
	fread(&m_iEndY, sizeof(int), 1, pFile);*/

	// 타일 타입 저장
	fread(&m_eTileType, sizeof(STAGE2D_TILE_TYPE), 1, pFile);

	Safe_Release_VecList(m_vecTileObj);
	Safe_Release_VecList(m_vecTile);
	Safe_Release_VecList(m_vecTileTr);

	m_vecTileObj.resize(m_iNumX * m_iNumY);
	m_vecTile.resize(m_iNumX * m_iNumY);
	m_vecTileTr.resize(m_iNumX * m_iNumY);

	for (int i = 0; i < (int)m_iNumY; ++i)
	{
		for (int j = 0; j < (int)m_iNumX; ++j)
		{
			int	iIndex = i * m_iNumX + j;

			CGameObject*	pTileObj = CGameObject::CreateObject("TileObj");

			pTileObj->SetScene(m_pScene);
			pTileObj->SetLayer(m_pLayer);

			CTransform*	pTransform = pTileObj->GetTransform();

			// 타일의 크기와 위치를 저장한다.
			Vector3	vScale;
			Vector3	vPos;

			fread(&vScale, sizeof(Vector3), 1, pFile);
			fread(&vPos, sizeof(Vector3), 1, pFile);

			pTransform->SetWorldScale(vScale);
			pTransform->SetWorldPos(vPos);

			CTile2D*	pTile = pTileObj->AddComponent<CTile2D>("Tile");

			pTile->Load(pFile);

			m_vecTileObj[iIndex] = pTileObj;
			m_vecTileTr[iIndex] = pTransform;
			m_vecTile[iIndex] = pTile;
		}
	}
}

