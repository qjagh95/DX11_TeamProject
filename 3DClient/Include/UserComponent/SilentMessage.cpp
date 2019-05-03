#include "../ClientHeader.h"
#include "SilentMessage.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "GameObject.h"
#include "Component/ColliderRect.h"
#include "Document.h"
#include "DocxInven.h"

CSilentMessage::CSilentMessage()
{
	m_eComType = (COMPONENT_TYPE)DT_SILENTMESSAGE;
	m_pAnimation = nullptr;
	m_bMouseOn = false;
}

CSilentMessage::CSilentMessage(const CSilentMessage & doc)	:
	CUserComponent(doc)
{
	m_pAnimation = nullptr;
}

CSilentMessage::~CSilentMessage()
{
	SAFE_RELEASE(m_pAnimation);
}

void CSilentMessage::AfterClone()
{
}

bool CSilentMessage::Init()
{
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("SMRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetDecalEnable(false);
	pRenderer->Enable2DRenderer();

	m_pObject->SetRenderGroup(RG_UI);

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "SilentMessage", TEXT("UI/Document/SilentMessageNoClick.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(220.f, 53.f, 1.f);
	m_pTransform->SetWorldPivot(0.5f, 0.5f, 0.f);

	m_pAnimation = m_pObject->AddComponent<CAnimation2D>("SMAnimation");

	vector<Clip2DFrame>	vecClipFrame;
	Clip2DFrame	tFrame = {};

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(320.f, 73.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("SM_NoClick", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"NoClick_SM", TEXT("UI/Document/SilentMessageNoClick.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(320.f, 73.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("SM_Click", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"Click_SM", TEXT("UI/Document/SilentMessageClick.png"));

	vecClipFrame.clear();

	CColliderRect* pBody = AddComponent<CColliderRect>("SilentMessageBody");

	pBody->SetCollisionCallback(CCT_ENTER, this, &CSilentMessage::Hit);
	pBody->SetCollisionCallback(CCT_LEAVE, this, &CSilentMessage::MouseOut);

	pBody->SetCollisionGroup("UI");
	pBody->SetInfo(Vector3(0.f, 0.f, 0.f), Vector3(220.f, 53.f, 0.f));

	SAFE_RELEASE(pBody);

	return true;
}

int CSilentMessage::Input(float fTime)
{
	return 0;
}

int CSilentMessage::Update(float fTime)
{
	if (m_bMouseOn)
	{
		m_pAnimation->ChangeClip("SM_Click");

		if (KEYPRESS("LButton"))
		{
			CGameObject*	pDocObj = CGameObject::FindObject("Document");
			pDocObj->SetEnable(true);

			CDocument* pDoc = pDocObj->FindComponentFromType<CDocument>((COMPONENT_TYPE)IT_DOCXICON);

			pDoc->ChangeClip("SilentMessage");

			SAFE_RELEASE(pDoc);
			SAFE_RELEASE(pDocObj);
		}
	}

	else if (!m_bMouseOn)
	{
		m_pAnimation->ChangeClip("SM_NoClick");
	}

	return 0;
}

int CSilentMessage::LateUpdate(float fTime)
{
	return 0;
}

void CSilentMessage::Collision(float fTime)
{
}

void CSilentMessage::Render(float fTime)
{
}

CSilentMessage * CSilentMessage::Clone()
{
	return new CSilentMessage(*this);
}

void CSilentMessage::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_bMouseOn = true;
	}
}

void CSilentMessage::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_bMouseOn = false;
	}
}
