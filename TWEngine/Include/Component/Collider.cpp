#include "EngineHeader.h"
#include "Collider.h"
#include "../Rendering/Shader.h"
#include "../Device.h"
#include "../Rendering/RenderState.h"
#include "../Resource/Mesh.h"

PUN_USING

CCollider::CCollider()
{
	m_eComType = CT_COLLIDER;
	m_strCollisionGroup = "Default";

	m_bUpdateCollision = true;

#ifdef _DEBUG
	m_pShader = GET_SINGLE(CShaderManager)->FindShader(COLLIDER_SHADER);
	m_pMesh = nullptr;
	m_pLayout = GET_SINGLE(CShaderManager)->FindInputLayout(POS_LAYOUT);
	m_pDepthDisable = nullptr;
	m_vColor = Vector4::Green;
#endif // _DEBUG

}


CCollider::CCollider(const CCollider & com) :
	CComponent(com)
{
	*this = com;

#ifdef _DEBUG
	m_pLayout = com.m_pLayout;
	m_pMesh = com.m_pMesh;
	m_pShader = com.m_pShader;
	m_pDepthDisable = com.m_pDepthDisable;
	m_vColor = Vector4::Green;

	if (m_pShader)
		m_pShader->AddRef();

	if (m_pMesh)
		m_pMesh->AddRef();

	if (m_pDepthDisable)
		m_pDepthDisable->AddRef();
#endif // _DEBUG
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

#ifdef _DEBUG
	SAFE_RELEASE(m_pDepthDisable);
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pMesh);
#endif	// _DEBUG
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

// �� �浹ü�� �ҼӵǾ� �ִ� ���� �ε����� �����ش�.
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
#ifdef _DEBUG
	if (m_PrevCollision.empty())
		m_vColor = Vector4::Green;

	else
		m_vColor = Vector4::Red;

	m_pShader->SetShader();
	CONTEXT->IASetInputLayout(m_pLayout);

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Collider", &m_vColor);

	if (m_pDepthDisable)
		m_pDepthDisable->SetState();

	m_pMesh->Render();

	if (m_pDepthDisable)
		m_pDepthDisable->ResetState();
#endif
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

	// ���� A�� X�࿡ ���� ����ó��
	float	fLength = abs(tSrc.vAxis[0].Dot(vLength));

	// ���� B�� ���� A�� X�࿡ �����Ѵ�.
	float	fR1 = tSrc.fLength[0];
	float	fR2 = fDot[0][0] * tDest.fLength[0] + fDot[0][1] * tDest.fLength[1];

	if (fLength > fR1 + fR2)
		return false;

	// ���� A�� Y�࿡ ���� ����ó��
	fLength = abs(tSrc.vAxis[1].Dot(vLength));

	// ���� B�� ���� A�� Y�࿡ �����Ѵ�.
	fR1 = tSrc.fLength[1];
	fR2 = fDot[0][2] * tDest.fLength[0] + fDot[0][3] * tDest.fLength[1];

	if (fLength > fR1 + fR2)
		return false;

	// ���� B�� X�࿡ ���� ����ó��
	fLength = abs(tDest.vAxis[0].Dot(vLength));

	// ���� A�� ���� B�� X�࿡ �����Ѵ�.
	fR1 = tDest.fLength[0];
	fR2 = fDot[1][0] * tSrc.fLength[0] + fDot[1][1] * tSrc.fLength[1];

	if (fLength > fR1 + fR2)
		return false;

	// ���� B�� Y�࿡ ���� ����ó��
	fLength = abs(tDest.vAxis[1].Dot(vLength));

	// ���� A�� ���� B�� Y�࿡ �����Ѵ�.
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
	// OBB ���ڰ� Z������ ȸ���� ��ŭ �ݴ�� ���� OBB������ ������ ��������
	// ȸ����Ų ��ǥ�� ���Ѵ�. �̷��� �� ��� �� ���� OBB���ڸ� �����࿡ ���ĵ�
	// �Ϲ� RECT�� ������� ����� �� ��ġ�� ������ �Ǳ� ������
	// Pt In Rect�� �̿��ؼ� ó���� �����ϴ�.
	// ȸ�������� ���Ѵ�.
	// ȸ�� ������ ���Ѵ�. ȸ���� 2D �̱� ������ Z�� ȸ���� �Ѵ�.
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

	// Box�� �ȼ��������� ��ȯ�Ѵ�.
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

void CCollider::SetCollisionCallback(COLLISION_CALLBACK_TYPE eType,
	void(*pFunc)(CCollider *, CCollider *, float))
{
	function<void(CCollider*, CCollider*, float)>	func;

	func = bind(pFunc, placeholders::_1, placeholders::_2, placeholders::_3);

	m_CollisionFunc[eType].push_back(func);
}

