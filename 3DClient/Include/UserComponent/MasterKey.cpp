#include "../ClientHeader.h"
#include "MasterKey.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "GameObject.h"
#include "Component/ColliderRect.h"
#include "KeyBigIcon.h"
#include "KeyInven.h"

CMasterKey::CMasterKey()
{
	m_eComType = (COMPONENT_TYPE)KT_MASTER;
	m_bMouseOn = false;
}

CMasterKey::CMasterKey(const CMasterKey & key)	:
	CUserComponent(key)
{
}

CMasterKey::~CMasterKey()
{
}

void CMasterKey::AfterClone()
{
}

bool CMasterKey::Init()
{
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("MKRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetDecalEnable(false);
	pRenderer->Enable2DRenderer();

	m_pObject->SetRenderGroup(RG_UI);

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "MasterKey", TEXT("UI/Key/KeyIcon_master.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	CTransform*		pTransform = m_pObject->GetTransform();

	pTransform->SetWorldScale(106.f, 139.f, 1.f);

	SAFE_RELEASE(pTransform);

	CColliderRect* pBody = AddComponent<CColliderRect>("CenterKeyBody");

	pBody->SetCollisionCallback(CCT_ENTER, this, &CMasterKey::Hit);
	pBody->SetCollisionCallback(CCT_LEAVE, this, &CMasterKey::MouseOut);

	pBody->SetCollisionGroup("UI");
	pBody->SetInfo(Vector3(0.f, 0.f, 0.f), Vector3(150.f, 150.f, 0.f));

	SAFE_RELEASE(pBody);

	return true;
}

int CMasterKey::Input(float fTime)
{
	return 0;
}

int CMasterKey::Update(float fTime)
{
	if (m_bMouseOn)
	{
		if (KEYPRESS("LButton"))
		{
			CGameObject*	pKeyBigObj = CGameObject::FindObject("KeyBigIcon");
			pKeyBigObj->SetEnable(true);

			CKeyBigICon* pKeyBig = pKeyBigObj->FindComponentFromType<CKeyBigICon>((COMPONENT_TYPE)IT_KEYICON);

			pKeyBig->ChangeClip("KeyBig_KeyMaster");

			SAFE_RELEASE(pKeyBig);
			SAFE_RELEASE(pKeyBigObj);
		}
	}

	return 0;
}

int CMasterKey::LateUpdate(float fTime)
{
	return 0;
}

void CMasterKey::Collision(float fTime)
{
}

void CMasterKey::Render(float fTime)
{
}

CMasterKey * CMasterKey::Clone()
{
	return new CMasterKey(*this);
}

void CMasterKey::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_bMouseOn = true;
	}
}

void CMasterKey::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_bMouseOn = false;
	}
}
