#include "../ClientHeader.h"
#include "MessageTH.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "GameObject.h"
#include "Component/ColliderRect.h"
#include "Document.h"
#include "DocxInven.h"

CMessageTH::CMessageTH()
{
	m_eComType = (COMPONENT_TYPE)DT_MSGTH;
	m_pAnimation = nullptr;
	m_bMouseOn = false;
}

CMessageTH::CMessageTH(const CMessageTH & msg)	:
	CUserComponent(msg)
{
	m_pAnimation = nullptr;
}

CMessageTH::~CMessageTH()
{
	SAFE_RELEASE(m_pAnimation);
}

void CMessageTH::AfterClone()
{
}

bool CMessageTH::Init()
{
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("THRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetDecalEnable(false);
	pRenderer->Enable2DRenderer();

	m_pObject->SetRenderGroup(RG_UI);

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "MessageTH", TEXT("UI/Document/Stage1/NoClick_msg_TH.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	//m_pTransform->SetWorldScale(330.f, 80.f, 1.f);
	m_pTransform->SetWorldScale(220.f, 53.f, 1.f);
	m_pTransform->SetWorldPivot(0.5f, 0.5f, 0.f);

	m_pAnimation = m_pObject->AddComponent<CAnimation2D>("MsgTHAnimation");

	vector<Clip2DFrame>	vecClipFrame;
	Clip2DFrame	tFrame = {};

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(320.f, 73.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("msgTH_NoClick", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"NoClick_msgTH", TEXT("UI/Document/Stage1/NoClick_msg_TH.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(320.f, 73.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("msgTH_Click", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"Click_msgTH", TEXT("UI/Document/Stage1/Click_msg_TH.png"));

	vecClipFrame.clear();

	CColliderRect* pBody = AddComponent<CColliderRect>("msgTHBody");

	pBody->SetCollisionCallback(CCT_ENTER, this, &CMessageTH::Hit);
	pBody->SetCollisionCallback(CCT_LEAVE, this, &CMessageTH::MouseOut);

	pBody->SetCollisionGroup("UI");
	//pBody->SetInfo(Vector3(0.f, 0.f, 0.f), Vector3(330.f, 80.f, 0.f));
	pBody->SetInfo(Vector3(0.f, 0.f, 0.f), Vector3(220.f, 53.f, 0.f));

	SAFE_RELEASE(pBody);

	return true;
}

int CMessageTH::Input(float fTime)
{
	return 0;
}

int CMessageTH::Update(float fTime)
{
	if (m_bMouseOn)
	{
		m_pAnimation->ChangeClip("msgTH_Click");

		if (KEYPRESS("LButton"))
		{
			CGameObject*	pDocObj = CGameObject::FindObject("Document");
			pDocObj->SetEnable(true);

			CDocument* pDoc = pDocObj->FindComponentFromType<CDocument>((COMPONENT_TYPE)IT_DOCXICON);

			pDoc->ChangeClip("Message_TH720");

			SAFE_RELEASE(pDoc);
			SAFE_RELEASE(pDocObj);
		}
	}

	else if (!m_bMouseOn)
	{
		m_pAnimation->ChangeClip("msgTH_NoClick");
	}

	return 0;
}

int CMessageTH::LateUpdate(float fTime)
{
	return 0;
}

void CMessageTH::Collision(float fTime)
{
}

void CMessageTH::Render(float fTime)
{
}

CMessageTH * CMessageTH::Clone()
{
	return new CMessageTH(*this);
}

void CMessageTH::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_bMouseOn = true;
	}
}

void CMessageTH::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_bMouseOn = false;
	}
}
