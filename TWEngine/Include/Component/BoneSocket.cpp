#include "../EngineHeader.h"
#include "BoneSocket.h"

PUN_USING

CBoneSocket::CBoneSocket()
{
	m_pObject = NULLPTR;
}

CBoneSocket::~CBoneSocket()
{
	SAFE_RELEASE(m_pObject);
}

bool CBoneSocket::Init()
{
	return true;
}

void CBoneSocket::Update(float fTime, const Matrix & matBone)
{
	if (m_pObject)
	{
		if (!m_pObject->GetActive())
		{
			SAFE_RELEASE(m_pObject);
			return;
		}

		Matrix	matRot, matTrans, matSocket;

		matRot.Rotation(m_vRotation);
		matTrans.Translation(m_vOffset);

		matSocket = matRot * matTrans;

		CTransform*	pTransform = m_pObject->GetTransformNoneCount();

		pTransform->SetOffsetRot(m_vRotation);
		pTransform->SetBoneMatrix(matBone * matTrans);
	}
}

void CBoneSocket::SetObject(CGameObject * pObj)
{
	SAFE_RELEASE(m_pObject);

	if (!pObj)
		pObj->AddRef();

	m_pObject = pObj;
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
