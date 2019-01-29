#include "stdafx.h"
#include "ColliderPixel.h"
#include "Transform.h"
#include "../Rendering/RenderManager.h"
#include "../Resource/ResourcesManager.h"
#include "Camera.h"
#include "../Scene/Scene.h"
#include "../Resource/Mesh.h"
#include "../Rendering/ShaderManager.h"
#include "ColliderPoint.h"
#include "ColliderOBB.h"
#include "ColliderRect.h"
#include "../PathManager.h"

PUN_USING

CColliderPixel::CColliderPixel()
{
	m_eCollType = CT_PIXEL;
#ifdef _DEBUG
	m_pDepthDisable = GET_SINGLE(CRenderManager)->FindRenderState(DEPTH_DISABLE);
	m_pMesh = GET_SINGLE(CResourcesManager)->FindMesh("ColliderRect");
#endif // _DEBUG
}

CColliderPixel::CColliderPixel(const CColliderPixel & com)	:
	CCollider(com)
{
	m_tRelativeInfo = com.m_tRelativeInfo;

	m_tRelativeInfo.pPixel = new Pixel24[m_tRelativeInfo.iWidth * m_tRelativeInfo.iHeight];

	memcpy(m_tRelativeInfo.pPixel, com.m_tRelativeInfo.pPixel,
		sizeof(Pixel24) * m_tRelativeInfo.iWidth * m_tRelativeInfo.iHeight);
}

CColliderPixel::~CColliderPixel()
{
	SAFE_DELETE_ARRAY(m_tRelativeInfo.pPixel);
}

PixelInfo CColliderPixel::GetInfo() const
{
	return m_tInfo;
}

void CColliderPixel::SetInfo(const Vector3 & vMin, const Pixel24 & tPixel, 
	const char * pFileName, const string & strPathKey)
{
	const char*	pPath = GET_SINGLE(CPathManager)->FindPathFromMultibyte(strPathKey);

	string	strPath;
	if (pPath)
		strPath = pPath;

	strPath += pFileName;

	FILE*	pFile = nullptr;

	fopen_s(&pFile, strPath.c_str(), "rb");

	if (!pFile)
		return;

	BITMAPFILEHEADER	fh;
	BITMAPINFOHEADER	ih;

	fread(&fh, sizeof(fh), 1, pFile);
	fread(&ih, sizeof(ih), 1, pFile);

	int iWidth = ih.biWidth;
	int iHeight = ih.biHeight;
	m_tRelativeInfo.tYasuoPixel = tPixel;

	m_tRelativeInfo.pPixel = new Pixel24[iWidth * iHeight];

	fread(m_tRelativeInfo.pPixel, sizeof(Pixel24), iWidth * iHeight, pFile);

	fclose(pFile);

	m_tRelativeInfo.tBox.vMin = vMin;
	m_tRelativeInfo.tBox.vMax = vMin + Vector3(iWidth, iHeight, 0);
	m_tRelativeInfo.tBox.vLength = m_tRelativeInfo.tBox.vMax - vMin;

	m_tRelativeInfo.tBox.vMin.z = 0.f;
	m_tRelativeInfo.tBox.vMax.z = 0.f;
	m_tRelativeInfo.tBox.vLength = 1.f;

	m_tRelativeInfo.iWidth = iWidth;
	m_tRelativeInfo.iHeight = iHeight;

	m_tInfo = m_tRelativeInfo;
}

bool CColliderPixel::Init()
{
	return true;
}

int CColliderPixel::Input(float fTime)
{
	return 0;
}

int CColliderPixel::Update(float fTime)
{
	return 0;
}

int CColliderPixel::LateUpdate(float fTime)
{
	Vector3	vPos = m_pTransform->GetWorldPos() - m_pTransform->GetPivot() *
		m_pTransform->GetWorldScale();

	m_tInfo.tBox.vMin = vPos + m_tRelativeInfo.tBox.vMin +
		m_vPivot * m_tRelativeInfo.tBox.vLength;
	m_tInfo.tBox.vMax = vPos + m_tRelativeInfo.tBox.vMax +
		m_vPivot * m_tRelativeInfo.tBox.vLength;
	m_tInfo.tBox.vLength = m_tRelativeInfo.tBox.vLength;

	// 이 정보 자체가 사각형의 영역을 판단할 데이터로 사용된다.
	m_vSectionMin = m_tInfo.tBox.vMin;
	m_vSectionMax = m_tInfo.tBox.vMax;

	return 0;
}

void CColliderPixel::Collision(float fTime)
{
}

void CColliderPixel::Render(float fTime)
{
#ifdef _DEBUG
	Matrix	matPos, matScale;
	matPos.Translation(m_tInfo.tBox.vMin);
	matScale.Scaling(m_tInfo.tBox.vLength);

	TransformCBuffer	tCBuffer = {};

	CCamera*	pMainCamera = m_pScene->GetMainCamera();

	Matrix	matView;

	if (m_strCollisionGroup != "UI")
		matView = pMainCamera->GetViewMatrix();

	tCBuffer.matWorld = matScale * matPos;
	tCBuffer.matView = matView;
	tCBuffer.matProj = pMainCamera->GetProjMatrix();
	tCBuffer.matWV = tCBuffer.matWorld * tCBuffer.matView;
	tCBuffer.matWVP = tCBuffer.matWV * tCBuffer.matProj;
	tCBuffer.vPivot = m_vPivot;
	tCBuffer.vLength = m_pMesh->GetLength();

	tCBuffer.matWorld.Transpose();
	tCBuffer.matView.Transpose();
	tCBuffer.matProj.Transpose();
	tCBuffer.matWV.Transpose();
	tCBuffer.matWVP.Transpose();

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform",
		&tCBuffer);

	SAFE_RELEASE(pMainCamera);

	CCollider::Render(fTime);
#endif // _DEBUG
}

CColliderPixel * CColliderPixel::Clone()
{
	return new CColliderPixel(*this);
}

bool CColliderPixel::Collision(CCollider * pDest, float fTime)
{
	switch (pDest->GetColliderType())
	{
	case CT_RECT:
		return CollisionRectToPixel(((CColliderRect*)pDest)->GetInfo(), m_tInfo);
	}

	return false;
}
