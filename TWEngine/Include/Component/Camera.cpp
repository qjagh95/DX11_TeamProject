#include "EngineHeader.h"
#include "Camera.h"
#include "Device.h"
#include "Frustum.h"
#include "Transform.h"
#include "Core.h"
#include "Light.h"
#include "../Scene/Layer.h"
#include "../GameObject.h"

PUN_USING

CCamera::CCamera()
{
	m_eComType = CT_CAMERA;
	m_eCameraType = CT_PERSPECTIVE;
	m_pTarget = nullptr;
	m_pFrustum = new CFrustum;

	m_bShadow = false;
	m_bFrustum = true;
	m_pShadowLight = NULLPTR;
}

CCamera::CCamera(const CCamera & camera) :
	CComponent(camera)
{
	m_eCameraType = camera.m_eCameraType;
	m_matView = camera.m_matView;
	m_matProj = camera.m_matProj;
	m_pTarget = nullptr;
	m_pFrustum = new CFrustum;

	m_bShadow = camera.m_bShadow;
	m_pShadowLight = NULLPTR;
}

CCamera::~CCamera()
{
	SAFE_RELEASE(m_pShadowLight);
	SAFE_DELETE(m_pFrustum);
	SAFE_RELEASE(m_pTarget);
}

void CCamera::SetTarget(CGameObject * pTarget)
{
	SAFE_RELEASE(m_pTarget);
	m_pTarget = pTarget->GetTransform();
}

void CCamera::SetTarget(CComponent * pTarget)
{
	SAFE_RELEASE(m_pTarget);
	m_pTarget = pTarget->GetTransform();
}

void CCamera::SetLightLayer(CLayer * pLayer)
{
	m_pLightLayer = pLayer;
}

Matrix CCamera::GetViewMatrix()	const
{
	return m_matView;
}

Matrix CCamera::GetProjMatrix() const
{
	return m_matProj;
}

void CCamera::SetWidth(float fWidth)
{
	m_fWidth = fWidth;
	SetCameraType(m_eCameraType);
}

void CCamera::SetHeight(float fHeight)
{
	m_fHeight = fHeight;
	SetCameraType(m_eCameraType);
}

void CCamera::SetViewAngle(float fAngle)
{
	m_fViewAngle = fAngle;
	SetCameraType(m_eCameraType);
}

void CCamera::SetNear(float fNear)
{
	m_fNear = fNear;
	SetCameraType(m_eCameraType);
}

void CCamera::SetFar(float fFar)
{
	m_fFar = fFar;
	SetCameraType(m_eCameraType);
}

void CCamera::SetCameraType(CAMERA_TYPE eType)
{
	m_eCameraType = eType;

	float	fHalfWidth = SHADOW_WIDTH / 2.f;
	float	fHalfHeight = SHADOW_HEIGHT / 2.f;

	switch (eType)
	{
	case CT_PERSPECTIVE:
		m_matShadowProj = XMMatrixPerspectiveFovLH(DegreeToRadian(m_fViewAngle), m_fWidth / m_fHeight, m_fNear, m_fFar);
		m_matProj = XMMatrixPerspectiveFovLH(DegreeToRadian(m_fViewAngle), m_fWidth / m_fHeight, m_fNear, m_fFar);

		break;
	case CT_ORTHO:
		/*
		0, 0.5, 0, 1		100 0   0 0
		0.5, -0.5, 0, 1		0   100 0 0
		-0.5, -0.5, 0, 1	0   0   1 0
							100 100 0 1
		100, 150, 0, 1		1/400 0      0 0
		150, 50, 0, 1		0     1/-300 0 0
		50, 50, 0, 1		0     0      1 0
							-1    1      1 1
		-0.75, 0.5
		-0.625, 0.83
		2/(r-l)      0            0           0
		0            2/(t-b)      0           0
		0            0            1/(zf-zn)   0
		(l+r)/(l-r)  (t+b)/(b-t)  zn/(zn-zf)  l
		*/
		m_matProj = XMMatrixOrthographicOffCenterLH(0.f, m_fWidth,
			0.f, m_fHeight, 0.0f, m_fFar);
		break;
	}
}

void CCamera::SetCameraInfo(CAMERA_TYPE eType, float fWidth, float fHeight,
	float fViewAngle, float fNear, float fFar)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_fViewAngle = fViewAngle;
	m_fNear = fNear;
	m_fFar = fFar;

	SetCameraType(eType);
}

bool CCamera::Init()
{
	return true;
}

int CCamera::Input(float fTime)
{
	return 0;
}

