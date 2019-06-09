#include "../ClientHeader.h"
#include "MessagePlanA.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "GameObject.h"
#include "Component/ColliderRect.h"
#include "Document.h"
#include "DocxInven.h"

CMessagePlanA::CMessagePlanA()
{
	m_eComType = (COMPONENT_TYPE)DT_MSGPLANA;
	m_pAnimation = nullptr;
	m_bMouseOn = false;
}

CMessagePlanA::CMessagePlanA(const CMessagePlanA & doc)	:
	CUserComponent(doc)
{
	m_pAnimation = nullptr;
}

CMessagePlanA::~CMessagePlanA()
{
	SAFE_RELEASE(m_pAnimation);
}

void CMessagePlanA::AfterClone()
{
}

bool CMessagePlanA::Init()
{
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("PARenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetDecalEnable(false);
	pRenderer->Enable2DRenderer();

	m_pObject->SetRenderGroup(RG_UI);

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "MessagePlnaA", TEXT("UI/Document/Stage2/NoClick_Plan_A.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(330.f, 80.f, 1.f);
	m_pTransform->SetWorldPivot(0.5f, 0.5f, 0.f);

	m_pAnimation = m_pObject->AddComponent<CAnimation2D>("PAAnimation");

	vector<Clip2DFrame>	vecClipFrame;
	Clip2DFrame	tFrame = {};

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(320.f, 73.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("PA_NoClick", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"NoClick_PA", TEXT("UI/Document/Stage2/NoClick_Plan_A.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(320.f, 73.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("PA_Click", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"Click_PA", TEXT("UI/Document/Stage2/Click_Plan_A.png"));

	vecClipFrame.clear();

	CColliderRect* pBody = AddComponent<CColliderRect>("DGBody");

	pBody->SetCollisionCallback(CCT_ENTER, this, &CMessagePlanA::Hit);
	pBody->SetCollisionCallback(CCT_LEAVE, this, &CMessagePlanA::MouseOut);

	pBody->SetCollisionGroup("UI");
	pBody->SetInfo(Vector3(0.f, 0.f, 0.f), Vector3(330.f, 80.f, 0.f));

	SAFE_RELEASE(pBody);

	return true;
}

int CMessagePlanA::Input(float fTime)
{
	return 0;
}

int CMessagePlanA::Update(float fTime)
{
	if (m_bMouseOn)
	{
		m_pAnimation->ChangeClip("PA_Click");

		if (KEYPRESS("LButton"))
		{
			CGameObject*	pDocObj = CGameObject::FindObject("Document");
			pDocObj->SetEnable(true);

			CDocument* pDoc = pDocObj->FindComponentFromType<CDocument>((COMPONENT_TYPE)IT_DOCXICON);

			pDoc->ChangeClip("Plan_A720");

			SAFE_RELEASE(pDoc);
			SAFE_RELEASE(pDocObj);
		}
	}

	else if (!m_bMouseOn)
	{
		m_pAnimation->ChangeClip("PA_NoClick");
	}

	return 0;
}

int CMessagePlanA::LateUpdate(float fTime)
{
	return 0;
}

void CMessagePlanA::Collision(float fTime)
{
}

void CMessagePlanA::Render(float fTime)
{
}

CMessagePlanA * CMessagePlanA::Clone()
{
	return new CMessagePlanA(*this);
}

void CMessagePlanA::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_bMouseOn = true;
	}
}

void CMessagePlanA::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_bMouseOn = false;
	}
}
