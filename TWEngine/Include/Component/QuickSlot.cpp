#include "EngineHeader.h"
#include "QuickSlot.h"
#include "Renderer.h"
#include "Material.h"
#include "ColliderRect.h"
#include "Transform.h"
#include "../GameObject.h"
#include "../Input.h"

PUN_USING

CQuickSlot::CQuickSlot()
{
	m_eUIType = UT_SLOT;
	SetTag("QuickSlot");
	m_bMouseOn = false;
	m_iFlag = -1;
}


CQuickSlot::CQuickSlot(const CQuickSlot & quickslot) :
	CUI(quickslot)
{
	m_eState = QS_NORMAL;
}

CQuickSlot::~CQuickSlot()
{
}

void CQuickSlot::AfterClone()
{
}

bool CQuickSlot::Init()
{
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("QuickSlotRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);

	pRenderer->Enable2DRenderer();

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "QuickSlot", TEXT("UI/QuickSlot/QS1_Edit.png"));

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(155.f, 115.f, 1.f);
	m_pTransform->SetWorldPivot(0.5f, 0.5f, 0.f);

	// QuickSlot
	/*CColliderRect*	pQuickSlotRC = AddComponent<CColliderRect>("QuickSlotBody");

	pQuickSlotRC->SetCollisionGroup("UI");
	pQuickSlotRC->SetInfo(Vector3(0.f, 0.f, 0.f), Vector3(155.f, 115.f, 0.f));
	pQuickSlotRC->SetCollisionCallback(CCT_ENTER, this, &CQuickSlot::Hit);
	pQuickSlotRC->SetCollisionCallback(CCT_STAY, this, &CQuickSlot::OnSlot);
	pQuickSlotRC->SetCollisionCallback(CCT_LEAVE, this, &CQuickSlot::MouseOut);

	SAFE_RELEASE(pQuickSlotRC);*/

	// Slot1.1
	CColliderRect*	pSlot1RC = AddComponent<CColliderRect>("SlotRCBody");

	pSlot1RC->SetCollisionGroup("UI");
	pSlot1RC->SetInfo(Vector3(25.f, 65.f, 0.f), Vector3(30.f, 70.f, 0.f));
	pSlot1RC->SetCollisionCallback(CCT_ENTER, this, &CQuickSlot::Hit);
	pSlot1RC->SetCollisionCallback(CCT_LEAVE, this, &CQuickSlot::MouseOut);

	SAFE_RELEASE(pSlot1RC);

	// Slot1.2
	CColliderRect*	pSlot2RC = AddComponent<CColliderRect>("Slot2RCBody");

	pSlot2RC->SetCollisionGroup("UI");
	pSlot2RC->SetInfo(Vector3(60.f, 65.f, 0.f), Vector3(65.f, 70.f, 0.f));
	pSlot2RC->SetCollisionCallback(CCT_ENTER, this, &CQuickSlot::Hit);
	pSlot2RC->SetCollisionCallback(CCT_LEAVE, this, &CQuickSlot::MouseOut);

	SAFE_RELEASE(pSlot2RC);

	// Slot1.3
	CColliderRect*	pSlot3RC = AddComponent<CColliderRect>("Slot3RCBody");

	pSlot3RC->SetCollisionGroup("UI");
	pSlot3RC->SetInfo(Vector3(93.f, 65.f, 0.f), Vector3(98.f, 70.f, 0.f));
	pSlot3RC->SetCollisionCallback(CCT_ENTER, this, &CQuickSlot::Hit);
	pSlot3RC->SetCollisionCallback(CCT_LEAVE, this, &CQuickSlot::MouseOut);

	SAFE_RELEASE(pSlot3RC);

	// Slot1.4
	CColliderRect*	pSlot4RC = AddComponent<CColliderRect>("Slot4RCBody");

	pSlot4RC->SetCollisionGroup("UI");
	pSlot4RC->SetInfo(Vector3(128.f, 65.f, 0.f), Vector3(133.f, 70.f, 0.f));
	pSlot4RC->SetCollisionCallback(CCT_ENTER, this, &CQuickSlot::Hit);
	pSlot4RC->SetCollisionCallback(CCT_LEAVE, this, &CQuickSlot::MouseOut);

	SAFE_RELEASE(pSlot4RC);

	// Slot2.1
	CColliderRect*	pSlot5RC = AddComponent<CColliderRect>("Slot5RCBody");

	pSlot5RC->SetCollisionGroup("UI");
	pSlot5RC->SetInfo(Vector3(24.f, 19.f, 0.f), Vector3(29.f, 24.f, 0.f));
	pSlot5RC->SetCollisionCallback(CCT_ENTER, this, &CQuickSlot::Hit);
	pSlot5RC->SetCollisionCallback(CCT_LEAVE, this, &CQuickSlot::MouseOut);

	SAFE_RELEASE(pSlot5RC);

	// Slot2.2
	CColliderRect*	pSlot6RC = AddComponent<CColliderRect>("Slot6RCBody");

	pSlot6RC->SetCollisionGroup("UI");
	pSlot6RC->SetInfo(Vector3(59.f, 19.f, 0.f), Vector3(64.f, 24.f, 0.f));
	pSlot6RC->SetCollisionCallback(CCT_ENTER, this, &CQuickSlot::Hit);
	pSlot6RC->SetCollisionCallback(CCT_LEAVE, this, &CQuickSlot::MouseOut);

	SAFE_RELEASE(pSlot6RC);

	// Slot2.3
	CColliderRect*	pSlot7RC = AddComponent<CColliderRect>("Slot7RCBody");

	pSlot7RC->SetCollisionGroup("UI");
	pSlot7RC->SetInfo(Vector3(94.f, 19.f, 0.f), Vector3(99.f, 24.f, 0.f));
	pSlot7RC->SetCollisionCallback(CCT_ENTER, this, &CQuickSlot::Hit);
	pSlot7RC->SetCollisionCallback(CCT_LEAVE, this, &CQuickSlot::MouseOut);

	SAFE_RELEASE(pSlot7RC);

	// Slot2.4
	CColliderRect*	pSlot8RC = AddComponent<CColliderRect>("Slot8RCBody");

	pSlot8RC->SetCollisionGroup("UI");
	pSlot8RC->SetInfo(Vector3(129.f, 19.f, 0.f), Vector3(134.f, 24.f, 0.f));
	pSlot8RC->SetCollisionCallback(CCT_ENTER, this, &CQuickSlot::Hit);
	pSlot8RC->SetCollisionCallback(CCT_LEAVE, this, &CQuickSlot::MouseOut);

	SAFE_RELEASE(pSlot8RC);

	m_eState = QS_NORMAL;

	return true;
}

