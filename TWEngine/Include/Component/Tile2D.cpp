#include "stdafx.h"
#include "Tile2D.h"
#include "../Resource/ResourcesManager.h"
#include "../Resource/Mesh.h"
#include "../Rendering/ShaderManager.h"
#include "../Rendering/Shader.h"
#include "Camera.h"
#include "../Scene/Scene.h"
#include "Transform.h"
#include "../Device.h"

PUN_USING

CTile2D::CTile2D()
{
	m_eTileOption = T2D_NORMAL;
	m_eTileType = S2D_RECT;
	m_eComType = CT_TILE2D;
	m_eFindList = TFL_NONE;
	m_iParent = -1;
	SetTag("Tile2D");
	m_bFind = false;

	m_fG = 0.f;
	m_fH = 0.f;
	m_fTotal = 0.f;

	m_bTileLine = true;
	m_pMesh = nullptr;
	m_pShader = nullptr;

	memset(m_iAdjNode, -1, sizeof(int) * AND_END);
}

CTile2D::CTile2D(const CTile2D & tile2D) :
	CComponent(tile2D)
{
	*this = tile2D;
}

CTile2D::~CTile2D()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pShader);
}

TILE2D_OPTION CTile2D::GetTile2DOption() const
{
	return m_eTileOption;
}

STAGE2D_TILE_TYPE CTile2D::GetTileType() const
{
	return m_eTileType;
}

TILE2D_FIND_LIST CTile2D::GetTileFindList() const
{
	return m_eFindList;
}

int CTile2D::GetAdjNodeIndex(ASTAR_NODE_DIR eDir) const
{
	return m_iAdjNode[eDir];
}

float CTile2D::GetG() const
{
	return m_fG;
}

float CTile2D::GetH() const
{
	return m_fH;
}

float CTile2D::GetTotal() const
{
	return m_fTotal;
}

int CTile2D::GetIndex() const
{
	return m_iIndex;
}

int CTile2D::GetParent() const
{
	return m_iParent;
}

void CTile2D::EnableFind()
{
	m_bFind = true;
}

void CTile2D::SetTileOption(TILE2D_OPTION eOption)
{
	m_eTileOption = eOption;
}

void CTile2D::SetTileType(STAGE2D_TILE_TYPE eType)
{
	m_eTileType = eType;

	switch (eType)
	{
	case S2D_RECT:
		m_pMesh = GET_SINGLE(CResourcesManager)->FindMesh("ColliderRect");
		break;
	case S2D_ISOMETRIC:
		m_pMesh = GET_SINGLE(CResourcesManager)->FindMesh("TileIso");
		break;
	}

	m_pShader = GET_SINGLE(CShaderManager)->FindShader(COLLIDER_SHADER);
	m_pLayout = GET_SINGLE(CShaderManager)->FindInputLayout(POS_LAYOUT);
}

void CTile2D::SetTileFindList(TILE2D_FIND_LIST eTFL)
{
	m_eFindList = eTFL;
}

void CTile2D::OnTile(bool bTile)
{
	m_bTileLine = bTile;
}

void CTile2D::ClearFindSet()
{
	m_fG = m_fH = m_fTotal = 0.f;
	m_eFindList = TFL_NONE;
	m_iParent = -1;
	m_bFind = false;
}

void CTile2D::SetAdjTile(int iIndex, ASTAR_NODE_DIR eDir)
{
	m_iAdjNode[eDir] = iIndex;
}

void CTile2D::SetParent(int iParent)
{
	m_iParent = iParent;
}

void CTile2D::SetG(float fG)
{
	m_fG = fG;
}

void CTile2D::SetH(float fH)
{
	m_fH = fH;
}

void CTile2D::Total()
{
	m_fTotal = m_fG + m_fH;
}

void CTile2D::SetIndex(int iIndex)
{
	m_iIndex = iIndex;
}

void CTile2D::Start()
{
}

void CTile2D::AfterClone()
{
}

bool CTile2D::Init()
{
	return true;
}

int CTile2D::Input(float fTime)
{
	return 0;
}

int CTile2D::Update(float fTime)
{
	return 0;
}

int CTile2D::LateUpdate(float fTime)
{
	return 0;
}

void CTile2D::Collision(float fTime)
{
}

void CTile2D::Render(float fTime)
{
	if (m_bTileLine)
	{
		TransformCBuffer	tCBuffer = {};

		CCamera*	pMainCamera = m_pScene->GetMainCamera();

		tCBuffer.matWorld = m_pTransform->GetWorldMatrix();
		tCBuffer.matView = pMainCamera->GetViewMatrix();
		tCBuffer.matProj = pMainCamera->GetProjMatrix();
		tCBuffer.matWV = tCBuffer.matWorld * tCBuffer.matView;
		tCBuffer.matWVP = tCBuffer.matWV * tCBuffer.matProj;
		tCBuffer.vPivot = Vector3::Zero;
		tCBuffer.vLength = m_pMesh->GetLength();

		tCBuffer.matWorld.Transpose();
		tCBuffer.matView.Transpose();
		tCBuffer.matProj.Transpose();
		tCBuffer.matWV.Transpose();
		tCBuffer.matWVP.Transpose();

		GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform",
			&tCBuffer);

		SAFE_RELEASE(pMainCamera);

		Vector4	vColor;
		switch (m_eTileOption)
		{
		case T2D_NORMAL:
			vColor = Vector4::Green;
			break;
		case T2D_NOMOVE:
			vColor = Vector4::Red;
			break;
		}

		if (m_bFind)
			vColor = Vector4(0.f, 0.f, 1.f, 1.f);

		m_pShader->SetShader();
		CONTEXT->IASetInputLayout(m_pLayout);

		GET_SINGLE(CShaderManager)->UpdateCBuffer("Collider", &vColor);

		m_pMesh->Render();
	}
}

CTile2D * CTile2D::Clone()
{
	return new CTile2D(*this);
}

void CTile2D::Save(FILE * pFile)
{
	CComponent::Save(pFile);

	/*
	class CShader*		m_pShader;
	class CMesh*		m_pMesh;
	ID3D11InputLayout*	m_pLayout;
	*/

	fwrite(&m_eTileOption, sizeof(TILE2D_OPTION), 1, pFile);
	fwrite(&m_eTileType, sizeof(STAGE2D_TILE_TYPE), 1, pFile);
	fwrite(&m_iIndex, sizeof(float), 1, pFile);

	fwrite(&m_iParent, sizeof(int), 1, pFile);

	for (int i = 0; i < AND_END; ++i)
	{
		fwrite(&m_iAdjNode[i], sizeof(int), 1, pFile);
	}
}

void CTile2D::Load(FILE * pFile)
{
	CComponent::Load(pFile);

	fread(&m_eTileOption, sizeof(TILE2D_OPTION), 1, pFile);
	fread(&m_eTileType, sizeof(STAGE2D_TILE_TYPE), 1, pFile);
	fread(&m_iIndex, sizeof(float), 1, pFile);
	/*fread(&m_eFindList, sizeof(TILE2D_FIND_LIST), 1, pFile);
	fread(&m_fG, sizeof(float), 1, pFile);
	fread(&m_fH, sizeof(float), 1, pFile);*/
	fread(&m_iParent, sizeof(int), 1, pFile);

	for (int i = 0; i < AND_END; ++i)
	{
		fread(&m_iAdjNode[i], sizeof(int), 1, pFile);
	}

	SetTileType(m_eTileType);
	OnTile(true);
}
