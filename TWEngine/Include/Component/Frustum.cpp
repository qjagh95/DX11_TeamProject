#include "../EngineHeader.h"
#include "Frustum.h"

PUN_USING

CFrustum::CFrustum()
{
	m_vOriginPos[0] = Vector3(-1.f, 1.f, 0.f);
	m_vOriginPos[1] = Vector3(1.f, 1.f, 0.f);
	m_vOriginPos[2] = Vector3(-1.f, -1.f, 0.f);
	m_vOriginPos[3] = Vector3(1.f, -1.f, 0.f);

	m_vOriginPos[4] = Vector3(-1.f, 1.f, 1.f);
	m_vOriginPos[5] = Vector3(1.f, 1.f, 1.f);
	m_vOriginPos[6] = Vector3(-1.f, -1.f, 1.f);
	m_vOriginPos[7] = Vector3(1.f, -1.f, 1.f);
}

CFrustum::~CFrustum()
{
}

void CFrustum::Update(Matrix matVP)
{
	// 역행렬을 구한다.
	matVP = matVP.Inverse();

	// 8개의 투영공간 위치를 월드공간으로 변환한다.
	for (int i = 0; i < 8; ++i)
	{
		m_vPos[i] = m_vOriginPos[i].TransformCoord(matVP);
	}

	// 월드공간의 8개 정점을 이용해서 육면체를 구성하는 6개 평면을
	// 만든다.
	// Left
	m_vPlane[PD_LEFT] = XMPlaneFromPoints(m_vPos[4].Convert(),
		m_vPos[0].Convert(), m_vPos[2].Convert());

	// Right
	m_vPlane[PD_RIGHT] = XMPlaneFromPoints(m_vPos[1].Convert(),
		m_vPos[5].Convert(), m_vPos[7].Convert());

	// Top
	m_vPlane[PD_TOP] = XMPlaneFromPoints(m_vPos[4].Convert(),
		m_vPos[5].Convert(), m_vPos[1].Convert());

	// Bottom
	m_vPlane[PD_BOTTOM] = XMPlaneFromPoints(m_vPos[2].Convert(),
		m_vPos[3].Convert(), m_vPos[7].Convert());

	// Near
	m_vPlane[PD_NEAR] = XMPlaneFromPoints(m_vPos[0].Convert(),
		m_vPos[1].Convert(), m_vPos[3].Convert());

	// Far
	m_vPlane[PD_FAR] = XMPlaneFromPoints(m_vPos[5].Convert(),
		m_vPos[4].Convert(), m_vPos[6].Convert());
}

bool CFrustum::FrustumInPoint(Vector3 & vPos)
{
	for (int i = 0; i < PD_END; ++i)
	{
		float fDist = XMVectorGetX(XMPlaneDotCoord(m_vPlane[i].Convert(), vPos.Convert()));
		if (fDist > 0.f)
			return true;
	}

	return false;
}

bool CFrustum::FrustumInSphere(Vector3& vCenter, float fRadius)
{
	for (int i = 0; i < PD_END; ++i)
	{
		float fDist = XMVectorGetX(XMPlaneDotCoord(m_vPlane[i].Convert(), vCenter.Convert()));
		if (fDist > fRadius)
			return true;
	}

	return false;
}