#include "../ClientHeader.h"
#include "CenterKey.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "GameObject.h"
#include "Component/ColliderRect.h"
#include "KeyBigIcon.h"
#include "KeyInven.h"

CCenterKey::CCenterKey()
{
	m_eComType = (COMPONENT_TYPE)KT_CENTER;
	m_bMouseOn = false;
}

CCenterKey::CCenterKey(const CCenterKey & key)	:
	CUserComponent(key)
{
}


CCenterKey::~CCenterKey()
{
}

void CCenterKey::AfterClone()
{
}

bool CCenterKey::Init()
{
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("CKRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetDecalEnable(false);
	pRenderer->Enable2DRenderer();

	m_pObject->SetRenderGroup(RG_UI);

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "CenterKey", TEXT("UI/Key/KeyIcon_center.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	CTransform*		pTransform = m_pObject->GetTransform();

	pTransform->SetWorldScale(74.f, 94.f, 1.f);

	SAFE_RELEASE(pTransform);

	CColliderRect* pBody = AddComponent<CColliderRect>("CenterKeyBody");

	pBody->SetCollisionCallback(CCT_ENTER, this, &CCenterKey::Hit);
	pBody->SetCollisionCallback(CCT_LEAVE, this, &CCenterKey::MouseOut);

	pBody->SetCollisionGroup("UI");
	pBody->SetInfo(Vector3(0.f, 0.f, 0.f), Vector3(100.f, 100.f, 0.f));

	SAFE_RELEASE(pBody);

	return true;
}

int CCenterKey::Input(float fTime)
{
	return 0;
}

int CCenterKey::Update(float fTime)
{
	if (m_bMouseOn)
	{
		if (KEYPRESS("LButton"))
		{
			CGameObject*	pKeyBigObj = CGameObject::FindObject("KeyBigIcon");
			pKeyBigObj->SetEnable(true);

			CKeyBigICon* pKeyBig = pKeyBigObj->FindComponentFromType<CKeyBigICon>((COMPONENT_TYPE)IT_KEYICON);

			pKeyBig->ChangeClip("KeyBig_KeyCenter");

			SAFE_RELEASE(pKeyBig);
			SAFE_RELEASE(pKeyBigObj);
		}
	}

	return 0;
}

int CCenterKey::LateUpdate(float fTime)
{
	return 0;
}

void CCenterKey::Collision(float fTime)
{
}

void CCenterKey::Render(float fTime)
{
}

CCenterKey * CCenterKey::Clone()
{
	return new CCenterKey(*this);
}

void CCenterKey::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_bMouseOn = true;
	}
}

void CCenterKey::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_bMouseOn = false;
	}
}
