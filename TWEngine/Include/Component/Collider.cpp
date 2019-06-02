#include "EngineHeader.h"
#include "Collider.h"
#include "../Device.h"
#include "../Resource/Mesh.h"
#include "../Rendering/Shader.h"
#include "../Rendering/RenderState.h"

PUN_USING

CCollider::CCollider()
{
	m_eComType = CT_COLLIDER;
	m_strCollisionGroup = "Default";
	m_bUpdateCollision = true;

	m_pShader = GET_SINGLE(CShaderManager)->FindShader(COLLIDER_SHADER);
	m_pMesh = nullptr;
	m_pLayout = GET_SINGLE(CShaderManager)->FindInputLayout(POS_LAYOUT);
	m_pDepthDisable = GET_SINGLE(CRenderManager)->FindRenderState(DEPTH_DISABLE);
	m_vColor = Vector4::Green;
	m_pWireFrame = nullptr;
	m_bCallBackFunc = false;
	m_vecStayingColliders.clear();
}


CCollider::CCollider(const CCollider & com) :
	CComponent(com)
{
	*this = com;
	m_vecStayingColliders.clear();
	m_pLayout = com.m_pLayout;
	m_pMesh = com.m_pMesh;
	m_pShader = com.m_pShader;
	m_pDepthDisable = com.m_pDepthDisable;
	m_vColor = Vector4::Green;
	m_pWireFrame = com.m_pWireFrame;

	if (m_pShader)
		m_pShader->AddRef();

	if (m_pMesh)
		m_pMesh->AddRef();

	if (m_pDepthDisable)
		m_pDepthDisable->AddRef();

	if (m_pWireFrame)
		m_pWireFrame->AddRef();
}

CCollider::~CCollider()
{
	list<CCollider*>::iterator	iter;
	list<CCollider*>::iterator	iterEnd = m_PrevCollision.end();

	for (iter = m_PrevCollision.begin(); iter != iterEnd; ++iter)
	{
		OnCollisionLeave((*iter), 0.f);
		(*iter)->OnCollisionLeave(this, 0.f);
		(*iter)->ErasePrevCollision(this);
	}

	SAFE_RELEASE(m_pWireFrame);
	SAFE_RELEASE(m_pDepthDisable);
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pMesh);
}

COLLIDER_ID CCollider::GetColliderID() const
{
	return m_eColliderID;
}

void CCollider::SetColliderID(int iID)
{
	m_eColliderID = (COLLIDER_ID)iID;
}

COLLIDER_TYPE CCollider::GetColliderType() const
{
	return m_eCollType;
}

Vector3 CCollider::GetSectionMin() const
{
	return m_vSectionMin;
}

Vector3 CCollider::GetSectionMax() const
{
	return m_vSectionMax;
}

bool CCollider::GetUpdateCollision() const
{
	return m_bUpdateCollision;
}

string CCollider::GetCollisionGroup() const
{
	return m_strCollisionGroup;
}

const list<int>* CCollider::GetColisionSection() const
{
	return &m_CollisionSectionList;
}

// 이 충돌체가 소속되어 있는 섹션 인덱스를 지워준다.
void CCollider::ClearCollisionSection()
{
	m_CollisionSectionList.clear();
}

void CCollider::AddCollisionSection(int iSection)
{
	m_CollisionSectionList.push_back(iSection);
}

void CCollider::SetCollisionGroup(const string & strCollisionGroup)
{
	m_strCollisionGroup = strCollisionGroup;
}

void CCollider::SetPivot(const Vector3 & vPivot)
{
	m_vPivot = vPivot;
}

void CCollider::SetPivot(float x, float y, float z)
{
	m_vPivot = Vector3(x, y, z);
}