int CCamera::Update(float fTime)
{
	m_matView.Identity();

	if (m_pTarget)
	{
		//m_pTransform->SetWorldPos(m_pTarget->GetWorldPos());
		Vector3	vMove = m_pTarget->GetWorldMove();

		//m_pTransform->SetWorldRot(m_pTarget->GetWorldRot());

		if (vMove != Vector3::Zero)
		{
			m_pTransform->Move(vMove);
		}
	}

	Vector3	vPos = m_pTransform->GetWorldPos() + m_pTransform->GetLocalPos();
	/*
	Xx Xy Xz 0
	Yx Yy Yz 0
	Zx Zy Zz 0
	0  0  0  1
	*/
	for (int i = 0; i < AXIS_END; ++i)
	{
		memcpy(&m_matView[i][0], &m_pTransform->GetWorldAxis((AXIS)i), sizeof(Vector3));
	}

	/*
	Xx Yx Zx 0
	Xy Yy Zy 0
	Xz Yz Zz 0
	0  0  0  1
	*/
	m_matView.Transpose();

	vPos *= -1.f;

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_matView[3][i] = vPos.Dot(m_pTransform->GetWorldAxis((AXIS)i));
	}

	m_matVP = m_matView * m_matProj;

	if (CCore::GetInst()->m_bEditorMode == false)
		m_pFrustum->Update(m_matView * m_matProj);

	//if (m_bShadow)
	//{
	//	m_matShadowView.Identity();

	//	// 조명의 3개 축을 얻어온다.
	//	Vector3	vLightAxis[AXIS_END];

	//	for (int i = 0; i < AXIS_END; ++i)
	//	{
	//		vLightAxis[i] = m_pShadowLight->GetWorldAxis((AXIS)i);
	//		vLightAxis[i].Normalize();
	//	}

	//	Vector3	vLightPos;

	//	if (m_pTarget)
	//		vLightPos = m_pTarget->GetWorldPos() - vLightAxis[AXIS_Z] * 20.f;
	//	else
	//		vLightPos = m_pTransform->GetWorldPos() + m_pTransform->GetWorldAxis(AXIS_Z) * 5.f - vLightAxis[AXIS_Z] * 30.f;

	//	for (int i = 0; i < AXIS_END; ++i)
	//		memcpy(&m_matShadowView[i][0], &vLightAxis[i], sizeof(Vector3));

	//	m_matShadowView.Transpose();

	//	vLightPos *= -1.f;

	//	for (int i = 0; i < AXIS_END; ++i)
	//		m_matShadowView[3][i] = vLightPos.Dot(vLightAxis[i]);
	//}

	return 0;
}

int CCamera::LateUpdate(float fTime)
{
	//const list<CGameObject*>* pObjList = m_pLightLayer->GetObjectList();

	//list<CGameObject*>::const_iterator iter;
	//list<CGameObject*>::const_iterator iterEnd = pObjList->end();

	//list<CGameObject*> pList;

	//pList.clear();

	//for (iter = pObjList->begin(); iter != iterEnd; ++iter)
	//{
	//	if (!(*iter)->GetEnable())
	//		continue;

	//	pList.push_back((*iter));
	//}

	//if (pList.empty())
	//{
	//	if (m_bShadow)
	//		m_bShadow = false;
	//	return 0;
	//}

	//CTransform* pTr = nullptr;
	//CLight* pLight = nullptr;
	//float fLength = 1000000.0f;
	//Vector3 vDist;
	//float fDist = 0.0f;

	//list<CGameObject*>::iterator iter1;
	//list<CGameObject*>::iterator iter1End = pList.end();
	//list<CGameObject*>::iterator iter1Find;

	//for (iter1 = pList.begin(); iter1 != iter1End;)
	//{
	//	pTr = (*iter1)->GetTransformNonCount();

	//	vDist = pTr->GetWorldPos() - m_pTransform->GetWorldPos();
	//	fDist = vDist.Length();

	//	if (fDist < fLength)
	//	{
	//		SAFE_RELEASE(pLight);
	//		pLight = (*iter1)->FindComponentFromType<CLight>(CT_LIGHT);

	//		if (pLight->GetLightType() != LT_SPOT)
	//		{
	//			SAFE_RELEASE(pLight);
	//			iter1 = pList.erase(iter1);
	//			if (pList.empty())
	//				break;

	//			continue;
	//		}

	//		fLength = fDist;
	//		iter1Find = iter1;
	//	}

	//	++iter1;
	//}

	//if (!pLight)
	//{
	//	if(m_bShadow)
	//		m_bShadow = false;

	//	return 0;
	//}

	//if(!m_bShadow)
	//	m_bShadow = true;

	//m_matShadowVP = pLight->GetShadowVP();

	//SAFE_RELEASE(pLight);

	return 0;
}

void CCamera::Collision(float fTime)
{
}

void CCamera::Render(float fTime)
{
}

CCamera * CCamera::Clone()
{
	return new CCamera(*this);
}

bool CCamera::FrustumInPoint(Vector3 & vPos)
{
	return m_pFrustum->FrustumInPoint(vPos);
}

bool CCamera::FrustumInSphere(Vector3 & vCenter, float fRadius)
{
	return m_pFrustum->FrustumInSphere(vCenter, fRadius);
}

void CCamera::SetShadowLight(CTransform * pShadowLight)
{
	SAFE_RELEASE(m_pShadowLight);
	pShadowLight->AddRef();
	m_pShadowLight = pShadowLight;
}

void CCamera::Shadow(bool bEnable)
{
	m_bShadow = bEnable;
}

Matrix CCamera::GetShadowViewMatrix() const
{
	return m_matShadowView;
}

Matrix CCamera::GetShadowProjMatrix() const
{
	return m_matShadowProj;
}

Matrix CCamera::GetShadowVP() const
{
	return m_matShadowVP;
}

bool CCamera::IsShadow() const
{
	return false;
}

Matrix CCamera::GetVP() const
{
	return m_matVP;
}

