#include "../ClientHeader.h"
#include "PhoneMessage.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "GameObject.h"
#include "Component/ColliderRect.h"
#include "Document.h"
#include "DocxInven.h"

CPhoneMessage::CPhoneMessage()
{
	m_eComType = (COMPONENT_TYPE)DT_PHONEMESSAGE;
	m_pAnimation = nullptr;
	m_bMouseOn = false;
}

CPhoneMessage::CPhoneMessage(const CPhoneMessage & doc)	:
	CUserComponent(doc)
{
	m_pAnimation = nullptr;
}

CPhoneMessage::~CPhoneMessage()
{
	SAFE_RELEASE(m_pAnimation);
}

void CPhoneMessage::AfterClone()
{
}

bool CPhoneMessage::Init()
{
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("PMRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetDecalEnable(false);
	pRenderer->Enable2DRenderer();

	m_pObject->SetRenderGroup(RG_UI);

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "PhoneMessage", TEXT("UI/Document/PhoneMessageNoClick.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(220.f, 53.f, 1.f);
	m_pTransform->SetWorldPivot(0.5f, 0.5f, 0.f);

	m_pAnimation = m_pObject->AddComponent<CAnimation2D>("PMAnimation");

	vector<Clip2DFrame>	vecClipFrame;
	Clip2DFrame	tFrame = {};

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(320.f, 73.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("PM_NoClick", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"NoClick_PM", TEXT("UI/Document/PhoneMessageNoClick.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(320.f, 73.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("PM_Click", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"Click_PM", TEXT("UI/Document/PhoneMessageClick.png"));

	vecClipFrame.clear();

	CColliderRect* pBody = AddComponent<CColliderRect>("PhoneMessageBody");

	pBody->SetCollisionCallback(CCT_ENTER, this, &CPhoneMessage::Hit);
	pBody->SetCollisionCallback(CCT_LEAVE, this, &CPhoneMessage::MouseOut);

	pBody->SetCollisionGroup("UI");
	pBody->SetInfo(Vector3(0.f, 0.f, 0.f), Vector3(220.f, 53.f, 0.f));

	SAFE_RELEASE(pBody);

	return true;
}

int CPhoneMessage::Input(float fTime)
{
	return 0;
}

int CPhoneMessage::Update(float fTime)
{
	if (m_bMouseOn)
	{
		m_pAnimation->ChangeClip("PM_Click");

		if (KEYPRESS("LButton"))
		{
			CGameObject*	pDocObj = CGameObject::FindObject("Document");
			pDocObj->SetEnable(true);

			CDocument* pDoc = pDocObj->FindComponentFromType<CDocument>((COMPONENT_TYPE)IT_DOCXICON);

			pDoc->ChangeClip("MobileMessage");

			SAFE_RELEASE(pDoc);
			SAFE_RELEASE(pDocObj);
		}
	}

	else if (!m_bMouseOn)
	{
		m_pAnimation->ChangeClip("PM_NoClick");
	}

	return 0;
}

int CPhoneMessage::LateUpdate(float fTime)
{
	return 0;
}

void CPhoneMessage::Collision(float fTime)
{
}

void CPhoneMessage::Render(float fTime)
{
}

CPhoneMessage * CPhoneMessage::Clone()
{
	return new CPhoneMessage(*this);
}

void CPhoneMessage::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_bMouseOn = true;
	}
}

void CPhoneMessage::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_bMouseOn = false;
	}
}
