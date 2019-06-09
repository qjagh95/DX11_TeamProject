#include "../ClientHeader.h"
#include "DocxGongji.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "GameObject.h"
#include "Component/ColliderRect.h"
#include "Document.h"
#include "DocxInven.h"

CDocxGongji::CDocxGongji()
{
	m_eComType = (COMPONENT_TYPE)DT_DOCXGONGJI;
	m_pAnimation = nullptr;
	m_bMouseOn = false;
}

CDocxGongji::CDocxGongji(const CDocxGongji & doc)	:
	CUserComponent(doc)
{
	m_pAnimation = nullptr;
}

CDocxGongji::~CDocxGongji()
{
	SAFE_RELEASE(m_pAnimation);
}

void CDocxGongji::AfterClone()
{
}

bool CDocxGongji::Init()
{
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("GongjiRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetDecalEnable(false);
	pRenderer->Enable2DRenderer();

	m_pObject->SetRenderGroup(RG_UI);

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "DocxGongji", TEXT("UI/Document/Stage1/NoClick_Gongji.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(330.f, 80.f, 1.f);
	m_pTransform->SetWorldPivot(0.5f, 0.5f, 0.f);

	m_pAnimation = m_pObject->AddComponent<CAnimation2D>("GongjiAnimation");

	vector<Clip2DFrame>	vecClipFrame;
	Clip2DFrame	tFrame = {};

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(320.f, 73.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("Gongji_NoClick", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"NoClick_Gongji", TEXT("UI/Document/Stage1/NoClick_Gongji.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(320.f, 73.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("Gongji_Click", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"Click_Gongji", TEXT("UI/Document/Stage1/Click_Gongji.png"));

	vecClipFrame.clear();

	CColliderRect* pBody = AddComponent<CColliderRect>("GongjiBody");

	pBody->SetCollisionCallback(CCT_ENTER, this, &CDocxGongji::Hit);
	pBody->SetCollisionCallback(CCT_LEAVE, this, &CDocxGongji::MouseOut);

	pBody->SetCollisionGroup("UI");
	pBody->SetInfo(Vector3(0.f, 0.f, 0.f), Vector3(330.f, 80.f, 0.f));

	SAFE_RELEASE(pBody);

	return true;
}

int CDocxGongji::Input(float fTime)
{
	return 0;
}

int CDocxGongji::Update(float fTime)
{
	if (m_bMouseOn)
	{
		m_pAnimation->ChangeClip("Gongji_Click");

		if (KEYPRESS("LButton"))
		{
			CGameObject*	pDocObj = CGameObject::FindObject("Document");
			pDocObj->SetEnable(true);

			CDocument* pDoc = pDocObj->FindComponentFromType<CDocument>((COMPONENT_TYPE)IT_DOCXICON);

			pDoc->ChangeClip("Gongji_Docx");

			SAFE_RELEASE(pDoc);
			SAFE_RELEASE(pDocObj);
		}
	}

	else if (!m_bMouseOn)
	{
		m_pAnimation->ChangeClip("Gongji_NoClick");
	}

	return 0;
}

int CDocxGongji::LateUpdate(float fTime)
{
	return 0;
}

void CDocxGongji::Collision(float fTime)
{
}

void CDocxGongji::Render(float fTime)
{
}

CDocxGongji * CDocxGongji::Clone()
{
	return new CDocxGongji(*this);
}

void CDocxGongji::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_bMouseOn = true;
	}
}

void CDocxGongji::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_bMouseOn = false;
	}
}