bool CCollider::CheckPrevCollision(CCollider * pDest)
{
	list<CCollider*>::iterator	iter;
	list<CCollider*>::iterator	iterEnd = m_PrevCollision.end();

	for (iter = m_PrevCollision.begin(); iter != iterEnd; ++iter)
	{
		if (*iter == pDest)
			return true;
	}

	return false;
}

void CCollider::AddPrevCollision(CCollider * pDest)
{
	m_PrevCollision.push_back(pDest);
}

void CCollider::ErasePrevCollision(CCollider * pDest)
{
	list<CCollider*>::iterator	iter;
	list<CCollider*>::iterator	iterEnd = m_PrevCollision.end();

	for (iter = m_PrevCollision.begin(); iter != iterEnd; ++iter)
	{
		if (*iter == pDest)
		{
			m_PrevCollision.erase(iter);
			return;
		}
	}
}

void CCollider::CheckPrevCollisionInSection(float fTime)
{
	list<CCollider*>::iterator	iter;
	list<CCollider*>::iterator	iterEnd = m_PrevCollision.end();

	for (iter = m_PrevCollision.begin(); iter != iterEnd;)
	{
		if (m_strCollisionGroup != (*iter)->m_strCollisionGroup)
		{
			OnCollisionLeave((*iter), fTime);
			(*iter)->OnCollisionLeave(this, fTime);

			(*iter)->ErasePrevCollision(this);
			iter = m_PrevCollision.erase(iter);

			iterEnd = m_PrevCollision.end();
			continue;
		}

		const list<int>*	pSectionList = (*iter)->GetColisionSection();
		list<int>::const_iterator	iter1;
		list<int>::const_iterator	iter1End = pSectionList->end();


		list<int>::iterator	iter2;
		list<int>::iterator	iter2End = m_CollisionSectionList.end();

		bool	bPair = false;

		for (iter1 = pSectionList->begin(); iter1 != iter1End; ++iter1)
		{
			for (iter2 = m_CollisionSectionList.begin(); iter2 != iter2End; ++iter2)
			{
				if (*iter1 == *iter2)
				{
					bPair = true;
					break;
				}
			}

			if (bPair)
				break;
		}

		if (!bPair)
		{
			OnCollisionLeave((*iter), fTime);
			(*iter)->OnCollisionLeave(this, fTime);

			(*iter)->ErasePrevCollision(this);
			iter = m_PrevCollision.erase(iter);

			iterEnd = m_PrevCollision.end();
			continue;
		}

		++iter;
	}
}

bool CCollider::Init()
{
	return true;
}

int CCollider::Input(float fTime)
{
	
	return 0;
}

int CCollider::Update(float fTime)
{
	return 0;
}

int CCollider::LateUpdate(float fTime)
{
	return 0;
}

void CCollider::Collision(float fTime)
{
}

void CCollider::Render(float fTime)
{
	m_vecStayingColliders.clear();
	m_bCallBackFunc = false;

	if (CCollisionManager::GetInst()->GetIsShow() == false)
		return;

	if (m_PrevCollision.empty())
		m_vColor = Vector4::Green;

	else
		m_vColor = Vector4::Red;

	m_pShader->SetShader();
	CONTEXT->IASetInputLayout(m_pLayout);

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Collider", &m_vColor);

	if (m_pDepthDisable)
		m_pDepthDisable->SetState();

	if (m_pWireFrame)
		m_pWireFrame->SetState();

	m_pMesh->Render();

	if (m_pDepthDisable)
		m_pDepthDisable->ResetState();

	if (m_pWireFrame)
		m_pWireFrame->ResetState();
}

void CCollider::Save(BinaryWrite * _pInstBW)
{
}

void CCollider::Load(BinaryRead * _pInstBR)
{
}

void CCollider::OnCollisionEnter(CCollider * pDest, float fTime)
{
	list<function<void(CCollider*, CCollider*, float)>>::iterator	iter;
	list<function<void(CCollider*, CCollider*, float)>>::iterator	iterEnd = m_CollisionFunc[CCT_ENTER].end();

	for (iter = m_CollisionFunc[CCT_ENTER].begin(); iter != iterEnd; ++iter)
	{
		(*iter)(this, pDest, fTime);
	}
}

