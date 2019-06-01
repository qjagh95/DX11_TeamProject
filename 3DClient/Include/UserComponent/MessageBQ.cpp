#include "../ClientHeader.h"
#include "MessageBQ.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "GameObject.h"
#include "Component/ColliderRect.h"
#include "Document.h"
#include "DocxInven.h"

CMessageBQ::CMessageBQ()
{
	m_eComType = (COMPONENT_TYPE)DT_MSGBQ;
	m_pAnimation = nullptr;
	m_bMouseOn = false;
}

CMessageBQ::CMessageBQ(const CMessageBQ & doc)	:
	CUserComponent(doc)
{
	m_pAnimation = nullptr;
}

CMessageBQ::~CMessageBQ()
{
	SAFE_RELEASE(m_pAnimation);
}

void CMessageBQ::AfterClone()
{
}

bool CMessageBQ::Init()
{
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("DBQRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetDecalEnable(false);
	pRenderer->Enable2DRenderer();

	m_pObject->SetRenderGroup(RG_UI);

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "MessageBQ", TEXT("UI/Document/Stage3/NoClick_BeQuiet.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(220.f, 53.f, 1.f);
	m_pTransform->SetWorldPivot(0.5f, 0.5f, 0.f);

	m_pAnimation = m_pObject->AddComponent<CAnimation2D>("DBQAnimation");

	vector<Clip2DFrame>	vecClipFrame;
	Clip2DFrame	tFrame = {};

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(320.f, 73.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("DBQ_NoClick", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"NoClick_DBQ", TEXT("UI/Document/Stage3/NoClick_BeQuiet.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(320.f, 73.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("DBQ_Click", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"Click_DBQ", TEXT("UI/Document/Stage3/Click_BeQuiet.png"));

	vecClipFrame.clear();

	CColliderRect* pBody = AddComponent<CColliderRect>("DBQBody");

	pBody->SetCollisionCallback(CCT_ENTER, this, &CMessageBQ::Hit);
	pBody->SetCollisionCallback(CCT_LEAVE, this, &CMessageBQ::MouseOut);

	pBody->SetCollisionGroup("UI");
	pBody->SetInfo(Vector3(0.f, 0.f, 0.f), Vector3(220.f, 53.f, 0.f));

	SAFE_RELEASE(pBody);

	return true;
}

int CMessageBQ::Input(float fTime)
{
	return 0;
}

int CMessageBQ::Update(float fTime)
{
	if (m_bMouseOn)
	{
		m_pAnimation->ChangeClip("DBQ_Click");

		if (KEYPRESS("LButton"))
		{
			CGameObject*	pDocObj = CGameObject::FindObject("Document");
			pDocObj->SetEnable(true);

			CDocument* pDoc = pDocObj->FindComponentFromType<CDocument>((COMPONENT_TYPE)IT_DOCXICON);

			pDoc->ChangeClip("BeQuiet720");

			SAFE_RELEASE(pDoc);
			SAFE_RELEASE(pDocObj);
		}
	}

	else if (!m_bMouseOn)
	{
		m_pAnimation->ChangeClip("DBQ_NoClick");
	}

	return 0;
}

int CMessageBQ::LateUpdate(float fTime)
{
	return 0;
}

void CMessageBQ::Collision(float fTime)
{
}

void CMessageBQ::Render(float fTime)
{
}

CMessageBQ * CMessageBQ::Clone()
{
	return new CMessageBQ(*this);
}

void CMessageBQ::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_bMouseOn = true;
	}
}

void CMessageBQ::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_bMouseOn = false;
	}
}
