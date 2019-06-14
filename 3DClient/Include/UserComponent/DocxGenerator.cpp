#include "../ClientHeader.h"
#include "DocxGenerator.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "GameObject.h"
#include "Component/ColliderRect.h"
#include "Document.h"
#include "DocxInven.h"
#include "../GameManager.h"

CDocxGenerator::CDocxGenerator()
{
	m_eComType = (COMPONENT_TYPE)DT_DOCXGENERATOR;
	m_pAnimation = nullptr;
	m_bMouseOn = false;
}

CDocxGenerator::CDocxGenerator(const CDocxGenerator & doc)	:
	CUserComponent(doc)
{
	m_pAnimation = nullptr;
}

CDocxGenerator::~CDocxGenerator()
{
	SAFE_RELEASE(m_pAnimation);
}

void CDocxGenerator::AfterClone()
{
}

bool CDocxGenerator::Init()
{
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("DGRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetDecalEnable(false);
	pRenderer->Enable2DRenderer();

	m_pObject->SetRenderGroup(RG_UI);

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "DocxGenerator", TEXT("UI/Document/Stage1/NoClick_Generator.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	//m_pTransform->SetWorldScale(330.f, 80.f, 1.f);
	m_pTransform->SetWorldScale(220.f, 53.f, 1.f);
	m_pTransform->SetWorldPivot(0.5f, 0.5f, 0.f);

	m_pAnimation = m_pObject->AddComponent<CAnimation2D>("DGAnimation");

	vector<Clip2DFrame>	vecClipFrame;
	Clip2DFrame	tFrame = {};

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(320.f, 73.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("DG_NoClick", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"NoClick_DG", TEXT("UI/Document/Stage1/NoClick_Generator.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(320.f, 73.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("DG_Click", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"Click_DG", TEXT("UI/Document/Stage1/Click_Generator.png"));

	vecClipFrame.clear();

	CColliderRect* pBody = AddComponent<CColliderRect>("DGBody");

	pBody->SetCollisionCallback(CCT_ENTER, this, &CDocxGenerator::Hit);
	pBody->SetCollisionCallback(CCT_LEAVE, this, &CDocxGenerator::MouseOut);

	pBody->SetCollisionGroup("UI");
	//pBody->SetInfo(Vector3(0.f, 0.f, 0.f), Vector3(330.f, 80.f, 0.f));
	pBody->SetInfo(Vector3(0.f, 0.f, 0.f), Vector3(220.f, 53.f, 0.f));

	SAFE_RELEASE(pBody);

	return true;
}

int CDocxGenerator::Input(float fTime)
{
	return 0;
}

int CDocxGenerator::Update(float fTime)
{
	if (m_bMouseOn)
	{
		m_pAnimation->ChangeClip("DG_Click");

		if (KEYPRESS("LButton"))
		{
			CGameObject*	pDocObj = CGameObject::FindObject("Document");
			pDocObj->SetEnable(true);

			CDocument* pDoc = pDocObj->FindComponentFromType<CDocument>((COMPONENT_TYPE)IT_DOCXICON);

			pDoc->ChangeClip("Generator_Docx");

			SAFE_RELEASE(pDoc);
			SAFE_RELEASE(pDocObj);
		}
	}

	else if (!m_bMouseOn)
	{
		m_pAnimation->ChangeClip("DG_NoClick");
	}

	return 0;
}

int CDocxGenerator::LateUpdate(float fTime)
{
	return 0;
}

void CDocxGenerator::Collision(float fTime)
{
}

void CDocxGenerator::Render(float fTime)
{
}

CDocxGenerator * CDocxGenerator::Clone()
{
	return new CDocxGenerator(*this);
}

void CDocxGenerator::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_bMouseOn = true;
	}
}

void CDocxGenerator::MouseOut(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MouseWindow")
	{
		m_bMouseOn = false;
	}
}
