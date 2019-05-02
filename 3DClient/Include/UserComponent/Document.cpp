#include "../ClientHeader.h"
#include "Document.h"

CDocument::CDocument()	:
	m_pAnimation(nullptr)
{
	m_eComType = (COMPONENT_TYPE)IT_DOCXICON;
}

CDocument::CDocument(const CDocument & doc)	:
	CUserComponent(doc)
{
}

CDocument::~CDocument()
{
	SAFE_RELEASE(m_pAnimation);
}

void CDocument::ChangeClip(const string & strName)
{
	m_pAnimation->ChangeClip(strName);
}

void CDocument::AfterClone()
{
}

bool CDocument::Init()
{
	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("DocRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetDecalEnable(false);
	pRenderer->Enable2DRenderer();

	m_pObject->SetRenderGroup(RG_UI);

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "Document", TEXT("UI/Document/MobileMessage.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	CTransform*		pTransform = m_pObject->GetTransform();
	pTransform->SetWorldScale(600.f, 600.f, 1.f);
	pTransform->SetWorldPos(830.f, 350.f, 1.f);
	pTransform->SetWorldPivot(0.5f, 0.5f, 0.f);

	SAFE_RELEASE(pTransform);

	m_pAnimation = m_pObject->AddComponent<CAnimation2D>("DocAnimation");

	vector<Clip2DFrame>	vecClipFrame;
	Clip2DFrame	tFrame = {};

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(800, 600);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("Message_Empty", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"Empty_Message", TEXT("UI/Document/MessageEmpty.png"));

	vecClipFrame.clear();

	tFrame.vLT = Vector2(0.f, 0.f);
	tFrame.vRB = Vector2(720.f, 720.f);
	vecClipFrame.push_back(tFrame);

	m_pAnimation->AddClip("MobileMessage", A2D_ATLAS, AO_LOOP, 1.f, vecClipFrame,
		"MessageMobile", TEXT("UI/Document/MobileMessage.png"));

	vecClipFrame.clear();

	return true;
}

int CDocument::Input(float fTime)
{
	return 0;
}

int CDocument::Update(float fTime)
{
	return 0;
}

int CDocument::LateUpdate(float fTime)
{
	return 0;
}

void CDocument::Collision(float fTime)
{
}

void CDocument::Render(float fTime)
{
}

CDocument * CDocument::Clone()
{
	return new CDocument(*this);
}