void CCollider::OnCollision(CCollider * pDest, float fTime)
{
	list<function<void(CCollider*, CCollider*, float)>>::iterator	iter;
	list<function<void(CCollider*, CCollider*, float)>>::iterator	iterEnd = m_CollisionFunc[CCT_STAY].end();

	for (iter = m_CollisionFunc[CCT_STAY].begin(); iter != iterEnd; ++iter)
	{
		(*iter)(this, pDest, fTime);
	}
}

void CCollider::OnCollisionLeave(CCollider * pDest, float fTime)
{
	list<function<void(CCollider*, CCollider*, float)>>::iterator	iter;
	list<function<void(CCollider*, CCollider*, float)>>::iterator	iterEnd = m_CollisionFunc[CCT_LEAVE].end();

	for (iter = m_CollisionFunc[CCT_LEAVE].begin(); iter != iterEnd; ++iter)
	{
		(*iter)(this, pDest, fTime);
	}
}

bool CCollider::CollisionRectToRect(const BoxInfo & tSrc,
	const BoxInfo & tDest)
{
	if (tSrc.vMin.x > tDest.vMax.x)
		return false;

	else if (tSrc.vMax.x < tDest.vMin.x)
		return false;

	else if (tSrc.vMin.y > tDest.vMax.y)
		return false;

	else if (tSrc.vMax.y < tDest.vMin.y)
		return false;

	return true;
}

bool CCollider::CollisionRectToPoint(const BoxInfo & tSrc, const Vector3 & vDest)
{
	if (vDest.x < tSrc.vMin.x)
		return false;

	else if (vDest.x > tSrc.vMax.x)
		return false;

	else if (vDest.y < tSrc.vMin.y)
		return false;

	else if (vDest.y > tSrc.vMax.y)
		return false;

	return true;
}

bool CCollider::CollisionOBB2DToOBB2D(const OBB2DInfo & tSrc, const OBB2DInfo & tDest)
{
	Vector3	vLength = tDest.vCenter - tSrc.vCenter;

	float	fDot[2][4];

	fDot[0][0] = abs(tSrc.vAxis[0].Dot(tDest.vAxis[0]));
	fDot[0][1] = abs(tSrc.vAxis[0].Dot(tDest.vAxis[1]));
	fDot[0][2] = abs(tSrc.vAxis[1].Dot(tDest.vAxis[0]));
	fDot[0][3] = abs(tSrc.vAxis[1].Dot(tDest.vAxis[1]));

	fDot[1][0] = abs(tDest.vAxis[0].Dot(tSrc.vAxis[0]));
	fDot[1][1] = abs(tDest.vAxis[0].Dot(tSrc.vAxis[1]));
	fDot[1][2] = abs(tDest.vAxis[1].Dot(tSrc.vAxis[0]));
	fDot[1][3] = abs(tDest.vAxis[1].Dot(tSrc.vAxis[1]));

	// 상자 A의 X축에 대한 내적처리
	float	fLength = abs(tSrc.vAxis[0].Dot(vLength));

	// 상자 B를 상자 A의 X축에 투영한다.
	float	fR1 = tSrc.fLength[0];
	float	fR2 = fDot[0][0] * tDest.fLength[0] + fDot[0][1] * tDest.fLength[1];

	if (fLength > fR1 + fR2)
		return false;

	// 상자 A의 Y축에 대한 내적처리
	fLength = abs(tSrc.vAxis[1].Dot(vLength));

	// 상자 B를 상자 A의 Y축에 투영한다.
	fR1 = tSrc.fLength[1];
	fR2 = fDot[0][2] * tDest.fLength[0] + fDot[0][3] * tDest.fLength[1];

	if (fLength > fR1 + fR2)
		return false;

	// 상자 B의 X축에 대한 내적처리
	fLength = abs(tDest.vAxis[0].Dot(vLength));

	// 상자 A를 상자 B의 X축에 투영한다.
	fR1 = tDest.fLength[0];
	fR2 = fDot[1][0] * tSrc.fLength[0] + fDot[1][1] * tSrc.fLength[1];

	if (fLength > fR1 + fR2)
		return false;

	// 상자 B의 Y축에 대한 내적처리
	fLength = abs(tDest.vAxis[1].Dot(vLength));

	// 상자 A를 상자 B의 Y축에 투영한다.
	fR1 = tDest.fLength[1];
	fR2 = fDot[1][2] * tSrc.fLength[0] + fDot[1][3] * tSrc.fLength[1];

	if (fLength > fR1 + fR2)
		return false;

	return true;
}

