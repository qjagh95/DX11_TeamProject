#include "../ClientHeader.h"
#include "DocxTest.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "GameObject.h"
#include "Component/ColliderRect.h"
#include "Document.h"
#include "DocxInven.h"

CDocxTest::CDocxTest()
{
	m_eComType = (COMPONENT_TYPE)DT_DOCXTEST;
	m_pAnimation = nullptr;
	m_bMouseOn = false;
}

CDocxTest::CDocxTest(const CDocxTest & doc)	:
	CUserComponent(doc)
{
	m_pAnimation = nullptr;
}

CDocxTest::~CDocxTest()
{
	SAFE_RELEASE(m_pAnimation);
}

void CDocxTest::AfterClone()
{
}

bool CDocxTest::Init()
{
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("DTRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetDecalEnable(false);
	pRenderer->Enable2DRenderer();

	m_pObject->SetRenderGroup(RG_UI);

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "DocxTest", TEXT("UI/Document/Stage2/NoClick_TestDocx.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(220.f, 53.f, 1.f);
	m_pTransform->SetWorldPivot(0.5f, 0.5f, 0.f);

	m_pAnimation = m_pObject->AddComponent<CAnimation2D>("DTAnimation");

	vector<Clip2DFrame>	vecClipFrame;
	Clip2DFrame	tFrame = {};

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(320.f, 73.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("DT_NoClick", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"NoClick_DT", TEXT("UI/Document/Stage2/NoClick_TestDocx.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(320.f, 73.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("DT_Click", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"Click_DT", TEXT("UI/Document/Stage2/Click_TestDocx.png"));

	vecClipFrame.clear();

	CColliderRect* pBody = AddComponent<CColliderRect>("DGBody");

	pBody->SetCollisionCallback(CCT_ENTER, this, &CDocxTest::Hit);
	pBody->SetCollisionCallback(CCT_LEAVE, this, &CDocxTest::MouseOut);

	pBody->SetCollisionGroup("UI");
	pBody->SetInfo(Vector3(0.f, 0.f, 0.f), Vector3(220.f, 53.f, 0.f));

	SAFE_RELEASE(pBody);

	return true;
}

int CDocxTest::Input(float fTime)
{
	return 0;
}

int CDocxTest::Update(float fTime)
{
	if (m_bMouseOn)
	{
		m_pAnimation->ChangeClip("DT_Click");

		if (KEYPRESS("LButton"))
		{
			CGameObject*	pDocObj = CGameObject::FindObject("Document");
			pDocObj->SetEnable(true);

			CDocument* pDoc = pDocObj->FindComponentFromType<CDocument>((COMPONENT_TYPE)IT_DOCXICON);

			pDoc->ChangeClip("TestDocx720");

			SAFE_RELEASE(pDoc);
			SAFE_RELEASE(pDocObj);
		}
	}

	else if (!m_bMouseOn)
	{
		m_pAnimation->ChangeClip("DT_NoClick");
	}

	return 0;
}

int CDocxTest::LateUpdate(float fTime)
{
	return 0;
}

void CDocxTest::Collision(float fTime)
{
}

void CDocxTest::Render(float fTime)
{
}

CDocxTest * CDocxTest::Clone()
{
	return new CDocxTest(*this);
}

void CDocxTest::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_bMouseOn = true;
	}
}

void CDocxTest::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_bMouseOn = false;
	}
}