int CQuickSlot::Input(float fTime)
{
	return 0;
}

int CQuickSlot::Update(float fTime)
{
	Vector2 vMouse = GET_SINGLE(CInput)->GetMouseClient();
	Vector3 vMousePos = Vector3(vMouse.x, vMouse.y, 0.f);

	if (m_bMouseOn)
	{
		m_pTransform->SetWorldPos(vMousePos);
	}

	return 0;
}

int CQuickSlot::LateUpdate(float fTime)
{
	return 0;
}

void CQuickSlot::Collision(float fTime)
{
}

void CQuickSlot::Render(float fTime)
{
}

CQuickSlot * CQuickSlot::Clone()
{
	return new CQuickSlot(*this);
}

void CQuickSlot::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	/*if (pDest->GetTag() == "MouseWindow")
	{
		m_iFlag *= -1;

		m_eState = QS_MOUSEON;
		if (KEYPRESS("LButton"))
		{
			if (m_iFlag == 1)
				m_bMouseOn = true;

			else if (m_iFlag == -1)
				m_bMouseOn = false;
		}
	}*/
	if (pDest->GetTag() == "MouseWindow")
	{
		m_eState = QS_MOUSEON;
	}
}

void CQuickSlot::OnSlot(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void CQuickSlot::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_eState = QS_NORMAL;
	}
}

void CQuickSlot::SetCallback(void(*pFunc)(float))
{
	m_ClickCallback = bind(pFunc, placeholders::_1);
}