bool CCollider::CollisionOBB2DToRect(const OBB2DInfo & tSrc, const BoxInfo & tDest)
{
	OBB2DInfo	tInfo;
	tInfo.vCenter = (tDest.vMin + tDest.vMax) / 2.f;
	tInfo.vAxis[0] = Vector3::Axis[0];
	tInfo.vAxis[1] = Vector3::Axis[1];
	tInfo.fLength[0] = tDest.vLength.x / 2.f;
	tInfo.fLength[1] = tDest.vLength.y / 2.f;
	return CollisionOBB2DToOBB2D(tSrc, tInfo);
}

bool CCollider::CollisionOBB2DToPoint(const OBB2DInfo & tSrc, const Vector3 & vDest)
{
	// OBB 상자가 Z축으로 회전한 만큼 반대로 점을 OBB상자의 중점을 기준으로
	// 회전시킨 좌표를 구한다. 이렇게 할 경우 이 점을 OBB상자를 월드축에 정렬된
	// 일반 RECT로 만들었을 경우의 점 위치가 나오게 되기 때문에
	// Pt In Rect를 이용해서 처리가 가능하다.
	// 회전각도를 구한다.
	// 회전 각도를 구한다. 회전은 2D 이기 때문에 Z축 회전을 한다.
	float	fAngle = tSrc.vAxis[0].Angle(Vector3(1.f, 0.f, 0.f));

	if (tSrc.vAxis[0].y < 0.f)
		fAngle = 360.f - fAngle;

	Matrix	matRot;
	matRot.RotationZ(-fAngle);
	memcpy(&matRot._41, &tSrc.vCenter, sizeof(Vector3));

	Vector3	vPos = vDest - tSrc.vCenter;

	vPos = vPos.TransformCoord(matRot);

	BoxInfo	tInfo;
	tInfo.vMin = tSrc.vCenter - Vector3(tSrc.fLength[0], tSrc.fLength[1], 0.f);
	tInfo.vMax = tSrc.vCenter + Vector3(tSrc.fLength[0], tSrc.fLength[1], 0.f);

	return CollisionRectToPoint(tInfo, vPos);
}

bool CCollider::CollisionRectToPixel(BoxInfo tSrc, const PixelInfo & tDest)
{
	if (!CollisionRectToRect(tSrc, tDest.tBox))
		return false;

	// Box를 픽셀공간으로 변환한다.
	tSrc.vMin -= tDest.tBox.vMin;
	tSrc.vMax -= tDest.tBox.vMin;

	tSrc.vMin.x = tSrc.vMin.x < 0.f ? 0.f : tSrc.vMin.x;
	tSrc.vMin.y = tSrc.vMin.y < 0.f ? 0.f : tSrc.vMin.y;

	tSrc.vMax.x = tSrc.vMax.x > tDest.iWidth ? tDest.iWidth : tSrc.vMax.x;
	tSrc.vMax.y = tSrc.vMax.y > tDest.iHeight ? tDest.iHeight : tSrc.vMax.y;

	for (int i = (int)tSrc.vMin.y; i < (int)tSrc.vMax.y; ++i)
	{
		for (int j = (int)tSrc.vMin.x; j < (int)tSrc.vMax.x; ++j)
		{
			int	iIndex = i * tDest.iWidth + j;

			Pixel24&	tPixel = tDest.pPixel[iIndex];

			if (tPixel.r != 0 || tPixel.g != 0 || tPixel.b != 0)
			{
				return true;
			}
		}
	}

	return false;
}

