#include "../EngineHeader.h"
#include "BoneSocket.h"

PUN_USING

CBoneSocket::CBoneSocket()
{
	m_pObject = NULLPTR;
	m_pAniObject = NULLPTR;
}

CBoneSocket::~CBoneSocket()
{
}

bool CBoneSocket::Init()
{
	return true;
}

void CBoneSocket::Update(float fTime, const Matrix & matBone)
{
	if (m_pObject)
	{
		//m_pObject = 의자오브젝트
		//m_pAniObject = 플레이어 오브젝트
		if (m_pObject->GetActive() == false)
			return;

		if (m_pAniObject->GetActive() == false)
			return;

		Matrix matScale;
		matScale.Scaling(m_pAniObject->GetTransformNonCount()->GetWorldScale());

		CTransform*	pTransform = m_pObject->GetTransformNoneCount();
		pTransform->SetBoneMatrix(matBone * matScale);
		pTransform->SetLocalRot(m_vRotation);
		pTransform->SetWorldPos(m_pAniObject->GetTransformNonCount()->GetWorldPos() + m_vOffset);
	}
}

void CBoneSocket::SetObject(class CGameObject* myObject, class CGameObject* AniObject)
{
	m_pObject = myObject;
	m_pAniObject = AniObject;
}

void CBoneSocket::SetOffset(const Vector3 & vOffset)
{
	m_vOffset = vOffset;
}

void CBoneSocket::SetOffset(float x, float y, float z)
{
	m_vOffset = Vector3(x, y, z);
}

void CBoneSocket::SetRotation(const Vector3 & vRot)
{
	m_vRotation = vRot;
}

void CBoneSocket::SetRotation(float x, float y, float z)
{
	m_vRotation = Vector3(x, y, z);
}

void CBoneSocket::SetRotationX(float x)
{
	m_vRotation.x = x;
}

void CBoneSocket::SetRotationY(float y)
{
	m_vRotation.y = y;
}

void CBoneSocket::SetRotationZ(float z)
{
	m_vRotation.z = z;
}
