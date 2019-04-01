#include "../EngineHeader.h"
#include "ColliderRay.h"
#include "Camera.h"
#include "Transform.h"
#include "ColliderOBB.h"
#include "ColliderPoint.h"
#include "ColliderPixel.h"
#include "ColliderSphere.h"
#include "../Input.h"
#include "../Device.h"
#include "../Scene/Scene.h"
#include "../Resource/Mesh.h"
#include "../Resource/ResourcesManager.h"
#include "../Rendering/RenderManager.h"
#include "../Rendering/ShaderManager.h"

PUN_USING

CColliderRay::CColliderRay()
{
	m_eCollType = CT_RAY;
	m_bMouseEnable = false;
#ifdef _DEBUG
	//m_pWireFrame = GET_SINGLE(CRenderManager)->FindRenderState(WIREFRAME);
	//m_pMesh = GET_SINGLE(CResourcesManager)->FindMesh("Sky");
#endif // _DEBUG
}

CColliderRay::CColliderRay(const CColliderRay & com) :
	CCollider(com)
{
	m_tRelativeInfo = com.m_tRelativeInfo;
	m_tInfo = com.m_tInfo;
	m_bMouseEnable = false;
}

CColliderRay::~CColliderRay()
{
}

RayInfo CColliderRay::GetInfo() const
{
	return m_tInfo;
}

void CColliderRay::SetInfo(const Vector3& vPos, const Vector3& vDir,
	float fDist)
{
	m_tRelativeInfo.vPos = vPos;
	m_tRelativeInfo.vDir = vDir;
	m_tRelativeInfo.fDist = fDist;

	m_tInfo.fDist = fDist;
}

void CColliderRay::MouseEnable()
{
	m_bMouseEnable = true;
}

bool CColliderRay::Init()
{
	return true;
}

int CColliderRay::Input(float fTime)
{
	return 0;
}

int CColliderRay::Update(float fTime)
{
	return 0;
}

int CColliderRay::LateUpdate(float fTime)
{
	if (m_bMouseEnable)
	{
		Vector2	vMousePos = GET_SINGLE(CInput)->GetMouse3DClient();

		// 카메라를 얻어온다.
		CCamera*	pCamera = m_pScene->GetMainCamera();

		Matrix	matView, matProj;
		matView = pCamera->GetViewMatrix();
		matProj = pCamera->GetProjMatrix();

		SAFE_RELEASE(pCamera);

		// 마우스 위치를 뷰공간으로 변환한다.
		float	fHalfWidth = _RESOLUTION.iWidth / 2.f;
		float	fHalfHeight = _RESOLUTION.iHeight / 2.f;
		Vector3	vDir;
		vDir.x = (vMousePos.x / fHalfWidth - 1.f) / matProj._11;
		vDir.y = (vMousePos.y / -fHalfHeight + 1.f) / matProj._22;
		vDir.z = 1.f;

		vDir.Normalize();

		Vector3	vPos = Vector3::Zero;

		// 월드공간으로 변환한다.
		matView.Inverse();
		vDir = vDir.TransformNormal(matView);
		vPos = vPos.TransformCoord(matView);

		vDir.Normalize();

		m_tInfo.vPos = vPos;
		m_tInfo.vDir = vDir;
		m_tInfo.fDist = -1.f;
	}

	else
	{
		Vector3	vPos = m_pTransform->GetWorldPos() - m_pTransform->GetPivot() *
			m_pTransform->GetWorldScale();

		m_tInfo.vPos = vPos + m_tRelativeInfo.vPos;
	}

	Vector3	vStart = m_tInfo.vPos;
	Vector3	vEnd;

	if (m_tInfo.fDist == -1.f)
		vEnd = m_tInfo.vPos + m_tInfo.vDir * 1000.f;

	else
		vEnd = m_tInfo.vPos + m_tInfo.vDir * m_tInfo.fDist;

	if (vStart.x < vEnd.x)
	{
		m_vSectionMin.x = vStart.x;
		m_vSectionMax.x = vEnd.x;
	}

	else
	{
		m_vSectionMin.x = vEnd.x;
		m_vSectionMax.x = vStart.x;
	}

	if (vStart.y < vEnd.y)
	{
		m_vSectionMin.y = vStart.y;
		m_vSectionMax.y = vEnd.y;
	}

	else
	{
		m_vSectionMin.y = vEnd.y;
		m_vSectionMax.y = vStart.y;
	}

	if (vStart.z < vEnd.z)
	{
		m_vSectionMin.z = vStart.z;
		m_vSectionMax.z = vEnd.z;
	}

	else
	{
		m_vSectionMin.z = vEnd.z;
		m_vSectionMax.z = vStart.z;
	}

	return 0;
}

void CColliderRay::Collision(float fTime)
{
}

void CColliderRay::Render(float fTime)
{
#ifdef _DEBUG
	/*Matrix	matPos, matScale;
	matPos.Translation(m_tInfo.vCenter);
	matScale.Scaling(m_tInfo.fRadius);

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

	CCollider::Render(fTime);*/
#endif // _DEBUG

}

CColliderRay * CColliderRay::Clone()
{
	return new CColliderRay(*this);
}

bool CColliderRay::Collision(CCollider * pDest, float fTime)
{
	switch (pDest->GetColliderType())
	{
	case CT_SPHERE:
		return CollisionRayToSphere(m_tInfo, ((CColliderSphere*)pDest)->GetInfo());
	}

	return false;
}