bool CCollider::CollisionSphereToSphere(SphereInfo tSrc, const SphereInfo & tDest)
{
	float fDist = tSrc.vCenter.Distance(tDest.vCenter);

	return fDist <= tSrc.fRadius + tDest.fRadius;
}

bool CCollider::CollisionOBB3DToOBB3D(const OBB3DInfo & tSrc, const OBB3DInfo & tDest)
{
	Vector3	vDir = tDest.vCenter - tSrc.vCenter;

	float	fAxisDot[3][3] = {};
	float	fAbsAxisDot[3][3] = {};
	float	fDirDot[3] = {};
	float	r, r1, r2;
	bool	bAxis = false;

	for (int i = 0; i < 3; ++i)
	{
		fAxisDot[AXIS_X][i] = tSrc.vAxis[AXIS_X].Dot(tDest.vAxis[i]);
		fAbsAxisDot[AXIS_X][i] = abs(fAxisDot[AXIS_X][i]);

		if (fAbsAxisDot[AXIS_X][i] > 0.99999f)
			bAxis = true;
	}

	// Center Dir을 Src의 X축에 내적한다.
	fDirDot[0] = vDir.Dot(tSrc.vAxis[AXIS_X]);
	r = abs(fDirDot[0]);
	r1 = tSrc.vLength.x;
	r2 = tDest.vLength.x * fAbsAxisDot[AXIS_X][AXIS_X] +
		tDest.vLength.y * fAbsAxisDot[AXIS_X][AXIS_Y] +
		tDest.vLength.z * fAbsAxisDot[AXIS_X][AXIS_Z];

	if (r > r1 + r2)
		return false;

	for (int i = 0; i < 3; ++i)
	{
		fAxisDot[AXIS_Y][i] = tSrc.vAxis[AXIS_Y].Dot(tDest.vAxis[i]);
		fAbsAxisDot[AXIS_Y][i] = abs(fAxisDot[AXIS_Y][i]);

		if (fAbsAxisDot[AXIS_Y][i] > 0.99999f)
			bAxis = true;
	}

	// Center Dir을 Src의 Y축에 내적한다.
	fDirDot[1] = vDir.Dot(tSrc.vAxis[AXIS_Y]);
	r = abs(fDirDot[1]);
	r1 = tSrc.vLength.y;
	r2 = tDest.vLength.x * fAbsAxisDot[AXIS_Y][AXIS_X] +
		tDest.vLength.y * fAbsAxisDot[AXIS_Y][AXIS_Y] +
		tDest.vLength.z * fAbsAxisDot[AXIS_Y][AXIS_Z];

	if (r > r1 + r2)
		return false;

	for (int i = 0; i < 3; ++i)
	{
		fAxisDot[AXIS_Z][i] = tSrc.vAxis[AXIS_Z].Dot(tDest.vAxis[i]);
		fAbsAxisDot[AXIS_Z][i] = abs(fAxisDot[AXIS_Z][i]);

		if (fAbsAxisDot[AXIS_Z][i] > 0.99999f)
			bAxis = true;
	}

	// Center Dir을 Src의 Z축에 내적한다.
	fDirDot[2] = vDir.Dot(tSrc.vAxis[AXIS_Z]);
	r = abs(fDirDot[2]);
	r1 = tSrc.vLength.z;
	r2 = tDest.vLength.x * fAbsAxisDot[AXIS_Z][AXIS_X] +
		tDest.vLength.y * fAbsAxisDot[AXIS_Z][AXIS_Y] +
		tDest.vLength.z * fAbsAxisDot[AXIS_Z][AXIS_Z];

	if (r > r1 + r2)
		return false;

	// Dest의 X축에 Center Dir을 내적한다.
	r = abs(vDir.Dot(tDest.vAxis[AXIS_X]));
	r1 = tSrc.vLength.x * fAbsAxisDot[AXIS_X][AXIS_X] +
		tSrc.vLength.y * fAbsAxisDot[AXIS_Y][AXIS_X] +
		tSrc.vLength.z * fAbsAxisDot[AXIS_Z][AXIS_X];
	r2 = tDest.vLength.x;

	if (r > r1 + r2)
		return false;

	// Dest의 Y축에 Center Dir을 내적한다.
	r = abs(vDir.Dot(tDest.vAxis[AXIS_Y]));
	r1 = tSrc.vLength.x * fAbsAxisDot[AXIS_X][AXIS_Y] +
		tSrc.vLength.y * fAbsAxisDot[AXIS_Y][AXIS_Y] +
		tSrc.vLength.z * fAbsAxisDot[AXIS_Z][AXIS_Y];
	r2 = tDest.vLength.y;

	if (r > r1 + r2)
		return false;

	// Dest의 Z축에 Center Dir을 내적한다.
	r = abs(vDir.Dot(tDest.vAxis[AXIS_Z]));
	r1 = tSrc.vLength.x * fAbsAxisDot[AXIS_X][AXIS_Z] +
		tSrc.vLength.y * fAbsAxisDot[AXIS_Y][AXIS_Z] +
		tSrc.vLength.z * fAbsAxisDot[AXIS_Z][AXIS_Z];
	r2 = tDest.vLength.z;

	if (r > r1 + r2)
		return false;

	if (bAxis)
		return true;

	r = abs(fDirDot[AXIS_Z] * fAxisDot[AXIS_Y][AXIS_X] -
		fDirDot[AXIS_Y] * fAxisDot[AXIS_Z][AXIS_X]);
	r1 = tSrc.vLength.y * fAbsAxisDot[AXIS_Z][AXIS_X] +
		tSrc.vLength.z * fAbsAxisDot[AXIS_Y][AXIS_X];
	r2 = tDest.vLength.y * fAbsAxisDot[AXIS_X][AXIS_Z] +
		tDest.vLength.z * fAbsAxisDot[AXIS_X][AXIS_Y];

	if (r > r1 + r2)
		return false;

	r = abs(fDirDot[AXIS_Z] * fAxisDot[AXIS_Y][AXIS_Y] -
		fDirDot[AXIS_Y] * fAxisDot[AXIS_Z][AXIS_Y]);
	r1 = tSrc.vLength.y * fAbsAxisDot[AXIS_Z][AXIS_Y] +
		tSrc.vLength.z * fAbsAxisDot[AXIS_Y][AXIS_Y];
	r2 = tDest.vLength.x * fAbsAxisDot[AXIS_X][AXIS_Z] +
		tDest.vLength.z * fAbsAxisDot[AXIS_X][AXIS_X];

	if (r > r1 + r2)
		return false;

	r = abs(fDirDot[AXIS_Z] * fAxisDot[AXIS_Y][AXIS_Z] -
		fDirDot[AXIS_Y] * fAxisDot[AXIS_Z][AXIS_Z]);
	r1 = tSrc.vLength.y * fAbsAxisDot[AXIS_Z][AXIS_Z] +
		tSrc.vLength.z * fAbsAxisDot[AXIS_Y][AXIS_Z];
	r2 = tDest.vLength.x * fAbsAxisDot[AXIS_X][AXIS_Y] +
		tDest.vLength.y * fAbsAxisDot[AXIS_X][AXIS_X];

	if (r > r1 + r2)
		return false;

	r = abs(fDirDot[AXIS_X] * fAxisDot[AXIS_Z][AXIS_X] -
		fDirDot[AXIS_Y] * fAxisDot[AXIS_Z][AXIS_Z]);
	r1 = tSrc.vLength.x * fAbsAxisDot[AXIS_Z][AXIS_X] +
		tSrc.vLength.z * fAbsAxisDot[AXIS_X][AXIS_X];
	r2 = tDest.vLength.y * fAbsAxisDot[AXIS_Y][AXIS_Z] +
		tDest.vLength.z * fAbsAxisDot[AXIS_Y][AXIS_Y];

	if (r > r1 + r2)
		return false;

	r = abs(fDirDot[AXIS_X] * fAxisDot[AXIS_Z][AXIS_Y] -
		fDirDot[AXIS_Z] * fAxisDot[AXIS_X][AXIS_Y]);
	r1 = tSrc.vLength.x * fAbsAxisDot[AXIS_Z][AXIS_Y] +
		tSrc.vLength.z * fAbsAxisDot[AXIS_X][AXIS_Y];
	r2 = tDest.vLength.x * fAbsAxisDot[AXIS_Y][AXIS_Z] +
		tDest.vLength.z * fAbsAxisDot[AXIS_Y][AXIS_X];

	if (r > r1 + r2)
		return false;

	r = abs(fDirDot[AXIS_X] * fAxisDot[AXIS_Z][AXIS_Z] -
		fDirDot[AXIS_Z] * fAxisDot[AXIS_X][AXIS_Z]);
	r1 = tSrc.vLength.x * fAbsAxisDot[AXIS_Z][AXIS_Z] +
		tSrc.vLength.z * fAbsAxisDot[AXIS_X][AXIS_Z];
	r2 = tDest.vLength.x * fAbsAxisDot[AXIS_Y][AXIS_Y] +
		tDest.vLength.y * fAbsAxisDot[AXIS_Y][AXIS_X];

	if (r > r1 + r2)
		return false;

	r = abs(fDirDot[AXIS_Y] * fAxisDot[AXIS_X][AXIS_X] -
		fDirDot[AXIS_X] * fAxisDot[AXIS_Y][AXIS_X]);
	r1 = tSrc.vLength.x * fAbsAxisDot[AXIS_Y][AXIS_X] +
		tSrc.vLength.y * fAbsAxisDot[AXIS_X][AXIS_X];
	r2 = tDest.vLength.y * fAbsAxisDot[AXIS_Z][AXIS_Z] +
		tDest.vLength.z * fAbsAxisDot[AXIS_Z][AXIS_Y];

	if (r > r1 + r2)
		return false;

	r = abs(fDirDot[AXIS_Y] * fAxisDot[AXIS_X][AXIS_Y] -
		fDirDot[AXIS_X] * fAxisDot[AXIS_Y][AXIS_Y]);
	r1 = tSrc.vLength.x * fAbsAxisDot[AXIS_Y][AXIS_Y] +
		tSrc.vLength.y * fAbsAxisDot[AXIS_X][AXIS_Y];
	r2 = tDest.vLength.x * fAbsAxisDot[AXIS_Z][AXIS_Z] +
		tDest.vLength.z * fAbsAxisDot[AXIS_Z][AXIS_X];

	if (r > r1 + r2)
		return false;

	r = abs(fDirDot[AXIS_Y] * fAxisDot[AXIS_X][AXIS_Z] -
		fDirDot[AXIS_X] * fAxisDot[AXIS_Y][AXIS_Z]);
	r1 = tSrc.vLength.x * fAbsAxisDot[AXIS_Y][AXIS_Z] +
		tSrc.vLength.y * fAbsAxisDot[AXIS_X][AXIS_Z];
	r2 = tDest.vLength.x * fAbsAxisDot[AXIS_Z][AXIS_Y] +
		tDest.vLength.y * fAbsAxisDot[AXIS_Z][AXIS_X];

	if (r > r1 + r2)
		return false;

	return true;
}

