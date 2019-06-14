#include "../ClientHeader.h"
#include "ControlBase.h"
#include "Door.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "Component/ColliderRect.h"
#include "Component/ColliderOBB3D.h"
#include "../GameManager.h"


CControlBase::CControlBase()
{
	m_eComType = (COMPONENT_TYPE)UT_CONTROL;
	m_eControlType = CONTROL_END;
	m_vecDoor.clear();
	m_bLeverDown = false;
	m_fAccTime = 0.0f;
	m_fLimitTime = 0.5f;
}

CControlBase::CControlBase(const CControlBase & pCom)
{
}


CControlBase::~CControlBase()
{
	//SAFE_RELEASE(m_pLeverTr);
}

void CControlBase::AfterClone()
{
}

bool CControlBase::Init()
{
	/*CGameObject* pObj = CGameObject::CreateObject("LeverObj", m_pLayer);

	m_pObject->AddChild(pObj);

	CRenderer* pRD = pObj->AddComponent<CRenderer>("ControlRenderer");
	m_pLeverTr = pObj->GetTransform();

	m_pLeverTr->SetLocalRotX(-45.0f);
	m_pLeverTr->SetWorldScale(0.05f);

	pRD->SetMesh("Lever", TEXT("Lever.msh"));

	SAFE_RELEASE(pRD);
	SAFE_RELEASE(pObj);*/

	/*pRD = FindComponentFromType<CRenderer>(CT_RENDERER);
	if (!pRD)
	{
		pRD = AddComponent<CRenderer>("ControlRenderer");
		pRD->SetMesh("ElectricalBox-03", TEXT("ElectricalBox-03.msh"));
	}
	CColliderOBB3D* pOBB = AddComponent<CColliderOBB3D>("ControlOBB");

	Vector3 vMeshLength = pRD->GetMeshLength();
	Vector3 vScale = vMeshLength * GetWorldScale();
	Vector3 vCenter;

	vCenter.x = vMeshLength.x * -0.5f;
	vCenter.z = vMeshLength.z * 0.0f;

	vScale = vScale * 0.5f;

	pOBB->SetInfo(vCenter, Vector3::Axis, vScale);
	pOBB->SetColliderID((COLLIDER_ID)UCI_CONTROL);

	SAFE_RELEASE(pRD);
*/
	return true;
}

int CControlBase::Input(float fTime)
{
	return 0;
}

int CControlBase::Update(float fTime)
{
	if (m_bLeverDown)
	{
		m_fAccTime += fTime;

		if (m_fAccTime < m_fLimitTime)
			m_pLeverTr->RotationX(90.0f, 1.0f / m_fLimitTime * fTime);
		else
		{
			m_pLeverTr->RotationX(135.0f);
			m_bLeverDown = false;
			Success();
		}
	}

	return 0;
}

int CControlBase::LateUpdate(float fTime)
{
	return 0;
}

void CControlBase::Collision(float fTime)
{
}

void CControlBase::Render(float fTime)
{
}

CControlBase * CControlBase::Clone()
{
	return new CControlBase(*this);
}

void CControlBase::SetControlType(CONTROL_TYPE eType)
{
	m_eControlType = eType;

	switch (m_eControlType)
	{
	case CONTROL_LIGHT:

		break;
	case CONTROL_DOOR:
		CGameObject* pObj = m_pLeverTr->GetGameObjectNonCount();
		pObj->SetEnable(false);

		break;
	}
}

void CControlBase::SetKeyName(const string & strKeyName)
{
	m_strKeyName = strKeyName;
}

void CControlBase::SetPassWord(int iPassword[4])
{
	m_iPassword[0] = iPassword[0];
	m_iPassword[1] = iPassword[1];
	m_iPassword[2] = iPassword[2];
	m_iPassword[3] = iPassword[3];
}

void CControlBase::SetPassWord(int i1, int i2, int i3, int i4)
{
	m_iPassword[0] = i1;
	m_iPassword[1] = i2;
	m_iPassword[2] = i3;
	m_iPassword[3] = i4;
}

bool CControlBase::Check()
{
	switch (m_eControlType)
	{
	case CONTROL_LIGHT:
	case CONTROL_DOOR:
		if (GET_SINGLE(CGameManager)->FindKey(m_strKeyName))
			return true;
		break;
	}

	return true;
}

void CControlBase::AddDoor(CDoor * pDoor)
{
	m_vecDoor.push_back(pDoor);
}


void CControlBase::Success()
{
	switch (m_eControlType)
	{
	case CONTROL_DOOR:
		for (int i = 0; i < m_vecDoor.size(); ++i)
			m_vecDoor[i]->UnLock();

		break;

	case CONTROL_LIGHT:
		GET_SINGLE(CGameManager)->BlinkAllSceneLight(2.0f, 0.25f, Vector4::White, true);
		break;
	}
}

void CControlBase::LeverDown()
{
	m_bLeverDown = true;
}

void CControlBase::ShowKeyPad()
{
	CGameObject* pObj = GET_SINGLE(CGameManager)->GetPlayer()->GetGameObject();
	pObj->SetEnable(false);


	SAFE_RELEASE(pObj);
}

void CControlBase::Interact()
{
	switch (m_eControlType)
	{
	case CONTROL_LIGHT:
		LeverDown();

		break;
	case CONTROL_DOOR:
		if (!m_pLeverTr->GetGameObjectNonCount()->GetEnable())
		{
			if (Check())
				m_pLeverTr->GetGameObjectNonCount()->SetEnable(true);
		}
		else
			LeverDown();

		break;
	}
}

void CControlBase::Interact(CCollider * pSrc, CCollider * pDest, float fTime)
{
	int iID = pDest->GetColliderID();

	if (iID == UCI_PLAYER_INTERACT)
	{
		if (KEYDOWN("KEY_E"))
			Interact();
	}
}
