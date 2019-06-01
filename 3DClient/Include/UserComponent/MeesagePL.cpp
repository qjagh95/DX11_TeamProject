#include "../ClientHeader.h"
#include "MeesagePL.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "GameObject.h"
#include "Component/ColliderRect.h"
#include "Document.h"
#include "DocxInven.h"

CMeesagePL::CMeesagePL()
{
	m_eComType = (COMPONENT_TYPE)DT_MSGPL;
	m_pAnimation = nullptr;
	m_bMouseOn = false;
}

CMeesagePL::CMeesagePL(const CMeesagePL & doc)	:
	CUserComponent(doc)
{
	m_pAnimation = nullptr;
}

CMeesagePL::~CMeesagePL()
{
	SAFE_RELEASE(m_pAnimation);
}

void CMeesagePL::AfterClone()
{
}

bool CMeesagePL::Init()
{
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("PLRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetDecalEnable(false);
	pRenderer->Enable2DRenderer();

	m_pObject->SetRenderGroup(RG_UI);

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "MessagePL", TEXT("UI/Document/Stage3/NoClick_PL.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(220.f, 53.f, 1.f);
	m_pTransform->SetWorldPivot(0.5f, 0.5f, 0.f);

	m_pAnimation = m_pObject->AddComponent<CAnimation2D>("PLAnimation");

	vector<Clip2DFrame>	vecClipFrame;
	Clip2DFrame	tFrame = {};

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(320.f, 73.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("PL_NoClick", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"NoClick_PL", TEXT("UI/Document/Stage3/NoClick_PL.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(320.f, 73.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("PL_Click", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"Click_PL", TEXT("UI/Document/Stage3/Click_PL.png"));

	vecClipFrame.clear();

	CColliderRect* pBody = AddComponent<CColliderRect>("PLBody");

	pBody->SetCollisionCallback(CCT_ENTER, this, &CMeesagePL::Hit);
	pBody->SetCollisionCallback(CCT_LEAVE, this, &CMeesagePL::MouseOut);

	pBody->SetCollisionGroup("UI");
	pBody->SetInfo(Vector3(0.f, 0.f, 0.f), Vector3(220.f, 53.f, 0.f));

	SAFE_RELEASE(pBody);

	return true;
}

int CMeesagePL::Input(float fTime)
{
	return 0;
}

int CMeesagePL::Update(float fTime)
{
	if (m_bMouseOn)
	{
		m_pAnimation->ChangeClip("PL_Click");

		if (KEYPRESS("LButton"))
		{
			CGameObject*	pDocObj = CGameObject::FindObject("Document");
			pDocObj->SetEnable(true);

			CDocument* pDoc = pDocObj->FindComponentFromType<CDocument>((COMPONENT_TYPE)IT_DOCXICON);

			pDoc->ChangeClip("Prepare_Lever720");

			SAFE_RELEASE(pDoc);
			SAFE_RELEASE(pDocObj);
		}
	}

	else if (!m_bMouseOn)
	{
		m_pAnimation->ChangeClip("PL_NoClick");
	}

	return 0;
}

int CMeesagePL::LateUpdate(float fTime)
{
	return 0;
}

void CMeesagePL::Collision(float fTime)
{
}

void CMeesagePL::Render(float fTime)
{
}

CMeesagePL * CMeesagePL::Clone()
{
	return new CMeesagePL(*this);
}

void CMeesagePL::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_bMouseOn = true;
	}
}

void CMeesagePL::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_bMouseOn = false;
	}
}