bool CCollider::CollisionRayToSphere(const RayInfo & tSrc, const SphereInfo & tDest)
{
	Vector3	vM = tSrc.vPos - tDest.vCenter;

	float	b, c;

	b = 2.f * vM.Dot(tSrc.vDir);
	c = vM.Dot(vM) - tDest.fRadius * tDest.fRadius;

	// 판별식
	float	fDisc = b * b - 4.f * c;

	if (fDisc < 0.f)
		return false;

	fDisc = sqrtf(fDisc);

	float	t0, t1;

	t0 = (-b + fDisc) / 2.f;
	t1 = (-b - fDisc) / 2.f;

	if (t0 < 0.f && t1 < 0.f)
		return false;

	return true;
}

bool CCollider::CollisionRayToOBB3D(const OBB3DInfo & tDest, const RayInfo & tSrc, CTransform * pOBBTr)
{
	Matrix OBBRotInv;
	Vector3 vOBBRot = m_pTransform->GetWorldRot();
	OBBRotInv.Rotation(vOBBRot);
	OBBRotInv.Inverse();
	_tagAABBInfo ABInfo;
	ABInfo.vCenter = tDest.vCenter;
	ABInfo.vLength = tDest.vLength;
	Vector3 vRayDir;
	Vector3 vRayPos;
	RayInfo tRay = tSrc;
	vRayPos = tRay.vPos;
	vRayDir = tSrc.vDir;
	vRayDir = vRayDir.TransformNormal(OBBRotInv);
	tRay.vDir = vRayDir;
	memcpy(&OBBRotInv._41, &tDest.vCenter, sizeof(Vector3));
	vRayPos = tSrc.vPos - tDest.vCenter;
	vRayPos = vRayPos.TransformCoord(OBBRotInv);
	tRay.vPos = vRayPos;

	return CollisionRayToAABB(tRay, ABInfo);
}

bool CCollider::CollisionRayToAABB(const RayInfo & _tSrc, const _tagAABBInfo & _tDest)
{
	float t;
	Vector3 dirfrac;
	// r.dir is unit direction vector of ray
	dirfrac.x = 1.0f / _tSrc.vDir.x;
	dirfrac.y = 1.0f / _tSrc.vDir.y;
	dirfrac.z = 1.0f / _tSrc.vDir.z;

	Vector3 vBoxLB;
	vBoxLB = _tDest.vCenter - (_tDest.vLength);
	Vector3 vBoxRT;
	vBoxRT = _tDest.vCenter + (_tDest.vLength);
	Vector3 vOriginPos = _tSrc.vPos;

	// lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
	// r.org is origin of ray
	float t1 = (vBoxLB.x - vOriginPos.x)*dirfrac.x;
	float t2 = (vBoxRT.x - vOriginPos.x)*dirfrac.x;
	float t3 = (vBoxLB.y - vOriginPos.y)*dirfrac.y;
	float t4 = (vBoxRT.y - vOriginPos.y)*dirfrac.y;
	float t5 = (vBoxLB.z - vOriginPos.z)*dirfrac.z;
	float t6 = (vBoxRT.z - vOriginPos.z)*dirfrac.z;

	float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
	float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

	// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
	if (tmax < 0)
	{
		t = tmax;
		return false;
	}

	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax)
	{
		t = tmax;
		return false;
	}

	t = tmin;

	return true;
}

void CCollider::SetCollisionCallback(COLLISION_CALLBACK_TYPE eType,
	void(*pFunc)(CCollider *, CCollider *, float))
{
	function<void(CCollider*, CCollider*, float)>	func;

	func = bind(pFunc, placeholders::_1, placeholders::_2, placeholders::_3);

	m_CollisionFunc[eType].push_back(func);
}

